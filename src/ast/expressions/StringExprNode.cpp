//
// Created by miroslav on 4/4/17.
//

#include "StringExprNode.h"

using namespace llvm;

StringExprNode::StringExprNode(const std::string &value) : value(value) {}

std::string StringExprNode::toString() const {
    return "\"" + value + "\"";
}

struct StringType: CodegenType {
    StringType(Type *const storeType) : CodegenType(storeType) {}

    bool equals(CodegenType *pType, const std::map<std::string,std::string> &regionsRemap) override {
        // TODO: check region
        return dynamic_cast<StringType*>(pType) != nullptr;
    }

    std::string toString() const override {
        return "str";
    }

    CodegenType *withRemapRegions(CodegenContext &context, const std::map<std::string, std::string> &map) override {
        assert(0);
        return nullptr;
    }
};

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

    return new CodegenValue(new StringType(ltype), var);
}
