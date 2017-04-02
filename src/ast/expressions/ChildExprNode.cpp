//
// Created by miroslav on 4/2/17.
//

#include "ChildExprNode.h"

ChildExprNode::ChildExprNode(ExprNode *base, const std::string &childName) : base(base), childName(childName) {}

std::string ChildExprNode::toString() const {
    return base->toString() + "." + childName;
}

CodegenValue *ChildExprNode::codegen(CodegenContext &context) {
    CodegenValue *base = this->base->codegen(context);
    return base->type->getChild(context,base,childName);
}
