//
// Created by miroslav on 3/31/17.
//

#include "CallExprNode.h"

CallExprNode::CallExprNode(ExprNode *expr) : expr(expr) {}

std::string CallExprNode::toString() const {
    return expr->toString() + "()";
}

CodegenValue * CallExprNode::codegen(CodegenContext &context, const llvm::Twine &Name) {
    CodegenValue *callee = expr->codegen(context, Name + ".callee");
    return callee->doCall(context, Name);
}
