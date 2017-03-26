/**
 * @author Miroslav Kravec
 */

#ifndef BP_INTVALNODE_H
#define BP_INTVALNODE_H

#include "ExprNode.h"

class IntValNode: public ExprNode {
public:
    IntValNode(long value);

    std::string toString() const override;
private:
    long value;
};


#endif //BP_INTVALNODE_H
