//
// Created by miroslav on 4/4/17.
//

#ifndef BP_FUNCTIONTYPE_H
#define BP_FUNCTIONTYPE_H

#include <CodegenContext.h>
#include <ast/declarations/ClassNode.h>

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
        CodegenType *retType =callReturnType;
        if(ClassType* ct = dynamic_cast<ClassType*>(retType)) {
            // TODO: more complex, based on called regions
            retType = ct->withRegions(ctx,{ctx.defaultRegion});
        }
        return new CodegenValue(retType,ctx.builder.CreateCall(value->value,values,Name));
    }

    bool equals(CodegenType *pType) override;

    std::string toString() const override;

    bool native = false;
};


#endif //BP_FUNCTIONTYPE_H
