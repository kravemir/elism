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
    virtual void addType(std::string name, CodegenType *type);
    virtual llvm::AllocaInst* createAlloca(std::string name, CodegenValue *value);
    virtual void addVariable(std::string name, CodegenValue *value);

    virtual CodegenValue * getValue(std::string name);
    virtual CodegenType * getType(std::string name);

    virtual llvm::Instruction* createAlloc(llvm::Type *allocType, llvm::Value *allocSize, llvm::Value *allocArraySize);

    virtual llvm::Value* getRegion();

public:
    llvm::Module * const module;
    llvm::LLVMContext &llvmContext;
    llvm::IRBuilder<> &builder;

    llvm::Type* regionType = 0;
    llvm::Value* region = 0;

    std::map<std::string,CodegenValue*> values;
    std::map<std::string,CodegenType*> types;
};

class CodegenType {
public:
    CodegenType(llvm::Type *const storeType);
    CodegenType(llvm::Type *const storeType, CodegenType *const callReturnType);

    virtual CodegenValue* doCall(CodegenContext &ctx, CodegenValue *value, const std::vector<CodegenValue *> &args,
                                 const llvm::Twine &Name = "");
    virtual CodegenValue* getChild(CodegenContext &ctx, CodegenValue *value, std::string name);

    bool isCallable() const {
        return callReturnType != nullptr;
    }

public:
    llvm::Type * const storeType;
    CodegenType * const callReturnType;

    virtual CodegenValue *getElement(CodegenContext &ctx, CodegenValue *value, CodegenValue *index);

    virtual CodegenValue *const getDefault(CodegenContext &context);
};

class CodegenValue {
public:
    CodegenValue(CodegenType *type, llvm::Value *value, llvm::Value *storeAddress = nullptr);

    virtual CodegenValue* doCall(CodegenContext &ctx,
                                 const std::vector<CodegenValue *> &args,
                                 const llvm::Twine &Name = "");
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

    CodegenValue *getValue(std::string name) override;
    llvm::AllocaInst *createAlloca(std::string name, CodegenValue *value) override;

    void addVariable(std::string name, CodegenValue *value) override;

    llvm::Value *getRegion() override;


public:
    CodegenContext &parent;
    std::map<std::string,std::pair<CodegenType*,llvm::AllocaInst*>> variables;
};



#endif //BP_CODEGENCONTEXT_H
