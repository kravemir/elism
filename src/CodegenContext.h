//
// Created by miroslav on 3/28/17.
//

#ifndef BP_CODEGENCONTEXT_H
#define BP_CODEGENCONTEXT_H


#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

class CodegenValue;

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

class CodegenValue {
public:
    CodegenValue(llvm::Value *value);
    CodegenValue(llvm::Value *value, llvm::Type *const callReturnType);

    virtual CodegenValue* doCall(CodegenContext &ctx);

    bool isCallable() const {
        return callReturnType != nullptr;
    }

public:
    llvm::Value *value;
    llvm::Type * const callReturnType;
};


#endif //BP_CODEGENCONTEXT_H
