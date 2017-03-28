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

void CodegenContext::addVariable(std::string name, llvm::Value *value) {
    assert(0 && "Current context doesn't support variables");
}

void CodegenContext::storeValue(std::string name, llvm::Value *value) {
    assert(0 && "Current context doesn't support variables");
}


void CodegenContext::addValue(std::string name, llvm::Value *value) {
    values[name] = value;
}

llvm::Value* CodegenContext::getValue(std::string name) {
    auto it = values.find(name);
    if(it != values.end())
        return it->second;
    return nullptr;
}