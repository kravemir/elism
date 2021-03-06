//
// Created by miroslav on 4/2/17.
//

#ifndef BP_CHILDEXPRNODE_H
#define BP_CHILDEXPRNODE_H


#include <ast/ExprNode.h>

class ChildExprNode: public ExprNode {
public:
    ChildExprNode(ExprNode *base, const std::string &childName);

    ~ChildExprNode() override;

private:
    std::string toString() const override;

    CodegenValue *codegen(CodegenContext &context, const llvm::Twine &Name = "") override;

private:
    ExprNode *base;
    const std::string childName;
};


#endif //BP_CHILDEXPRNODE_H
