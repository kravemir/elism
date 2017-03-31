//
// Created by miroslav on 3/28/17.
//

#include "BinaryOperationExprNode.h"

using namespace llvm;

BinaryOperationExprNode::BinaryOperationExprNode(const char OP, ExprNode *const e1, ExprNode *const e2) : OP(OP), e1(e1), e2(e2) {}

std::string BinaryOperationExprNode::toString() const {
    return "(" + e1->toString() + " " + OP + " " + e2->toString() + ")";
}

CodegenValue * BinaryOperationExprNode::codegen(CodegenContext &context) {
    Value* v1 = e1->codegen(context)->value;
    Value* v2 = e2->codegen(context)->value;
    // TODO: types

    Value *res = nullptr;
    switch(OP) {
        case '+':
            res = context.builder.CreateAdd(v1, v2); break;
        case '-':
            res = context.builder.CreateSub(v1, v2); break;
        case '*':
            res = context.builder.CreateMul(v1, v2); break;
        case '/':
            res = context.builder.CreateSDiv(v1, v2); break;
        default:
            assert(0);
    }
    return new CodegenValue(res);
}
