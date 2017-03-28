/**
 * @author Miroslav Kravec
 */

#include "ReturnNode.h"

#include <cassert>

using namespace llvm;

ReturnNode::ReturnNode(ExprNode *expr) : expr(expr) {
    assert(expr != nullptr);
}

void ReturnNode::print(Printer &printer) const {
    printer.println("return " + expr->toString() + ";");
}

ReturnNode::~ReturnNode() {
    delete expr;
}

void ReturnNode::codegen(CodegenContext &context) {
    context.codegenReturn(expr->codegen(context));
}
