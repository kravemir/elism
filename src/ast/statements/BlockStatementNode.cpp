//
// Created by miroslav on 4/6/17.
//

#include "BlockStatementNode.h"

BlockStatementNode::BlockStatementNode(const std::vector<StatementNode *> &statements) : statements(statements) {}

void BlockStatementNode::print(Printer &printer) const {
    printer.println("TODO");
}

void BlockStatementNode::codegen(CodegenContext &context) {
    for(StatementNode* node : statements)
        node->codegen(context);
}

BlockStatementNode::~BlockStatementNode() {
    for(StatementNode *s : statements)
        delete s;
}
