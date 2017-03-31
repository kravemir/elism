/**
 * @author Miroslav Kravec
 */

#include "ReturnStatementNode.h"

#include <cassert>

using namespace llvm;

ReturnStatementNode::ReturnStatementNode(ExprNode *expr) : expr(expr) {
    assert(expr != nullptr);
}

void ReturnStatementNode::print(Printer &printer) const {
    printer.println("return " + expr->toString() + ";");
}

ReturnStatementNode::~ReturnStatementNode() {
    delete expr;
}

void ReturnStatementNode::codegen(CodegenContext &context) {
    context.codegenReturn(expr->codegen(context));
}
