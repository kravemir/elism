/**
 * @author Miroslav Kravec
 */

#ifndef BP_RETURNNODE_H
#define BP_RETURNNODE_H


#include "StatementNode.h"

class ReturnNode: public StatementNode {
public:
    ReturnNode(ExprNode *expr);
    virtual ~ReturnNode();

    void print(Printer &printer) const override;

private:
    ExprNode *expr;
};


#endif //BP_RETURNNODE_H
