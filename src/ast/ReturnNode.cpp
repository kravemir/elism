/**
 * @author Miroslav Kravec
 */

#include "ReturnNode.h"

#include <cassert>

ReturnNode::ReturnNode(ExprNode *expr) : expr(expr) {
    assert(expr != nullptr);
}

void ReturnNode::print(Printer &printer) const {
    printer.println("return " + expr->toString() + ";");
}

ReturnNode::~ReturnNode() {
    delete expr;
}
