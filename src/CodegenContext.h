//
// Created by miroslav on 3/28/17.
//

#ifndef BP_CODEGENCONTEXT_H
#define BP_CODEGENCONTEXT_H


#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

class CodegenContext {
public:
    CodegenContext(llvm::LLVMContext &llvmContext,
                   llvm::Module *const module,
                   llvm::IRBuilder<> &builder);

    virtual void codegenReturn(llvm::Value *value);
    virtual void addValue(std::string name, llvm::Value *value);
    virtual void addVariable(std::string name, llvm::Value *value);
    virtual void storeValue(std::string name, llvm::Value *value);
    virtual llvm::Value* getValue(std::string name);

public:
    llvm::Module * const module;
    llvm::LLVMContext &llvmContext;
    llvm::IRBuilder<> &builder;

    std::map<std::string,llvm::Value*> values;
};


#endif //BP_CODEGENCONTEXT_H
