//
// Created by miroslav on 3/28/17.
//

#ifndef BP_NAMEEXPR_H
#define BP_NAMEEXPR_H


#include "ast/ExprNode.h"

class NameExprNode: public ExprNode {
public:
    NameExprNode(const std::string &name);

    std::string toString() const override;

    CodegenValue * codegen(CodegenContext &context, const llvm::Twine &Name = "") override;

private:
    std::string name;
};


#endif //BP_NAMEEXPR_H
