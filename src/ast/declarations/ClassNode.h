//
// Created by miroslav on 3/31/17.
//

#ifndef BP_CLASSNODE_H
#define BP_CLASSNODE_H

#include <ast/statements/VarStatementNode.h>
#include "CodegenContext.h"

struct ClassType: CodegenType {
    ClassType(std::string name,llvm::Type *const storeType, ClassType *super, const std::string &region) : CodegenType(storeType), name(name), super(super), region(region) {}

    CodegenValue *getChild(CodegenContext &ctx, CodegenValue *value, std::string name) override;

    bool equals(CodegenType *pType, const std::map<std::string,std::string> &regionsRemap) override;

    std::string toString() const override;

    CodegenType *withRegions(CodegenContext &ctx, const std::vector<std::string> &regions) override;

    CodegenType *withRemapRegions(CodegenContext &context, const std::map<std::string, std::string> &map) override;

    std::string name;
    ClassType* super;
    std::map<std::string,std::pair<int,CodegenType*>> children;
    std::map<std::string,CodegenValue*> functions;

    llvm::Function *initF;

    std::string region;

    ClassType *base;
};

struct ClassTypeContext: ChildCodegenContext {
    ClassTypeContext(CodegenContext &parent)
            : ChildCodegenContext(parent) {}

    void addVariable(std::string name, CodegenValue *value) override {
        variables.push_back({name,value});
    }

    std::vector<std::pair<std::string,CodegenValue*>> variables;
    ClassType *classType;
};

class ClassNode {
public:
    ClassNode(const std::string &name, const std::vector<StatementNode *> &statements);
    ClassNode(const std::string &name, const std::string &super, const std::vector<StatementNode *> &statements);
    ~ClassNode();

    virtual void print(Printer &printer);
    void codegen(CodegenContext &context);

private:
    std::string name, super;
    bool hasSuper;
    std::vector<StatementNode*> statements;
};


#endif //BP_CLASSNODE_H
