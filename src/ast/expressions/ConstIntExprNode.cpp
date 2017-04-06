/**
 * @author Miroslav Kravec
 */

#include <codegen/IntType.h>
#include "ConstIntExprNode.h"

using namespace llvm;

ConstIntExprNode::ConstIntExprNode(long value) : value(value) {}

std::string ConstIntExprNode::toString() const {
    return std::to_string(value);
}

CodegenValue * ConstIntExprNode::codegen(CodegenContext &context, const llvm::Twine &Name) {
    // TODO: support multiple sizes
    return new CodegenValue(
            IntType::get64(context),
            ConstantInt::get(context.llvmContext, APInt((unsigned) 64, (uint64_t) value))
    );
}
