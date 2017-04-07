//
// Created by miroslav on 4/7/17.
//

#ifndef BP_FORSTATEMENT_H
#define BP_FORSTATEMENT_H


#include <ast/StatementNode.h>

class ForStatementNode: public StatementNode {
public:
    ForStatementNode(const std::string &name, ExprNode *expr, StatementNode *statement);

    void print(Printer &printer) const override;

    void codegen(CodegenContext &context) override;

private:
    std::string name;
    ExprNode *expr;
    StatementNode *statement;
};


#endif //BP_FORSTATEMENT_H
