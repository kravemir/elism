//
// Created by miroslav on 3/31/17.
//

#ifndef BP_CLASSNODE_H
#define BP_CLASSNODE_H

#include <ast/statements/VarStatementNode.h>
#include "CodegenContext.h"

class ClassNode {
public:
    ClassNode(const std::string &name, const std::vector<VarStatementNode *> &variables);

    virtual void print(Printer &printer);
    void codegen(CodegenContext &context);

private:
    std::string name;
    std::vector<VarStatementNode*> variables;
};


#endif //BP_CLASSNODE_H
