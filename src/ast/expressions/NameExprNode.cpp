//
// Created by miroslav on 3/28/17.
//

#include "NameExprNode.h"

NameExprNode::NameExprNode(const std::string &name) : name(name) {}

std::string NameExprNode::toString() const {
    return name;
}

CodegenValue * NameExprNode::codegen(CodegenContext &context, const llvm::Twine &Name) {
    CodegenValue *value = context.getValue(name); // todo: Name
    if(!value) {
        fprintf(stderr, "Name %s not resolved\n", name.c_str());
        assert(0);
    }
    return value;
}
