/**
 * @author Miroslav Kravec
 */

#include "IntValNode.h"

IntValNode::IntValNode(long value) : value(value) {}

std::string IntValNode::toString() const {
    return std::to_string(value);
}
