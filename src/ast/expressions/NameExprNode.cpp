//
// Created by miroslav on 3/28/17.
//

#include "NameExprNode.h"

NameExprNode::NameExprNode(const std::string &name) : name(name) {}

std::string NameExprNode::toString() const {
    return name;
}

llvm::Value *NameExprNode::codegen(CodegenContext &context) {
    return context.getValue(name);
}
