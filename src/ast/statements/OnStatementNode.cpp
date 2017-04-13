//
// Created by miroslav on 4/13/17.
//

#include "OnStatementNode.h"

OnStatementNode::OnStatementNode(const std::string &name, ExprNode *expr, StatementNode *stmt) : name(name), expr(expr),
                                                                                                 stmt(stmt) {}

void OnStatementNode::print(Printer &printer) const {
    // TODO
}

void OnStatementNode::codegen(CodegenContext &context) {
    stmt->codegen(context);
}
