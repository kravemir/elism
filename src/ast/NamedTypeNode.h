/**
 * @author Miroslav Kravec
 */

#ifndef BP_NAMEDTYPENODE_H
#define BP_NAMEDTYPENODE_H

#include "TypeNode.h"

class NamedTypeNode: public TypeNode {
public:
    NamedTypeNode(const std::string &name);

    std::string toString() override;

private:
    std::string name;
};


#endif //BP_NAMEDTYPENODE_H
