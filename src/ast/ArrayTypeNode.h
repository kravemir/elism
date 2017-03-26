/**
 * @author Miroslav Kravec
 */

#ifndef BP_ARRAYTYPENODE_H
#define BP_ARRAYTYPENODE_H


#include "TypeNode.h"

class ArrayTypeNode: public TypeNode {
public:
    ArrayTypeNode(TypeNode *baseType);

    std::string toString() override;

private:
    TypeNode *baseType;
};


#endif //BP_ARRAYTYPENODE_H
