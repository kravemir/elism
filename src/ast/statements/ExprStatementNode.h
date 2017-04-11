//
// Created by miroslav on 4/6/17.
//

#ifndef BP_EXPRSTATEMENTNODE_H
#define BP_EXPRSTATEMENTNODE_H


#include <ast/StatementNode.h>

class ExprStatementNode: public StatementNode {
public:
    ExprStatementNode(ExprNode *expr);

    ~ExprStatementNode() override;

    void print(Printer &printer) const override;

    void codegen(CodegenContext &context) override;

private:
    ExprNode *expr;
};


#endif //BP_EXPRSTATEMENTNODE_H
