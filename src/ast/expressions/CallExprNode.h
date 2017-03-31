//
// Created by miroslav on 3/31/17.
//

#ifndef BP_CALLEXPRNODE_H
#define BP_CALLEXPRNODE_H


#include <ast/ExprNode.h>

class CallExprNode: public ExprNode {
public:
    CallExprNode(ExprNode *expr);

    std::string toString() const override;

    CodegenValue * codegen(CodegenContext &context) override;

private:
    ExprNode *expr;
};


#endif //BP_CALLEXPRNODE_H
