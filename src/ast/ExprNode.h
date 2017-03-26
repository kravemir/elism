/**
 * @author Miroslav Kravec
 */

#ifndef BP_EXPRNODE_H
#define BP_EXPRNODE_H

#include <string>

class ExprNode {
public:
    virtual std::string toString() const = 0;
};


#endif //BP_EXPRNODE_H
