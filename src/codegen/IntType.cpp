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
