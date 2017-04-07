//
// Created by miroslav on 3/31/17.
//

#ifndef BP_CLASSNODE_H
#define BP_CLASSNODE_H

#include <ast/statements/VarStatementNode.h>
#include "CodegenContext.h"

struct ClassType: CodegenType {
    ClassType(llvm::Type *const storeType) : CodegenType(storeType) {}

    CodegenValue *getChild(CodegenContext &ctx, CodegenValue *value, std::string name) override;

    std::map<std::string,std::pair<int,CodegenType*>> children;
    std::map<std::string,CodegenValue*> functions;
};

struct ClassTypeContext: ChildCodegenContext {
    ClassTypeContext(CodegenContext &parent)
            : ChildCodegenContext(parent) {}

    void addVariable(std::string name, CodegenValue *value) override {
        variables.push_back({name,value});
    }

    std::vector<std::pair<std::string,CodegenValue*>> variables;
    std::vector<std::pair<std::string,CodegenValue*>> functions;
    ClassType *classType;
};

class ClassNode {
public:
    ClassNode(const std::string &name, const std::vector<StatementNode *> &statements);

    virtual void print(Printer &printer);
    void codegen(CodegenContext &context);

private:
    std::string name;
    std::vector<StatementNode*> statements;
};


#endif //BP_CLASSNODE_H
