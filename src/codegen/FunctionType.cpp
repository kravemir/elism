//
// Created by miroslav on 4/4/17.
//

#include "FunctionType.h"

bool FunctionType::equals(CodegenType *pType, const std::map<std::string,std::string> &regionsRemap) {
    // TODO
    return false;
}

std::string FunctionType::toString() const {
    return "TODO: function";
}

CodegenValue *FunctionType::doCall(CodegenContext &ctx, CodegenValue *value, const std::vector<std::string> &regions,
                                   const std::vector<CodegenValue *> &args, const llvm::Twine &Name) {
    std::vector<llvm::Value*> values;
    if(!native) {
        llvm::Value *handle = ctx.getRegionHandle(regions[0]);
        if(handle == nullptr) {
            fprintf(stderr, "Don't have handle for: %s\n",regions[0].c_str());
            exit(1);
        }
        values.push_back(handle);
    }
    for(CodegenValue *v : args)
        values.push_back(v->value);
    CodegenType *retType =callReturnType;


    // TODO: check lengths
    std::map<std::string,std::string> regionsRemap;
    for(int i = 0; i < regions.size() && i < this->regions.size(); i++) {
        regionsRemap[regions[i]] = this->regions[i];
    }

    // TODO: varargs for printf
    for(int i = 0; i < args.size() && i < argTypes.size(); i++) {
        // TODO: region map
        if(!args[i]->type->equals(this->argTypes[i],regionsRemap)) {
            fprintf(stderr, "Argument type ``%s'' differs to parameter type ``%s'', with region mapping:\n", args[i]->type->toString().c_str(), this->argTypes[i]->toString().c_str());
            for(auto it : regionsRemap) {
                fprintf(stderr, "\t%s -> %s\n", it.first.c_str(), it.second.c_str());
            }
            exit(121);
        }
    }

    // todo
    std::map<std::string,std::string> regionsRemapInvert;
    for(auto it : regionsRemap) {
        regionsRemapInvert[it.second] = it.first;
    }

    retType = retType->withRemapRegions(ctx,regionsRemapInvert);

    return new CodegenValue(retType,ctx.builder.CreateCall(value->value,values,Name));
}

CodegenType *FunctionType::withRemapRegions(CodegenContext &context, const std::map<std::string, std::string> &map) {
    assert(0);
    return nullptr;
}
