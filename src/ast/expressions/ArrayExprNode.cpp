//
// Created by miroslav on 4/4/17.
//

#include "ArrayExprNode.h"

#include <codegen/ArrayType.h>
#include <llvm/IR/Module.h>

using namespace llvm;

ArrayLiteralExprNode::ArrayLiteralExprNode(const std::vector<ExprNode *> &values) : values(values) {}

std::string ArrayLiteralExprNode::toString() const {
    return "TODO";
}

void createArrayAlloc(CodegenContext &context, Value* &arrayObjectMalloc, Value* &Malloc, ::ArrayType *arrayType, Value *size) {

    Type* ITy = Type::getInt64PtrTy(context.llvmContext);
    Constant* AllocSize = ConstantExpr::getSizeOf(arrayType->elementType->storeType);
    AllocSize = ConstantExpr::getTruncOrBitCast(AllocSize, ITy);
    Malloc = context.createAlloc(
            arrayType->elementType->storeType,
            AllocSize,
            size
    );

    AllocSize = ConstantExpr::getSizeOf(arrayType->referenceObjectType);
    AllocSize = ConstantExpr::getTruncOrBitCast(AllocSize, ITy);

    arrayObjectMalloc = context.createAlloc(
            arrayType->referenceObjectType,
            AllocSize,
            nullptr
    );

    context.builder.CreateStore(
            size,
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
}

CodegenValue *ArrayLiteralExprNode::codegen(CodegenContext &context, const llvm::Twine &Name) {
    std::vector<CodegenValue*> valuesValues;
    for(ExprNode* e : values) {
        valuesValues.push_back(e->codegen(context));
    }

    ::ArrayType *type = ::ArrayType::get(context,valuesValues[0]->type);

    // create array object: allocation, fill object size and reference
    Value* arrayObjectMalloc, *Malloc;
    createArrayAlloc(context,arrayObjectMalloc,Malloc,type,ConstantInt::get(Type::getInt64Ty(context.llvmContext), valuesValues.size()));

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

ArrayInitializerExprNode::ArrayInitializerExprNode(ExprNode *value, ExprNode *count) : value(value), count(count) {}

std::string ArrayInitializerExprNode::toString() const {
    return "TODO";
}

CodegenValue *ArrayInitializerExprNode::codegen(CodegenContext &context, const llvm::Twine &Name) {
    CodegenValue *value = this->value->codegen(context, Name + ".value");
    CodegenValue *count = this->count->codegen(context, Name + ".count");

    ::ArrayType *type = ::ArrayType::get(context,value->type);

    // create array object: allocation, fill object size and reference
    Value* arrayObjectMalloc, *Malloc;
    createArrayAlloc(context,arrayObjectMalloc,Malloc,type,count->value);

    Function *TheFunction = context.builder.GetInsertBlock()->getParent();

    // create blocks
    BasicBlock *ConditionBB = BasicBlock::Create(context.llvmContext, "array.condition");
    BasicBlock *ContentBB = BasicBlock::Create(context.llvmContext, "array.content");
    BasicBlock *ContinueBB = BasicBlock::Create(context.llvmContext, "array.continue");

    AllocaInst *allocaInst = context.createAlloca("array.i", new CodegenValue(count->type,ConstantInt::get(count->value->getType(), 0)));
    context.builder.CreateBr(ConditionBB);

    TheFunction->getBasicBlockList().push_back(ConditionBB);
    context.builder.SetInsertPoint(ConditionBB);
    llvm::Value *cond = context.builder.CreateICmpSLT(context.builder.CreateLoad(allocaInst),count->value);
    context.builder.CreateCondBr(cond, ContentBB, ContinueBB);

    TheFunction->getBasicBlockList().push_back(ContentBB);
    context.builder.SetInsertPoint(ContentBB);
    context.builder.CreateStore(
            value->value,
            context.builder.CreateGEP(
                    Malloc,
                    {
                            context.builder.CreateLoad(allocaInst)
                    },
                    "array.i"
            )
    );
    context.builder.CreateStore(
            context.builder.CreateAdd(context.builder.CreateLoad(allocaInst), ConstantInt::get(count->value->getType(), 1)),
            allocaInst
    );
    context.builder.CreateBr(ConditionBB);

    TheFunction->getBasicBlockList().push_back(ContinueBB);
    context.builder.SetInsertPoint(ContinueBB);

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
