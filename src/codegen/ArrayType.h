//
// Created by miroslav on 4/4/17.
//

#ifndef BP_ARRAYTYPE_H
#define BP_ARRAYTYPE_H


#include <CodegenContext.h>

struct ArrayType: CodegenType {
    ArrayType(llvm::Type *const storeType, llvm::Type *const referenceObjectType, CodegenType *elementType);

    CodegenValue *getElement(CodegenContext &ctx, CodegenValue *value, CodegenValue *index) override;

    CodegenValue *getChild(CodegenContext &ctx, CodegenValue *value, std::string name) override;

    CodegenType *elementType;
    llvm::Type *referenceObjectType;

    static ArrayType* get(CodegenContext &ctx, CodegenType *elementType);

    bool equals(CodegenType *pType) override;
};


#endif //BP_ARRAYTYPE_H
