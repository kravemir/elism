//
// Created by miroslav on 4/11/17.
//

#include "VoidType.h"

VoidType::VoidType(llvm::Type *const storeType) : CodegenType(storeType) {}

VoidType *VoidType::get(CodegenContext &ctx) {
    return new VoidType(llvm::Type::getVoidTy(ctx.llvmContext));
}

bool VoidType::equals(CodegenType *pType, const std::map<std::string,std::string> &regionsRemap) {
    return dynamic_cast<VoidType*>(pType) != nullptr;
}

std::string VoidType::toString() const {
    return "void";
}

CodegenType *VoidType::withRemapRegions(CodegenContext &context, const std::map<std::string, std::string> &map) {
    return this;
}
