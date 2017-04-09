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

llvm::AllocaInst *CodegenContext::createAlloca(std::string name, CodegenValue *value) {
    assert(0 && "Current context doesn't support allocas");
}

void CodegenContext::addValue(std::string name, CodegenValue *value) {
    //printf("Add %p: %s\n",this,name.c_str());
    assert(value);
    values[name] = value;
}

void CodegenContext::addType(std::string name, CodegenType *type) {
    assert(type);
    types[name] = type;
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

CodegenType *CodegenContext::getType(std::string name) {
    auto it = types.find(name);
    if(it != types.end())
        return it->second;
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

CodegenValue *CodegenType::doCall(CodegenContext &ctx, CodegenValue *value, const std::vector<CodegenValue *> &args,
                                  const llvm::Twine &Name) {
    assert(0);
    return nullptr;
}

CodegenValue *CodegenType::getChild(CodegenContext &ctx, CodegenValue *value, std::string name) {
    assert(0);
    return nullptr;
}

CodegenValue *CodegenType::getElement(CodegenContext &ctx, CodegenValue *value, CodegenValue *index) {
    assert(0);
    return nullptr;
}

CodegenValue::CodegenValue(CodegenType *type, llvm::Value *value, llvm::Value *storeAddress)
        : type(type),
          value(value),
          storeAddress(storeAddress)
{
    assert(type);
    assert(value);
}

CodegenValue *CodegenValue::doCall(CodegenContext &ctx, const std::vector<CodegenValue *> &args,
                                   const llvm::Twine &Name) {
    return type->doCall(ctx, this, args, Name);
}

void CodegenValue::doStore(CodegenContext &ctx, CodegenValue *value) {
    // TODO: type check
    ctx.builder.CreateStore(value->value, storeAddress);
}

ChildCodegenContext::ChildCodegenContext(CodegenContext &parent)
        : CodegenContext(parent.llvmContext, parent.module, parent.builder),
          parent(parent) {
}

void ChildCodegenContext::addVariable(std::string name, CodegenValue *value) {
    llvm::AllocaInst* alloca = createAlloca(name,value);
    variables[name] = std::make_pair(value->type,alloca);
}

CodegenValue *ChildCodegenContext::getValue(std::string name) {
    auto it = variables.find(name);
    if(it != variables.end()) {
        return new CodegenValue(it->second.first,builder.CreateLoad(it->second.second,name),it->second.second);
    }
    CodegenValue *value = CodegenContext::getValue(name);
    if(value == nullptr) {
        return parent.getValue(name);
    }
    return value;
}

llvm::AllocaInst *ChildCodegenContext::createAlloca(std::string name, CodegenValue *value) {
    return parent.createAlloca(name,value);
}
