/**
 * @author Miroslav Kravec
 */

#include "ConstIntExprNode.h"

using namespace llvm;

ConstIntExprNode::ConstIntExprNode(long value) : value(value) {}

std::string ConstIntExprNode::toString() const {
    return std::to_string(value);
}

CodegenValue * ConstIntExprNode::codegen(CodegenContext &context) {
    return new CodegenValue(ConstantInt::get(context.llvmContext, APInt((unsigned) 32, value)));
}
