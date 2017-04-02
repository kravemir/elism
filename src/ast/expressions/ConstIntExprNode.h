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

    CodegenValue * codegen(CodegenContext &context, const llvm::Twine &Name = "") override;

private:
    long value;
};


#endif //BP_INTVALNODE_H
