//
// Created by miroslav on 4/13/17.
//

#ifndef BP_ONSTATEMENTNODE_H
#define BP_ONSTATEMENTNODE_H


#include <ast/StatementNode.h>

class OnStatementNode: public StatementNode {
public:
    OnStatementNode(const std::string &name, ExprNode *expr, StatementNode *stmt);

    void print(Printer &printer) const override;

    void codegen(CodegenContext &context) override;

private:
    std::string name;
    ExprNode *expr;
    StatementNode *stmt;
};


#endif //BP_ONSTATEMENTNODE_H
