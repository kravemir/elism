/**
 * @author Miroslav Kravec
 */

#ifndef BP_INTVALNODE_H
#define BP_INTVALNODE_H

#include "ast/ExprNode.h"

class ConstIntExprNode: public ExprNode {
public:
    ConstIntExprNode(long value);

    std::string toString() const override;

    llvm::Value *codegen(CodegenContext &context) override;

private:
    long value;
};


#endif //BP_INTVALNODE_H
