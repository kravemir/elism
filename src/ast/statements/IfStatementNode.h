//
// Created by miroslav on 4/6/17.
//

#ifndef BP_IFSTATEMENTNODE_H
#define BP_IFSTATEMENTNODE_H


class IfStatementNode: public StatementNode {
public:
    IfStatementNode(ExprNode *conditionExpr, StatementNode *statement);

    void print(Printer &printer) const override;

    void codegen(CodegenContext &context) override;

private:
    ExprNode *conditionExpr;
    StatementNode *statement;
};


#endif //BP_IFSTATEMENTNODE_H
