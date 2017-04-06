//
// Created by miroslav on 4/6/17.
//

#include "ExprStatementNode.h"

ExprStatementNode::ExprStatementNode(ExprNode *expr) : expr(expr) {}

void ExprStatementNode::print(Printer &printer) const {
    printer.println("TODO");
}

void ExprStatementNode::codegen(CodegenContext &context) {
    expr->codegen(context);
}
