//
// Created by miroslav on 4/4/17.
//

#ifndef BP_ARRAYEXPRNODE_H
#define BP_ARRAYEXPRNODE_H

#include <ast/ExprNode.h>

class ArrayLiteralExprNode: public ExprNode {
public:
    ArrayLiteralExprNode(const std::vector<ExprNode *> &values);

    std::string toString() const override;

    CodegenValue *codegen(CodegenContext &context, const llvm::Twine &Name) override;

private:
    std::vector<ExprNode*> values;
};

class ArrayInitializerExprNode: public ExprNode {
public:
    ArrayInitializerExprNode(ExprNode *value, ExprNode *count);

    std::string toString() const override;

    CodegenValue *codegen(CodegenContext &context, const llvm::Twine &Name) override;

private:
    ExprNode *value;
    ExprNode *count;
};

class ArrayElementExprNode: public ExprNode {
public:
    ArrayElementExprNode(ExprNode *value, ExprNode *idx);

    std::string toString() const override;

    CodegenValue *codegen(CodegenContext &context, const llvm::Twine &Name) override;

private:
    ExprNode *value;
    ExprNode *idx;
};

#endif //BP_ARRAYEXPRNODE_H
