/**
 * @author Miroslav Kravec
 */

#include "ArrayTypeNode.h"

ArrayTypeNode::ArrayTypeNode(TypeNode *baseType) : baseType(baseType) {}

std::string ArrayTypeNode::toString() {
    return baseType->toString() + "[]";
}
