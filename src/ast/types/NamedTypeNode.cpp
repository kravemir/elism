/**
 * @author Miroslav Kravec
 */

#include <llvm/IR/DerivedTypes.h>

#include <CodegenContext.h>
#include <codegen/IntType.h>
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
    }
    CodegenType *type = context.getType(name);
    assert(type);
    return type;
}
