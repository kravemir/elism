//
// Created by miroslav on 3/28/17.
//

#ifndef BP_NAMEEXPR_H
#define BP_NAMEEXPR_H


#include "ExprNode.h"

class NameExpr: public ExprNode {
public:
    NameExpr(const std::string &name);

    std::string toString() const override;

    llvm::Value *codegen(CodegenContext &context) override;

private:
    std::string name;
};


#endif //BP_NAMEEXPR_H
