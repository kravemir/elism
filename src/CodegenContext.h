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

    virtual llvm::Value* createAlloc(llvm::Type *allocType, llvm::Value *allocSize, llvm::Value *allocArraySize);

    virtual llvm::Value* getRegion();

public:
    llvm::Module * const module;
    llvm::LLVMContext &llvmContext;
    llvm::IRBuilder<> &builder;

    llvm::Type* regionType = 0;
    llvm::Value* region = 0;
    std::string defaultRegion;

    std::map<std::string,CodegenValue*> values;
    std::map<std::string,CodegenType*> types;
};

class CodegenType {
public:
    CodegenType(llvm::Type *const storeType);
    CodegenType(llvm::Type *const storeType, CodegenType *const callReturnType);

    virtual CodegenValue* doCall(CodegenContext &ctx, CodegenValue *value, const std::vector<std::string> &regions,
                                 const std::vector<CodegenValue *> &args, const llvm::Twine &Name = "");
    virtual CodegenValue* getChild(CodegenContext &ctx, CodegenValue *value, std::string name);

    bool isCallable() const {
        return callReturnType != nullptr;
    }

    virtual std::string toString() const = 0;

    // TODO: workaround, split class to named type generator and type
    virtual CodegenType* withRegions(CodegenContext &ctx, const std::vector<std::string> &regions);

public:
    llvm::Type * const storeType;
    CodegenType * const callReturnType;

    virtual CodegenValue *getElement(CodegenContext &ctx, CodegenValue *value, CodegenValue *index);

    virtual CodegenValue *const getDefault(CodegenContext &context);

    /**
     * Compares self to other, comparison shall check data types, and regions.
     *
     * Region mapping argument is for function calls. It maps invocation regions to regions in function declaration.
     *
     * @param other
     * @param regionsRemap maps region name of value (self) to name of regions for other
     * @return true/false
     */
    virtual bool equals(CodegenType *other, const std::map<std::string,std::string> &regionsRemap) = 0;

    virtual CodegenType *withRemapRegions(CodegenContext &context, const std::map<std::string, std::string> &map) = 0;
};

class CodegenValue {
public:
    CodegenValue(CodegenType *type, llvm::Value *value, llvm::Value *storeAddress = nullptr);

    virtual CodegenValue* doCall(CodegenContext &ctx, const std::vector<std::string> &regions,
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

    CodegenType *getType(std::string name) override;


public:
    CodegenContext &parent;
    std::map<std::string,std::pair<CodegenType*,llvm::AllocaInst*>> variables;
};



#endif //BP_CODEGENCONTEXT_H
