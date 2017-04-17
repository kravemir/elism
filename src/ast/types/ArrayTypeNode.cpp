/**
 * @author Miroslav Kravec
 */

#include <llvm/IR/Type.h>
#include <CodegenContext.h>
#include "ArrayTypeNode.h"

#include <codegen/ArrayType.h>

ArrayTypeNode::ArrayTypeNode(TypeNode *baseType) : baseType(baseType) {}

std::string ArrayTypeNode::toString() {
    return baseType->toString() + "[]";
}

ArrayTypeNode::~ArrayTypeNode() {
    delete baseType;
}

CodegenType *ArrayTypeNode::codegen(CodegenContext &context) {
    std::string region = context.defaultRegion;
    if(this->regions.size() == 1)
        region = regions[0];
    else if(this->regions.size() > 1) {
        fprintf(stderr,"Error - array type can have only one region\n");
        exit(1);
    }
    return ArrayType::get(context, baseType->codegen(context),region);
}
