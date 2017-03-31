/**
 * @author Miroslav Kravec
 */

#ifndef BP_RETURNNODE_H
#define BP_RETURNNODE_H


#include "ast/StatementNode.h"

class ReturnStatementNode: public StatementNode {
public:
    ReturnStatementNode(ExprNode *expr);
    virtual ~ReturnStatementNode();

    void print(Printer &printer) const override;

    void codegen(CodegenContext &context) override;

private:
    ExprNode *expr;
};


#endif //BP_RETURNNODE_H
