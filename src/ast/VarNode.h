//
// Created by miroslav on 3/28/17.
//

#ifndef BP_VARNODE_H
#define BP_VARNODE_H


#include "StatementNode.h"
#include "TypeNode.h"

class VarNode: public StatementNode {
public:
    VarNode(const std::string &name, const TypeNode *type, ExprNode *const expr);

    void print(Printer &printer) const override;

    void codegen(CodegenContext &context) override;

private:
    const std::string name;
    const TypeNode *type;
    ExprNode * const expr;
};


#endif //BP_VARNODE_H
