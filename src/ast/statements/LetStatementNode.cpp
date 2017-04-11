//
// Created by miroslav on 3/28/17.
//

#include <ast/expressions/NameExprNode.h>
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
    CodegenValue* value = expr->codegen(context, name);
    context.addValue(name,value);
}

LetStatementNode::~LetStatementNode() {
    delete expr;
}
