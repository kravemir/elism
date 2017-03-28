//
// Created by miroslav on 3/28/17.
//

#include "NameExpr.h"

NameExpr::NameExpr(const std::string &name) : name(name) {}

std::string NameExpr::toString() const {
    return name;
}

llvm::Value *NameExpr::codegen(CodegenContext &context) {
    return context.getValue(name);
}
