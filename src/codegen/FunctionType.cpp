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
    if(!native)
        values.push_back(ctx.region);
    for(CodegenValue *v : args)
        values.push_back(v->value);
    CodegenType *retType =callReturnType;
    if(ClassType* ct = dynamic_cast<ClassType*>(retType)) {
        // TODO: more complex, based on called regions
        retType = ct->withRegions(ctx,{ctx.defaultRegion});
    }

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
    return new CodegenValue(retType,ctx.builder.CreateCall(value->value,values,Name));
}
