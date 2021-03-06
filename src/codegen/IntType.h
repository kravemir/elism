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

    bool equals(CodegenType *pType, const std::map<std::string,std::string> &regionsRemap) override;

    std::string toString() const override;

    CodegenType *withRemapRegions(CodegenContext &context, const std::map<std::string, std::string> &map) override;

};


#endif //BP_INTTYPE_H
