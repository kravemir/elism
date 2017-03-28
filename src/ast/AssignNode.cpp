//
// Created by miroslav on 3/28/17.
//

#include "AssignNode.h"

using namespace llvm;

AssignNode::AssignNode(const std::string &name, ExprNode *const expr) : name(name), expr(expr) {}

void AssignNode::print(Printer &printer) const {
    printer.println("TODO");
}

void AssignNode::codegen(CodegenContext &context) {
    Value* value = expr->codegen(context);
    context.storeValue(name,value);
}
