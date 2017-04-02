//
// Created by miroslav on 3/31/17.
//

#include "ClassNode.h"

using namespace llvm;

ClassNode::ClassNode(const std::string &name, const std::vector<VarStatementNode *> &variables) : name(name), variables(
        variables) {}

void ClassNode::print(Printer &printer) {
    printer.println("TODO");
}

struct ClassTypeContext: ChildCodegenContext {
    ClassTypeContext(CodegenContext &parent)
            : ChildCodegenContext(parent) {}

    void addVariable(std::string name, CodegenValue *value) override {
        variables.push_back({name,value});
    }

    std::vector<std::pair<std::string,CodegenValue*>> variables;
};

struct ClassType: CodegenType {
    ClassType(Type *const storeType) : CodegenType(storeType) {}

    CodegenValue *getChild(CodegenContext &ctx, CodegenValue *value, std::string name) override {
        auto it = children.find(name);
        if(it != children.end()) {
            Value *ptr = ctx.builder.CreateGEP(
                    value->value,
                    (std::vector<Value *>) {
                            Constant::getNullValue(IntegerType::getInt64Ty(ctx.llvmContext)),
                            ConstantInt::get(ctx.llvmContext, APInt((unsigned) 32, (uint64_t) it->second.first))
                    },
                    "child." + name + ".addr"
            );
            return new CodegenValue(it->second.second,ctx.builder.CreateLoad(ptr,"child."+name),ptr);
        }
        assert(0);
        return nullptr;
    }

    std::map<std::string,std::pair<int,CodegenType*>> children;
};

void ClassNode::codegen(CodegenContext &context) {
    StructType *classType = StructType::create(context.llvmContext,"class." + name + ".Instance");
    Type *classPtrType = PointerType::get(classType,0);
    ClassType *cClassType = new ClassType(classPtrType);

    llvm::FunctionType *FT_init = llvm::FunctionType::get(Type::getVoidTy(context.llvmContext), {classPtrType}, false);
    Function *F_init = Function::Create(FT_init, Function::ExternalLinkage, "class." + name + ".init", context.module);

    {
        // Create a new basic block to start insertion into.
        BasicBlock *BB = BasicBlock::Create(context.llvmContext, "entry.initializers", F_init);
        context.builder.SetInsertPoint(BB);

        ClassTypeContext typeContext(context);
        for (VarStatementNode *var : variables) {
            var->codegen(typeContext);
        }

        std::vector<Type *> types;
        int idx = 0;
        for (auto v : typeContext.variables) {
            v.second->value->dump();
            cClassType->children[v.first] = std::make_pair(idx,v.second->type);
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

}

