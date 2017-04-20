//
// Created by miroslav on 4/4/17.
//

#include "ArrayType.h"
#include "IntType.h"

using namespace llvm;

::ArrayType::ArrayType(Type *const storeType, llvm::Type *const referenceObjectType, CodegenType *elementType, const std::string &region)
        : CodegenType(storeType),
          referenceObjectType(referenceObjectType),
          elementType(elementType),
          region(region)
{}

::ArrayType *::ArrayType::get(CodegenContext &ctx, CodegenType *base, const std::string &region) {
    StructType *lType = StructType::get(ctx.llvmContext, {
            Type::getInt64Ty(ctx.llvmContext),
            PointerType::get(base->storeType, 0)
    });

    return new ArrayType(PointerType::get(lType,0), lType, base, region);
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

CodegenValue *::ArrayType::getChild(CodegenContext &ctx, CodegenValue *value, std::string name) {
    if("length" == name) {
        return new CodegenValue(
                IntType::get64(ctx),
                ctx.builder.CreateLoad(ctx.builder.CreateGEP(
                        value->value,
                        {
                                ConstantInt::get(Type::getInt64Ty(ctx.llvmContext), 0),
                                ConstantInt::get(Type::getInt32Ty(ctx.llvmContext), 0)
                        },
                        "length"
                ))
        );
    }
    return CodegenType::getChild(ctx, value, name);
}


bool ::ArrayType::equals(CodegenType *pType, const std::map<std::string,std::string> &regionsRemap) {
    ::ArrayType *at = dynamic_cast<::ArrayType*>(pType);
    if(!at)
        return false;
    if(regionsRemap.size() > 0) {
        auto it = regionsRemap.find(this->region);
        // TODO check if found, what next if not found???
        if (it->second != at->region) {
            return false;
        }
    } else {
        if (this->region != at->region) {
            return false;
        }
    }
    return this->elementType->equals(at->elementType,regionsRemap);
}

std::string (::ArrayType::toString)() const {
    return elementType->toString() + "[] @" + region;
}

CodegenType *::ArrayType::withRemapRegions(CodegenContext &context, const std::map<std::string, std::string> &map) {
    return ::ArrayType::get(context,elementType->withRemapRegions(context,map),map.find(this->region)->second);
}
