//
// Created by miroslav on 4/6/17.
//

#ifndef BP_WHILESTATEMENTNODE_H
#define BP_WHILESTATEMENTNODE_H


#include <ast/StatementNode.h>

class WhileStatementNode: public StatementNode {
public:
    WhileStatementNode(ExprNode *condition, StatementNode *statement);

    void print(Printer &printer) const override;

    void codegen(CodegenContext &context) override;

private:
    ExprNode *condition;
    StatementNode *statement;
};


#endif //BP_WHILESTATEMENTNODE_H
