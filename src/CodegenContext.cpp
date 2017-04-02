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

CodegenType::CodegenType(llvm::Type *const storeType)
        : storeType(storeType),
          callReturnType(nullptr)
{
}

CodegenType::CodegenType(llvm::Type *const storeType, CodegenType *const callReturnType)
        : storeType(storeType),
          callReturnType(callReturnType)
{
}

CodegenValue *CodegenType::doCall(CodegenContext &ctx, CodegenValue *value) {
    assert(0);
    return nullptr;
}

CodegenValue *CodegenType::getChild(CodegenContext &ctx, CodegenValue *value, std::string name) {
    assert(0);
    return nullptr;
}

CodegenValue::CodegenValue(CodegenType *type, llvm::Value *value)
        : type(type),
          value(value)
{
    assert(type);
    assert(value);
}

CodegenValue *CodegenValue::doCall(CodegenContext &ctx) {
    return type->doCall(ctx, this);
}

ChildCodegenContext::ChildCodegenContext(CodegenContext &parent)
        : CodegenContext(parent.llvmContext, parent.module, parent.builder),
          parent(parent) {
}
