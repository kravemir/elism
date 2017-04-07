//
// Created by miroslav on 3/31/17.
//

#include "ClassNode.h"

#include <codegen/FunctionType.h>

using namespace llvm;

ClassNode::ClassNode(const std::string &name, const std::vector<StatementNode *> &statements)
        : name(name),
          statements(statements)
{}

void ClassNode::print(Printer &printer) {
    printer.println("TODO");
}

void ClassNode::codegen(CodegenContext &context) {
    StructType *classType = StructType::create(context.llvmContext,"class." + name + ".Instance");
    Type *classPtrType = PointerType::get(classType,0);
    ClassType *cClassType = new ClassType(classPtrType);

    llvm::FunctionType *FT_init = llvm::FunctionType::get(Type::getVoidTy(context.llvmContext), {classPtrType}, false);
    Function *F_init = Function::Create(FT_init, Function::ExternalLinkage, "class." + name + ".init", context.module);

    ClassTypeContext typeContext(context);
    typeContext.classType = cClassType;
    {
        // Create a new basic block to start insertion into.
        BasicBlock *BB = BasicBlock::Create(context.llvmContext, "entry.initializers", F_init);
        context.builder.SetInsertPoint(BB);

        for (StatementNode *var : statements) {
            if(dynamic_cast<VarStatementNode*>(var)) // TODO: codegen as class initialzier
                var->codegen(typeContext);
        }

        std::vector<Type *> types;
        int idx = 0;
        for (auto v : typeContext.variables) {
            v.second->value->dump();
            cClassType->children[v.first] = std::make_pair(idx++,v.second->type);
            types.push_back(v.second->value->getType());
        }
        classType->setBody(types);

        idx = 0;
        for (auto v : typeContext.variables) {
            Value *ptr = typeContext.builder.CreateGEP(
                    (Argument *) (F_init->arg_begin()),
                    (std::vector<Value *>) {
                            Constant::getNullValue(IntegerType::getInt64Ty(typeContext.llvmContext)),
                            ConstantInt::get(typeContext.llvmContext, APInt((unsigned) 32, (uint64_t) idx++))
                    },
                    "var." + v.first + ".addr"
            );
            typeContext.builder.CreateStore(v.second->value, ptr);
        }

        context.builder.CreateRetVoid();
        classType->dump();
        F_init->dump();
    }

    for (StatementNode *stmt : statements) {
        stmt->codegenAsClassStatement(typeContext);
    }


    llvm::FunctionType *FT_new = llvm::FunctionType::get(classPtrType, {}, false);
    Function *F_new = Function::Create(FT_new, Function::ExternalLinkage, "class." + name + ".new", context.module);

    {
        // Create a new basic block to start insertion into.
        BasicBlock *BB = BasicBlock::Create(context.llvmContext, "entry.initializers", F_new);
        context.builder.SetInsertPoint(BB);

        Type* ITy = Type::getInt64PtrTy(context.llvmContext);
        Constant* AllocSize = ConstantExpr::getSizeOf(classType);
        AllocSize = ConstantExpr::getTruncOrBitCast(AllocSize, ITy);
        Instruction* Malloc = CallInst::CreateMalloc(context.builder.GetInsertBlock(),
                                                     ITy, classType, AllocSize,
                                                     nullptr, nullptr, name + ".instance");

        context.builder.Insert(Malloc);
        context.builder.CreateCall(F_init, {Malloc});
        context.builder.CreateRet(Malloc);
        F_new->dump();
    }

    ::FunctionType *CFT = new ::FunctionType(cClassType);
    context.addValue(name, new CodegenValue(CFT,F_new));
    context.addType(name, cClassType);

}

struct ClassFunctionType: CodegenType {
    ClassFunctionType(Type *const storeType, CodegenType *const callReturnType)
            : CodegenType(storeType, callReturnType)
    {}

    CodegenValue *
    doCall(CodegenContext &ctx, CodegenValue *value, std::vector<CodegenValue *> &args, const Twine &Name) override {
        return CodegenType::doCall(ctx, value, args, Name);
    }
};

CodegenValue *ClassType::getChild(CodegenContext &ctx, CodegenValue *value, std::string name) {
    auto it = children.find(name);
    if(it != children.end()) {
        llvm::Value *ptr = ctx.builder.CreateGEP(
                value->value,
                (std::vector<llvm::Value *>) {
                        llvm::Constant::getNullValue(llvm::IntegerType::getInt64Ty(ctx.llvmContext)),
                        llvm::ConstantInt::get(ctx.llvmContext, llvm::APInt((unsigned) 32, (uint64_t) it->second.first))
                },
                "child." + name + ".addr"
        );
        return new CodegenValue(it->second.second,ctx.builder.CreateLoad(ptr,"child."+name),ptr);
    }
    return nullptr;
}
