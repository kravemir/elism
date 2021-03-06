//
// Created by miroslav on 3/28/17.
//

#include <codegen/IntType.h>
#include "BinaryOperationExprNode.h"

#include "parser.h"

using namespace llvm;

BinaryOperationExprNode::BinaryOperationExprNode(const char OP, ExprNode *const e1, ExprNode *const e2) : OP(OP), e1(e1), e2(e2) {}

std::string BinaryOperationExprNode::toString() const {
    return "(" + e1->toString() + " " + OP + " " + e2->toString() + ")";
}

CodegenValue * BinaryOperationExprNode::codegen(CodegenContext &context, const llvm::Twine &Name) {
    CodegenValue* cv1 = e1->codegen(context);
    CodegenValue* cv2 = e2->codegen(context);
    Value *v1 = cv1->value;
    Value *v2 = cv2->value;
    // TODO: types

    IntType *intType1 = dynamic_cast<IntType*>(cv1->type);
    IntType *intType2 = dynamic_cast<IntType*>(cv2->type);

    if(intType1 && intType2) {
        Value *res = nullptr;
        switch (OP) {
            case '+':
                res = context.builder.CreateAdd(v1, v2, Name);
                break;
            case '-':
                res = context.builder.CreateSub(v1, v2, Name);
                break;
            case '*':
                res = context.builder.CreateMul(v1, v2, Name);
                break;
            case '/':
                res = context.builder.CreateSDiv(v1, v2, Name);
                break;
            case TOKEN_EQUALS:
                res = context.builder.CreateICmpEQ(v1, v2, Name);
                break;
            case TOKEN_LT:
                res = context.builder.CreateICmpSLT(v1, v2, Name);
                break;
            case TOKEN_GT:
                res = context.builder.CreateICmpSGT(v1, v2, Name);
                break;
            case TOKEN_LE:
                res = context.builder.CreateICmpSLE(v1, v2, Name);
                break;
            case TOKEN_GE:
                res = context.builder.CreateICmpSGE(v1, v2, Name);
                break;
            default:
                assert(0);
        }
        //printf("type: %s\n",cv1->type->toString().c_str());
        return new CodegenValue(cv1->type, res);
    }

    assert(0 && "No suitable operator found");
    return nullptr;
}

BinaryOperationExprNode::~BinaryOperationExprNode() {
    delete e1;
    delete e2;
}
