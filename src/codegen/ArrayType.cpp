//
// Created by miroslav on 4/4/17.
//

#include "ArrayType.h"

using namespace llvm;

::ArrayType::ArrayType(Type *const storeType, llvm::Type *const referenceObjectType, CodegenType *elementType)
        : CodegenType(storeType),
          referenceObjectType(referenceObjectType),
          elementType(elementType)
{}

::ArrayType *::ArrayType::get(CodegenContext &ctx, CodegenType *base) {
    StructType *lType = StructType::get(ctx.llvmContext, {
            Type::getInt64Ty(ctx.llvmContext),
            PointerType::get(base->storeType, 0)
    });

    return new ArrayType(PointerType::get(lType,0), lType, base);
}

CodegenValue *::ArrayType::getElement(CodegenContext &ctx, CodegenValue *value, CodegenValue *index) {
    Value *arrayPtr = ctx.builder.CreateLoad(
            ctx.builder.CreateGEP(value->value,{
                    ConstantInt::get(Type::getInt64Ty(ctx.llvmContext), 0),
                    ConstantInt::get(Type::getInt32Ty(ctx.llvmContext), 1)
            }),
            "array.ptr"
    );
    Value *elementPtr = ctx.builder.CreateGEP(arrayPtr,{index->value}, "element.ptr");
    Value* val = ctx.builder.CreateLoad(elementPtr, "element");
    return new CodegenValue(elementType, val, elementPtr);
}

