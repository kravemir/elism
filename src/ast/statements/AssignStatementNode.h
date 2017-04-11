//
// Created by miroslav on 3/28/17.
//

#ifndef BP_ASSIGNNODE_H
#define BP_ASSIGNNODE_H


#include "ast/StatementNode.h"

class AssignStatementNode: public StatementNode {
public:
    AssignStatementNode(ExprNode *const targetExpr, ExprNode *const valueExpr);

    ~AssignStatementNode() override;

    void print(Printer &printer) const override;

    void codegen(CodegenContext &context) override;

private:
    ExprNode * const targetExpr;
    ExprNode * const valueExpr;
};


#endif //BP_ASSIGNNODE_H
