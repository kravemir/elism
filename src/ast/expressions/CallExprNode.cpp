//
// Created by miroslav on 3/31/17.
//

#include "CallExprNode.h"

CallExprNode::CallExprNode(ExprNode *expr, std::vector<ExprNode*> args) : expr(expr), args(args) {}

std::string CallExprNode::toString() const {
    return expr->toString() + "()";
}

CodegenValue * CallExprNode::codegen(CodegenContext &context, const llvm::Twine &Name) {
    CodegenValue *callee = expr->codegen(context, Name + ".callee");
    std::vector<CodegenValue*> argsv;
    for(ExprNode* e : args) {
        argsv.push_back(e->codegen(context));
    }
    return callee->doCall(context, argsv, Name);
}

CallExprNode::~CallExprNode() {
    delete expr;
}
