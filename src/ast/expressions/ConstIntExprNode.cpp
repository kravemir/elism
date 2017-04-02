/**
 * @author Miroslav Kravec
 */

#include "ConstIntExprNode.h"

using namespace llvm;

ConstIntExprNode::ConstIntExprNode(long value) : value(value) {}

std::string ConstIntExprNode::toString() const {
    return std::to_string(value);
}

struct IntType: CodegenType {
    IntType(Type *const storeType) : CodegenType(storeType) {}

};

CodegenValue * ConstIntExprNode::codegen(CodegenContext &context, const llvm::Twine &Name) {
    return new CodegenValue(
            new IntType(Type::getInt64Ty(context.llvmContext)),
            ConstantInt::get(context.llvmContext, APInt((unsigned) 32, (uint64_t) value))
    );
}
