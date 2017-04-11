//
// Created by miroslav on 4/4/17.
//

#include "StringExprNode.h"

using namespace llvm;

StringExprNode::StringExprNode(const std::string &value) : value(value) {}

std::string StringExprNode::toString() const {
    return "\"" + value + "\"";
}

CodegenValue *StringExprNode::codegen(CodegenContext &context, const llvm::Twine &Name) {
    Constant *format_const = ConstantDataArray::getString(context.llvmContext, value.c_str() );
    Type* ltype = ArrayType::get(IntegerType::get(context.llvmContext, 8), value.size() + 1);
    GlobalVariable *var = new GlobalVariable(
            *context.module,
            ltype,
            true,
            llvm::GlobalValue::PrivateLinkage,
            format_const,
            ".str"
    );

    return new CodegenValue(new CodegenType(ltype), var);
}
