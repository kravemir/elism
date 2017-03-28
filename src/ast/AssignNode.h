//
// Created by miroslav on 3/28/17.
//

#ifndef BP_ASSIGNNODE_H
#define BP_ASSIGNNODE_H


#include "StatementNode.h"

class AssignNode: public StatementNode {
public:
    AssignNode(const std::string &name, ExprNode *const expr);

    void print(Printer &printer) const override;

    void codegen(CodegenContext &context) override;

private:
    const std::string name;
    ExprNode * const expr;
};


#endif //BP_ASSIGNNODE_H
