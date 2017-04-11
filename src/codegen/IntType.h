//
// Created by miroslav on 4/4/17.
//

#ifndef BP_INTTYPE_H
#define BP_INTTYPE_H

#include <CodegenContext.h>

struct IntType: CodegenType {
    IntType(int bits, llvm::Type *const storeType) : CodegenType(storeType), bits(bits) {}

    const int bits;

    static IntType* get32(CodegenContext &ctx);
    static IntType* get64(CodegenContext &ctx);

    CodegenValue *const getDefault(CodegenContext &context) override;
};


#endif //BP_INTTYPE_H
