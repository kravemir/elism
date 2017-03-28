//
// Created by miroslav on 3/28/17.
//

#include "BiopExpr.h"

using namespace llvm;

BiopExpr::BiopExpr(const char OP, ExprNode *const e1, ExprNode *const e2) : OP(OP), e1(e1), e2(e2) {}

std::string BiopExpr::toString() const {
    return "(" + e1->toString() + " " + OP + " " + e2->toString() + ")";
}

llvm::Value *BiopExpr::codegen(CodegenContext &context) {
    Value* v1 = e1->codegen(context);
    Value* v2 = e2->codegen(context);
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
    return res;
}
