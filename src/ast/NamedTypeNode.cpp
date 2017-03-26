/**
 * @author Miroslav Kravec
 */

#include "NamedTypeNode.h"

NamedTypeNode::NamedTypeNode(const std::string &name) : name(name) {}

std::string NamedTypeNode::toString() {
    return name;
}
