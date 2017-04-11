//
// Created by miroslav on 3/28/17.
//

#ifndef BP_LETNODE_H
#define BP_LETNODE_H


#include "ast/StatementNode.h"

class LetStatementNode: public StatementNode {
public:
    LetStatementNode(const std::string &name, ExprNode *const expr);

    ~LetStatementNode() override;

    void print(Printer &printer) const override;

    void codegen(CodegenContext &context) override;

private:
    const std::string name;
    ExprNode * const expr;
};


#endif //BP_LETNODE_H
