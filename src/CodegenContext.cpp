//
// Created by miroslav on 3/28/17.
//

#include "CodegenContext.h"

CodegenContext::CodegenContext(llvm::LLVMContext &llvmContext,
                               llvm::Module *const module,
                               llvm::IRBuilder<> &builder)
        : llvmContext(llvmContext),
          module(module),
          builder(builder)
{}

void CodegenContext::codegenReturn(llvm::Value *value) {
    assert(0 && "Current context doesn't support return");
}
