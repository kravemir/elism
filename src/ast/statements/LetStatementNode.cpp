//
// Created by miroslav on 3/28/17.
//

#include "LetStatementNode.h"

using namespace llvm;

LetStatementNode::LetStatementNode(const std::string &name, ExprNode *const expr) : name(name), expr(expr) {}

void LetStatementNode::print(Printer &printer) const {
    printer.print("let ");
    printer.print(name);
    printer.print(" = ");
    printer.print(expr->toString());
    printer.println(";");
}

void LetStatementNode::codegen(CodegenContext &context) {
    CodegenValue *val = expr->codegen(context);
    context.addValue(name,val);
}
