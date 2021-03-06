//
// Created by miroslav on 3/31/17.
//

#ifndef BP_CALLEXPRNODE_H
#define BP_CALLEXPRNODE_H


#include <ast/ExprNode.h>

class CallExprNode: public ExprNode {
public:
    CallExprNode(ExprNode *expr, const std::vector<std::string> &regions, std::vector<ExprNode*> args);

    ~CallExprNode() override;

    std::string toString() const override;

    CodegenValue * codegen(CodegenContext &context, const llvm::Twine &Name = "") override;

private:
    std::vector<std::string> regions;
    ExprNode *expr;
    std::vector<ExprNode*> args;
};


#endif //BP_CALLEXPRNODE_H
