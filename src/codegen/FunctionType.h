//
// Created by miroslav on 4/4/17.
//

#ifndef BP_FUNCTIONTYPE_H
#define BP_FUNCTIONTYPE_H

#include <CodegenContext.h>

struct FunctionType: CodegenType {
    FunctionType(CodegenType *const callReturnType) : CodegenType(nullptr, callReturnType) {
        assert(callReturnType);
    }

    CodegenValue *doCall(CodegenContext &ctx, CodegenValue *value, std::vector<CodegenValue*> &args, const llvm::Twine &Name = "") override {
        std::vector<llvm::Value*> values;
        for(CodegenValue *v : args)
            values.push_back(v->value);
        return new CodegenValue(callReturnType,ctx.builder.CreateCall(value->value,values,Name));
    }
};


#endif //BP_FUNCTIONTYPE_H
