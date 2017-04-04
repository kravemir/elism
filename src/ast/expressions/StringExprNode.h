//
// Created by miroslav on 4/4/17.
//

#ifndef BP_STRINGEXPRNODE_H
#define BP_STRINGEXPRNODE_H


#include <ast/ExprNode.h>

class StringExprNode: public ExprNode {
public:
    StringExprNode(const std::string &value);

    std::string toString() const override;

    CodegenValue *codegen(CodegenContext &context, const llvm::Twine &Name) override;

private:
    std::string value;
};


#endif //BP_STRINGEXPRNODE_H
