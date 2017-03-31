//
// Created by miroslav on 3/28/17.
//

#ifndef BP_VARNODE_H
#define BP_VARNODE_H


#include "ast/StatementNode.h"
#include "ast/TypeNode.h"

class VarStatementNode: public StatementNode {
public:
    VarStatementNode(const std::string &name, const TypeNode *type, ExprNode *const expr);

    void print(Printer &printer) const override;

    void codegen(CodegenContext &context) override;

private:
    const std::string name;
    const TypeNode *type;
    ExprNode * const expr;
};


#endif //BP_VARNODE_H
