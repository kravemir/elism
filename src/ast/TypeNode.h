/**
 * @author Miroslav Kravec
 */

#ifndef BP_TYPENODE_H
#define BP_TYPENODE_H

#include <string>

#include "llvm/IR/Type.h"

#include "CodegenContext.h"

class TypeNode {
public:
    virtual ~TypeNode();

    virtual std::string toString() = 0;
    virtual CodegenType *codegen(CodegenContext &context) = 0;
};


#endif //BP_TYPENODE_H
