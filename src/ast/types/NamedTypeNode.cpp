/**
 * @author Miroslav Kravec
 */

#include <llvm/IR/DerivedTypes.h>

#include <CodegenContext.h>
#include <codegen/IntType.h>
#include <codegen/VoidType.h>
#include "NamedTypeNode.h"

NamedTypeNode::NamedTypeNode(const std::string &name) : name(name) {}

std::string NamedTypeNode::toString() {
    return name;
}

CodegenType* NamedTypeNode::codegen(CodegenContext &context) {
    if("i32" == name) {
        return IntType::get32(context);
    } else if ("i64" == name) {
        return IntType::get64(context);
    } else if ("void" == name) {
        return VoidType::get(context);
    } else if ("bool" == name) {
        return new IntType(1, llvm::Type::getInt1Ty(context.llvmContext));
    }
    CodegenType *type = context.getType(name);
    assert(type);
    return type;
}
