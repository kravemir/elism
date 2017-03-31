/**
 * @author Miroslav Kravec
 */

#include <llvm/IR/DerivedTypes.h>

#include <CodegenContext.h>
#include "NamedTypeNode.h"

NamedTypeNode::NamedTypeNode(const std::string &name) : name(name) {}

std::string NamedTypeNode::toString() {
    return name;
}

llvm::Type* NamedTypeNode::codegen(CodegenContext &context) {
    if("i32" == name) {
        return llvm::Type::getInt32Ty(context.llvmContext);;
    }
    assert(0);
    return 0;
}
