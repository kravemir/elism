/**
 * @author Miroslav Kravec
 */

#include "IntValNode.h"

using namespace llvm;

IntValNode::IntValNode(long value) : value(value) {}

std::string IntValNode::toString() const {
    return std::to_string(value);
}

llvm::Value *IntValNode::codegen(CodegenContext &context) {
    return ConstantInt::get(context.llvmContext, APInt((unsigned) 32, 42));
}
