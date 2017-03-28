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

public:
    llvm::Module * const module;
    llvm::LLVMContext &llvmContext;
    llvm::IRBuilder<> &builder;
};


#endif //BP_CODEGENCONTEXT_H
