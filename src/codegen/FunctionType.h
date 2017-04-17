//
// Created by miroslav on 4/4/17.
//

#ifndef BP_FUNCTIONTYPE_H
#define BP_FUNCTIONTYPE_H

#include <CodegenContext.h>

struct FunctionType: CodegenType {
    FunctionType(llvm::Type *ptrType, CodegenType *const callReturnType) : CodegenType(ptrType, callReturnType) {
        assert(callReturnType);
    }

    CodegenValue *doCall(CodegenContext &ctx, CodegenValue *value, const std::vector<CodegenValue *> &args,
                         const llvm::Twine &Name = "") override {
        std::vector<llvm::Value*> values;
        if(!native)
            values.push_back(ctx.region);
        for(CodegenValue *v : args)
            values.push_back(v->value);
        return new CodegenValue(callReturnType,ctx.builder.CreateCall(value->value,values,Name));
    }

    bool equals(CodegenType *pType) override;

    bool native = false;
};


#endif //BP_FUNCTIONTYPE_H
