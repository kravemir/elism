//
// Created by miroslav on 4/4/17.
//

#ifndef BP_FUNCTIONTYPE_H
#define BP_FUNCTIONTYPE_H

#include <CodegenContext.h>
#include <ast/declarations/ClassNode.h>

struct FunctionType: CodegenType {
    FunctionType(llvm::Type *ptrType, CodegenType *const callReturnType, const std::vector<std::string> &regions, const std::vector<CodegenType*> &argTypes)
            : CodegenType(ptrType, callReturnType),
              regions(regions),
              argTypes(argTypes)
    {
        assert(callReturnType);
    }

    CodegenValue *doCall(CodegenContext &ctx, CodegenValue *value, const std::vector<std::string> &regions,
                         const std::vector<CodegenValue *> &args, const llvm::Twine &Name = "") override;

    bool equals(CodegenType *pType, const std::map<std::string,std::string> &regionsRemap) override;

    std::string toString() const override;

    CodegenType *withRemapRegions(CodegenContext &context, const std::map<std::string, std::string> &map) override;

    bool native = false;

    std::vector<std::string> regions;
    std::vector<CodegenType*> argTypes;
};


#endif //BP_FUNCTIONTYPE_H
