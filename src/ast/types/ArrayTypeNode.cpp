/**
 * @author Miroslav Kravec
 */

#include <llvm/IR/Type.h>
#include <CodegenContext.h>
#include "ArrayTypeNode.h"

#include <codegen/ArrayType.h>

ArrayTypeNode::ArrayTypeNode(TypeNode *baseType) : baseType(baseType) {}

std::string ArrayTypeNode::toString() {
    return baseType->toString() + "[]";
}

ArrayTypeNode::~ArrayTypeNode() {
    delete baseType;
}

CodegenType *ArrayTypeNode::codegen(CodegenContext &context) {
    return ArrayType::get(context, baseType->codegen(context));
}
