//
// Created by miroslav on 4/4/17.
//

#ifndef BP_ARRAYTYPE_H
#define BP_ARRAYTYPE_H


#include <CodegenContext.h>

struct ArrayType: CodegenType {
    ArrayType(llvm::Type *const storeType, llvm::Type *const referenceObjectType, CodegenType *elementType, const std::string &region);

    CodegenValue *getElement(CodegenContext &ctx, CodegenValue *value, CodegenValue *index) override;

    CodegenValue *getChild(CodegenContext &ctx, CodegenValue *value, std::string name) override;

    CodegenType *elementType;
    llvm::Type *referenceObjectType;
    std::string region;

    static ArrayType* get(CodegenContext &ctx, CodegenType *elementType, const std::string &region);

    bool equals(CodegenType *pType, const std::map<std::string,std::string> &regionsRemap) override;

    std::string toString() const override;

    CodegenType *withRemapRegions(CodegenContext &context, const std::map<std::string, std::string> &map) override;
};


#endif //BP_ARRAYTYPE_H
