//
// Created by miroslav on 3/28/17.
//

#ifndef BP_BIOPEXPR_H
#define BP_BIOPEXPR_H


#include "ast/ExprNode.h"

class BinaryOperationExprNode: public ExprNode {
public:
    BinaryOperationExprNode(const char OP, ExprNode *const e1, ExprNode *const e2);

    std::string toString() const override;

    CodegenValue * codegen(CodegenContext &context) override;

private:
    const char OP;
    ExprNode * const e1, * const e2;
};


#endif //BP_BIOPEXPR_H
