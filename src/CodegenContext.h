//
// Created by miroslav on 3/28/17.
//

#ifndef BP_CODEGENCONTEXT_H
#define BP_CODEGENCONTEXT_H


#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

class CodegenValue;
class CodegenType;
class CodegenContext;

class CodegenContext {
public:
    CodegenContext(llvm::LLVMContext &llvmContext,
                   llvm::Module *const module,
                   llvm::IRBuilder<> &builder);

    virtual void codegenReturn(CodegenValue *value);
    virtual void addValue(std::string name, CodegenValue *value);
    virtual void addVariable(std::string name, CodegenValue *value);
    virtual void storeValue(std::string name, CodegenValue *value);
    virtual CodegenValue * getValue(std::string name);

public:
    llvm::Module * const module;
    llvm::LLVMContext &llvmContext;
    llvm::IRBuilder<> &builder;

    std::map<std::string,CodegenValue*> values;
};

class CodegenType {
public:
    CodegenType(llvm::Type *const storeType);
    CodegenType(llvm::Type *const storeType, CodegenType *const callReturnType);

    virtual CodegenValue* doCall(CodegenContext &ctx, CodegenValue *value, const llvm::Twine &Name = "");
    virtual CodegenValue* getChild(CodegenContext &ctx, CodegenValue *value, std::string name);

    bool isCallable() const {
        return callReturnType != nullptr;
    }

public:
    llvm::Type * const storeType;
    CodegenType * const callReturnType;
};

class CodegenValue {
public:
    CodegenValue(CodegenType *type, llvm::Value *value, llvm::Value *storeAddress = nullptr);

    virtual CodegenValue* doCall(CodegenContext &ctx, const llvm::Twine &Name = "");
    virtual void doStore(CodegenContext &ctx, CodegenValue *value);

    bool isCallable() const {
        return type->isCallable();
    }

public:
    CodegenType* type;
    llvm::Value *value;
    llvm::Value *storeAddress;
};

class ChildCodegenContext: public CodegenContext {
public:
    ChildCodegenContext(CodegenContext &parent);

public:
    CodegenContext &parent;
};

struct FunctionType: CodegenType {
    FunctionType(CodegenType *const callReturnType) : CodegenType(nullptr, callReturnType) {
        assert(callReturnType);
    }

    CodegenValue *doCall(CodegenContext &ctx, CodegenValue *value, const llvm::Twine &Name = "") override {
        return new CodegenValue(callReturnType,ctx.builder.CreateCall(value->value,{},Name));
    }
};



#endif //BP_CODEGENCONTEXT_H
