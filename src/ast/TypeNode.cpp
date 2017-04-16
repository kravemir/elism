/**
 * @author Miroslav Kravec
 */

#include <llvm/IR/Type.h>
#include <CodegenContext.h>
#include "TypeNode.h"

TypeNode::~TypeNode() {

}

void TypeNode::setRegions(std::vector<std::string> &&regions) {
    this->regions = regions;
}
