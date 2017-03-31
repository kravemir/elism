//
// Created by miroslav on 3/28/17.
//

#include "AssignStatementNode.h"

using namespace llvm;

AssignStatementNode::AssignStatementNode(const std::string &name, ExprNode *const expr) : name(name), expr(expr) {}

void AssignStatementNode::print(Printer &printer) const {
    printer.println("TODO");
}

void AssignStatementNode::codegen(CodegenContext &context) {
    Value* value = expr->codegen(context);
    context.storeValue(name,value);
}
