//
// Created by miroslav on 3/28/17.
//

#include "LetNode.h"

using namespace llvm;

LetNode::LetNode(const std::string &name, ExprNode *const expr) : name(name), expr(expr) {}

void LetNode::print(Printer &printer) const {
    printer.print("let ");
    printer.print(name);
    printer.print(" = ");
    printer.print(expr->toString());
    printer.println(";");
}

void LetNode::codegen(CodegenContext &context) {
    Value *val = expr->codegen(context);
    context.addValue(name,val);
}
