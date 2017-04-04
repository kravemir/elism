//
// Created by miroslav on 4/4/17.
//

#include "ArrayExprNode.h"

#include <codegen/ArrayType.h>

using namespace llvm;

ArrayLiteralExprNode::ArrayLiteralExprNode(const std::vector<ExprNode *> &values) : values(values) {}

std::string ArrayLiteralExprNode::toString() const {
    return "TODO";
}

CodegenValue *ArrayLiteralExprNode::codegen(CodegenContext &context, const llvm::Twine &Name) {
    std::vector<CodegenValue*> valuesValues;
    for(ExprNode* e : values) {
        valuesValues.push_back(e->codegen(context));
    }

    ::ArrayType *type = ::ArrayType::get(context,valuesValues[0]->type);

    Type* ITy = Type::getInt64PtrTy(context.llvmContext);
    Constant* AllocSize = ConstantExpr::getSizeOf(valuesValues[0]->type->storeType);
    AllocSize = ConstantExpr::getTruncOrBitCast(AllocSize, ITy);
    Instruction* Malloc = CallInst::CreateMalloc(
            context.builder.GetInsertBlock(),
            ITy,
            valuesValues[0]->type->storeType,
            AllocSize,
            ConstantInt::get(context.llvmContext, APInt(32, valuesValues.size(), false)),
            nullptr, ""
    );
    context.builder.Insert(Malloc);

    AllocSize = ConstantExpr::getSizeOf(type->referenceObjectType);
    AllocSize = ConstantExpr::getTruncOrBitCast(AllocSize, ITy);
    Instruction* arrayObjectMalloc = CallInst::CreateMalloc(
            context.builder.GetInsertBlock(),
            ITy,
            type->referenceObjectType,
            AllocSize,
            nullptr, nullptr, ""
    );
    context.builder.Insert(arrayObjectMalloc);

    context.builder.CreateStore(
            ConstantInt::get(Type::getInt64Ty(context.llvmContext), valuesValues.size()),
            context.builder.CreateGEP(
                    arrayObjectMalloc,
                    {
                            ConstantInt::get(Type::getInt64Ty(context.llvmContext), 0),
                            ConstantInt::get(Type::getInt32Ty(context.llvmContext), 0)
                    },
                    "size"
            )
    );
    context.builder.CreateStore(
            Malloc,
            context.builder.CreateGEP(
                    arrayObjectMalloc,
                    {
                            ConstantInt::get(Type::getInt64Ty(context.llvmContext), 0),
                            ConstantInt::get(Type::getInt32Ty(context.llvmContext), 1)
                    },
                    "array"
            )
    );
    for(int i = 0; i < valuesValues.size(); i++) {
        context.builder.CreateStore(
                valuesValues[i]->value,
                context.builder.CreateGEP(
                        Malloc,
                        {
                                ConstantInt::get(Type::getInt64Ty(context.llvmContext), i)
                        },
                        "array." + std::to_string(i)
                )
        );
    }

    return new CodegenValue(type, arrayObjectMalloc);
}

ArrayInitializerExprNode::ArrayInitializerExprNode(ExprNode *value, int count) : value(value), count(count) {}

std::string ArrayInitializerExprNode::toString() const {
    return "TODO";
}

CodegenValue *ArrayInitializerExprNode::codegen(CodegenContext &context, const llvm::Twine &Name) {
    CodegenValue *value = this->value->codegen(context, Name + ".value");

    ::ArrayType *type = ::ArrayType::get(context,value->type);

    Type* ITy = Type::getInt64PtrTy(context.llvmContext);
    Constant* AllocSize = ConstantExpr::getSizeOf(value->type->storeType);
    AllocSize = ConstantExpr::getTruncOrBitCast(AllocSize, ITy);
    Instruction* Malloc = CallInst::CreateMalloc(
            context.builder.GetInsertBlock(),
            ITy,
            value->type->storeType,
            AllocSize,
            ConstantInt::get(context.llvmContext, APInt(32, count, false)),
            nullptr, ""
    );
    context.builder.Insert(Malloc);

    AllocSize = ConstantExpr::getSizeOf(type->referenceObjectType);
    AllocSize = ConstantExpr::getTruncOrBitCast(AllocSize, ITy);
    Instruction* arrayObjectMalloc = CallInst::CreateMalloc(
            context.builder.GetInsertBlock(),
            ITy,
            type->referenceObjectType,
            AllocSize,
            nullptr, nullptr, ""
    );
    context.builder.Insert(arrayObjectMalloc);

    context.builder.CreateStore(
            ConstantInt::get(Type::getInt64Ty(context.llvmContext), count),
            context.builder.CreateGEP(
                    arrayObjectMalloc,
                    {
                            ConstantInt::get(Type::getInt64Ty(context.llvmContext), 0),
                            ConstantInt::get(Type::getInt32Ty(context.llvmContext), 0)
                    },
                    "size"
            )
    );
    context.builder.CreateStore(
            Malloc,
            context.builder.CreateGEP(
                    arrayObjectMalloc,
                    {
                            ConstantInt::get(Type::getInt64Ty(context.llvmContext), 0),
                            ConstantInt::get(Type::getInt32Ty(context.llvmContext), 1)
                    },
                    "array"
            )
    );
    for(int i = 0; i < count; i++) {
        context.builder.CreateStore(
                value->value,
                context.builder.CreateGEP(
                        Malloc,
                        {
                                ConstantInt::get(Type::getInt64Ty(context.llvmContext), i)
                        },
                        "array." + std::to_string(i)
                )
        );
    }

    return new CodegenValue(type, arrayObjectMalloc);
}

ArrayElementExprNode::ArrayElementExprNode(ExprNode *value, ExprNode *idx) : value(value), idx(idx) {}

std::string ArrayElementExprNode::toString() const {
    return "TODO";
}

CodegenValue *ArrayElementExprNode::codegen(CodegenContext &context, const Twine &Name) {
    CodegenValue *v = value->codegen(context);
    CodegenValue *i = idx->codegen(context);
    return v->type->getElement(context,v,i);
}
