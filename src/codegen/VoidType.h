//
// Created by miroslav on 4/11/17.
//

#ifndef BP_VOIDTYPE_H
#define BP_VOIDTYPE_H


#include <CodegenContext.h>

class VoidType: public CodegenType {
public:
    VoidType(llvm::Type *const storeType);

    static VoidType* get(CodegenContext &ctx);

    bool equals(CodegenType *pType, const std::map<std::string,std::string> &regionsRemap) override;

    std::string toString() const override;

    CodegenType *withRemapRegions(CodegenContext &context, const std::map<std::string, std::string> &map) override;

private:
};


#endif //BP_VOIDTYPE_H
