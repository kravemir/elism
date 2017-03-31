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

void CodegenContext::codegenReturn(CodegenValue *value) {
    assert(0 && "Current context doesn't support return");
}

void CodegenContext::addVariable(std::string name, CodegenValue *value) {
    assert(0 && "Current context doesn't support variables");
}

void CodegenContext::storeValue(std::string name, CodegenValue *value) {
    assert(0 && "Current context doesn't support variables");
}


void CodegenContext::addValue(std::string name, CodegenValue *value) {
    //printf("Add %p: %s\n",this,name.c_str());
    assert(value);
    values[name] = value;
}

CodegenValue * CodegenContext::getValue(std::string name) {
    //printf("Get %p: %s\n",this,name.c_str());
    auto it = values.find(name);
    if(it != values.end())
        return it->second;
    /*printf("Get: -- not found --\n");
    for(auto it : values) {
        printf("Contains: %s\n", it.first.c_str());
    }*/
    return nullptr;
}

CodegenValue::CodegenValue(llvm::Value *value)
        : value(value),
          callReturnType(nullptr)
{
    assert(value);
}

CodegenValue::CodegenValue(llvm::Value *value, llvm::Type *const callReturnType)
        : value(value),
          callReturnType(callReturnType)
{
    assert(value);
}

CodegenValue *CodegenValue::doCall(CodegenContext &ctx) {
    assert(0);
    return nullptr;
}
