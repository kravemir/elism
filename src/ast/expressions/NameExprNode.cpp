//
// Created by miroslav on 3/28/17.
//

#include "NameExprNode.h"

NameExprNode::NameExprNode(const std::string &name) : name(name) {}

std::string NameExprNode::toString() const {
    return name;
}

CodegenValue * NameExprNode::codegen(CodegenContext &context) {
    CodegenValue *value = context.getValue(name);
    assert(value);
    return value;
}
