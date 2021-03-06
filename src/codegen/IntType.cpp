//
// Created by miroslav on 4/4/17.
//

#include "IntType.h"

IntType *IntType::get32(CodegenContext &ctx) {
    return new IntType(32, llvm::Type::getInt64Ty(ctx.llvmContext));
}

IntType *IntType::get64(CodegenContext &ctx) {
    return new IntType(64, llvm::Type::getInt64Ty(ctx.llvmContext));
}

CodegenValue *const IntType::getDefault(CodegenContext &context) {
    return new CodegenValue(this, llvm::ConstantInt::get(this->storeType, 0, false));
}

bool IntType::equals(CodegenType *pType, const std::map<std::string,std::string> &regionsRemap) {
    IntType *it = dynamic_cast<IntType*>(pType);
    if(!it)
        return false;
    return this->bits == it->bits;
}

std::string IntType::toString() const {
    return "i" + std::to_string(bits);
}

CodegenType *IntType::withRemapRegions(CodegenContext &context, const std::map<std::string, std::string> &map) {
    return this;
}
