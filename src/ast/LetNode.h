//
// Created by miroslav on 3/28/17.
//

#ifndef BP_LETNODE_H
#define BP_LETNODE_H


#include "StatementNode.h"

class LetNode: public StatementNode {
public:
    LetNode(const std::string &name, ExprNode *const expr);

    void print(Printer &printer) const override;

    void codegen(CodegenContext &context) override;

private:
    const std::string name;
    ExprNode * const expr;
};


#endif //BP_LETNODE_H
