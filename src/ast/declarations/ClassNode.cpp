//
// Created by miroslav on 3/31/17.
//

#include "ClassNode.h"

#include <codegen/FunctionType.h>
#include <llvm/IR/Module.h>
#include <ast/expressions/CallExprNode.h>
#include <ast/expressions/NameExprNode.h>

using namespace llvm;

ClassNode::ClassNode(const std::string &name, const std::vector<StatementNode *> &statements)
        : name(name),
          hasSuper(false),
          statements(statements)
{}

ClassNode::ClassNode(const std::string &name, const std::string &super, const std::vector<StatementNode *> &statements)
        : name(name),
          super(super),
          hasSuper(true),
          statements(statements)
{}

void ClassNode::print(Printer &printer) {
    printer.println("TODO");
}

void ClassNode::codegen(CodegenContext &context) {
    ClassType *super = nullptr;
    if(hasSuper) {
        super = dynamic_cast<ClassType*>(context.getType(this->super));
        assert(super);
    }

    StructType *classType = StructType::create(context.llvmContext,"class." + name + ".Instance");
    Type *classPtrType = PointerType::get(classType,0);
    ClassType *cClassType = new ClassType(classPtrType,super);

    llvm::FunctionType *FT_init = llvm::FunctionType::get(Type::getVoidTy(context.llvmContext), {context.regionType,classPtrType}, false);
    Function *F_init = Function::Create(FT_init, Function::ExternalLinkage, "class." + name + ".init", context.module);
    cClassType->initF = F_init;

    ClassTypeContext typeContext(context);
    typeContext.classType = cClassType;
    {
        // Create a new basic block to start insertion into.
        BasicBlock *BB = BasicBlock::Create(context.llvmContext, "entry.initializers", F_init);
        context.builder.SetInsertPoint(BB);
        typeContext.region = (Argument*)F_init->arg_begin();

        for (StatementNode *var : statements) {
            if(dynamic_cast<VarStatementNode*>(var)) // TODO: codegen as class initialzier
                var->codegen(typeContext);
        }


        std::vector<Type *> types;
        int idx = super ? 1 : 0;
        if(super){
            types.push_back(super->storeType->getPointerElementType());
        }
        for (auto v : typeContext.variables) {
            cClassType->children[v.first] = std::make_pair(idx++,v.second->type);
            types.push_back(v.second->value->getType());
        }
        if(super) {
            typeContext.builder.CreateCall(super->initF, {(Argument*)F_init->arg_begin()});
        }
        classType->setBody(types);

        idx = super ? 1 : 0;
        for (auto v : typeContext.variables) {
            Value *ptr = typeContext.builder.CreateGEP(
                    (Argument *) (++(F_init->arg_begin())),
                    (std::vector<Value *>) {
                            Constant::getNullValue(IntegerType::getInt64Ty(typeContext.llvmContext)),
                            ConstantInt::get(typeContext.llvmContext, APInt((unsigned) 32, (uint64_t) idx++))
                    },
                    "var." + v.first + ".addr"
            );
            typeContext.builder.CreateStore(v.second->value, ptr);
        }

        context.builder.CreateRetVoid();
    }

    for (StatementNode *stmt : statements) {
        stmt->codegenAsClassStatement(typeContext);
    }


    llvm::FunctionType *FT_new = llvm::FunctionType::get(classPtrType, {context.regionType}, false);
    Function *F_new = Function::Create(FT_new, Function::ExternalLinkage, "class." + name + ".new", context.module);

    {
        // Create a new basic block to start insertion into.
        BasicBlock *BB = BasicBlock::Create(context.llvmContext, "entry.initializers", F_new);
        context.builder.SetInsertPoint(BB);
        context.region = (Argument*)F_new->arg_begin();

        Type* ITy = Type::getInt64PtrTy(context.llvmContext);
        Constant* AllocSize = ConstantExpr::getSizeOf(classType);
        AllocSize = ConstantExpr::getTruncOrBitCast(AllocSize, ITy);
        Value* Malloc = context.createAlloc(classType, AllocSize,nullptr);

        context.builder.CreateCall(F_init, {context.region,Malloc});
        context.builder.CreateRet(Malloc);
    }

    ::FunctionType *CFT = new ::FunctionType(FT_new,cClassType);
    context.addValue(name, new CodegenValue(CFT,F_new));
    context.addType(name, cClassType);

}

ClassNode::~ClassNode() {
    for(StatementNode *s : statements)
        delete s;
}

struct ClassFunctionType: CodegenType {
    ClassFunctionType(Type * storeType, CodegenType *const callReturnType)
            : CodegenType(storeType, callReturnType)
    {}

    CodegenValue * doCall(CodegenContext &ctx, CodegenValue *value, const std::vector<CodegenValue *> &args,
                          const Twine &Name) override {
        std::vector<llvm::Value*> values = {ctx.region};
        values.push_back(ctx.builder.CreateExtractValue(value->value,{0}));
        for(CodegenValue *v : args)
            values.push_back(v->value);
        return new CodegenValue(callReturnType,ctx.builder.CreateCall(ctx.builder.CreateExtractValue(value->value,{1}),values,Name));
    }

    bool equals(CodegenType *pType) override {
        return false;
    }

    std::string toString() const override {
        return "TODO: cft";
    }
};

CodegenValue *ClassType::getChild(CodegenContext &ctx, CodegenValue *value, std::string name) {
    llvm::Value *valThis = value->value;

    ClassType *clst = this;
    while(clst) {
        {
            auto it = clst->children.find(name);
            if (it != clst->children.end()) {
                llvm::Value *ptr = ctx.builder.CreateGEP(
                        valThis,
                        (std::vector<llvm::Value *>) {
                                llvm::Constant::getNullValue(llvm::IntegerType::getInt64Ty(ctx.llvmContext)),
                                llvm::ConstantInt::get(ctx.llvmContext,
                                                       llvm::APInt((unsigned) 32, (uint64_t) it->second.first))
                        },
                        "child." + name + ".addr"
                );
                return new CodegenValue(it->second.second, ctx.builder.CreateLoad(ptr, "child." + name), ptr);
            }
        }
        {
            auto it = clst->functions.find(name);
            if (it != clst->functions.end()) {
                llvm::StructType *lType = StructType::get(
                        ctx.llvmContext,
                        {
                                storeType,
                                PointerType::get(it->second->type->storeType, 0)
                        }
                );

                ClassFunctionType *type = new ClassFunctionType(lType, it->second->type->callReturnType);
                Value *val = ConstantStruct::get(lType, {
                        Constant::getNullValue(storeType),
                        Constant::getNullValue(PointerType::get(it->second->type->storeType, 0)),
                });
                val = ctx.builder.CreateInsertValue(val, valThis, {0});
                val = ctx.builder.CreateInsertValue(val, it->second->value, {1});

                return new CodegenValue(type, val);
            }
        }
        clst = clst->super;
        if(clst) {
            valThis = ctx.builder.CreateGEP(valThis, {
                    ConstantInt::getNullValue(Type::getInt64Ty(ctx.llvmContext)),
                    ConstantInt::getNullValue(Type::getInt32Ty(ctx.llvmContext))
            });
        }
    }
    return nullptr;
}

bool ClassType::equals(CodegenType *pType) {
    return pType == this;
}

std::string ClassType::toString() const {
    return "TODO: class";
}
