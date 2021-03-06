//
// Created by miroslav on 3/31/17.
//

#include "ClassNode.h"

#include <codegen/FunctionType.h>
#include <llvm/IR/Module.h>
#include <ast/expressions/CallExprNode.h>
#include <ast/expressions/NameExprNode.h>

using namespace llvm;

ClassNode::ClassNode(const std::string &name, const std::vector<StatementNode *> &statements)
        : name(name),
          hasSuper(false),
          statements(statements)
{}

ClassNode::ClassNode(const std::string &name, const std::string &super, const std::vector<StatementNode *> &statements)
        : name(name),
          super(super),
          hasSuper(true),
          statements(statements)
{}

void ClassNode::print(Printer &printer) {
    printer.println("TODO");
}

void ClassNode::codegen(CodegenContext &context) {
    ClassType *super = nullptr;
    if(hasSuper) {
        super = dynamic_cast<ClassType*>(context.getType(this->super));
        assert(super);
    }

    StructType *classType = StructType::create(context.llvmContext,"class." + name + ".Instance");
    Type *classPtrType = PointerType::get(classType,0);
    ClassType *cClassType = new ClassType(name,classPtrType,super,context.defaultRegion);
    cClassType->base = cClassType;

    llvm::FunctionType *FT_init = llvm::FunctionType::get(Type::getVoidTy(context.llvmContext), {context.regionType,classPtrType}, false);
    Function *F_init = Function::Create(FT_init, Function::ExternalLinkage, "class." + name + ".init", context.module);
    cClassType->initF = F_init;

    ClassTypeContext typeContext(context);
    typeContext.classType = cClassType;
    {
        // Create a new basic block to start insertion into.
        BasicBlock *BB = BasicBlock::Create(context.llvmContext, "entry.initializers", F_init);
        context.builder.SetInsertPoint(BB);
        typeContext.region = (Argument*)F_init->arg_begin();

        for (StatementNode *var : statements) {
            if(dynamic_cast<VarStatementNode*>(var)) // TODO: codegen as class initialzier
                var->codegen(typeContext);
        }


        std::vector<Type *> types;
        int idx = super ? 1 : 0;
        if(super){
            types.push_back(super->storeType->getPointerElementType());
        }
        for (auto v : typeContext.variables) {
            cClassType->children[v.first] = std::make_pair(idx++,v.second->type);
            types.push_back(v.second->value->getType());
        }
        if(super) {
            typeContext.builder.CreateCall(super->initF, {(Argument*)F_init->arg_begin()});
        }
        classType->setBody(types);

        idx = super ? 1 : 0;
        for (auto v : typeContext.variables) {
            Value *ptr = typeContext.builder.CreateGEP(
                    (Argument *) (++(F_init->arg_begin())),
                    (std::vector<Value *>) {
                            Constant::getNullValue(IntegerType::getInt64Ty(typeContext.llvmContext)),
                            ConstantInt::get(typeContext.llvmContext, APInt((unsigned) 32, (uint64_t) idx++))
                    },
                    "var." + v.first + ".addr"
            );
            typeContext.builder.CreateStore(v.second->value, ptr);
        }

        context.builder.CreateRetVoid();
    }

    for (StatementNode *stmt : statements) {
        stmt->codegenAsClassStatement(typeContext);
    }


    llvm::FunctionType *FT_new = llvm::FunctionType::get(classPtrType, {context.regionType}, false);
    Function *F_new = Function::Create(FT_new, Function::ExternalLinkage, "class." + name + ".new", context.module);

    {
        // Create a new basic block to start insertion into.
        BasicBlock *BB = BasicBlock::Create(context.llvmContext, "entry.initializers", F_new);
        context.builder.SetInsertPoint(BB);
        context.region = (Argument*)F_new->arg_begin();

        Type* ITy = Type::getInt64PtrTy(context.llvmContext);
        Constant* AllocSize = ConstantExpr::getSizeOf(classType);
        AllocSize = ConstantExpr::getTruncOrBitCast(AllocSize, ITy);
        Value* Malloc = context.createAlloc(classType, AllocSize,nullptr);

        context.builder.CreateCall(F_init, {context.region,Malloc});
        context.builder.CreateRet(Malloc);
    }

    ::FunctionType *CFT = new ::FunctionType(FT_new,cClassType->withRegions(context,{"fn_default"}),{"fn_default"},{});
    context.addValue(name, new CodegenValue(CFT,F_new));
    context.addType(name, cClassType);

}

ClassNode::~ClassNode() {
    for(StatementNode *s : statements)
        delete s;
}

struct ClassFunctionType: CodegenType {
    ClassFunctionType(Type * storeType, CodegenType *const callReturnType, std::string instanceRegion, const std::vector<std::string> &regions, const std::vector<CodegenType*> &argTypes)
            : CodegenType(storeType, callReturnType), instanceRegion(instanceRegion), regions(regions), argTypes(argTypes)
    {}

    CodegenValue * doCall(CodegenContext &ctx, CodegenValue *value, const std::vector<std::string> &regions,
                          const std::vector<CodegenValue *> &args, const Twine &Name) override {
        std::vector<llvm::Value*> values;
        // add default handle
        if(true) {
            assert(regions.size() > 0);
            llvm::Value *handle = ctx.getRegionHandle(regions[0]);
            if(handle == nullptr) {
                fprintf(stderr, "Don't have handle for: %s\n",regions[0].c_str());
                exit(1);
            }
            values.push_back(handle);
        }
        // add this ptr
        values.push_back(ctx.builder.CreateExtractValue(value->value,{0}));
        // add arguments
        for(CodegenValue *v : args)
            values.push_back(v->value);
        CodegenType *retType =callReturnType;

        if(regions.size() != this->regions.size()) {
            fprintf(stderr, "Call regions count %lu differs from function declaration regions count %lu\n", regions.size(), this->regions.size());
            value->value->getType()->dump();
            exit(121);
        }

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

        {
            auto it = regionsRemap.find(instanceRegion);
            if(it == regionsRemap.end() || it->second != "this") {
                fprintf(stderr, "Bad instance region for call, should be %s, not %s\n", instanceRegion.c_str(), it == regionsRemap.end() ? "<none>" : it->second.c_str());
                for(auto it : regionsRemap) {
                    fprintf(stderr, "\t%s -> %s\n", it.first.c_str(), it.second.c_str());
                }
                exit(121);

            }
        }

        return new CodegenValue(retType,ctx.builder.CreateCall(ctx.builder.CreateExtractValue(value->value,{1}),values,Name));
    }

    bool equals(CodegenType *pType, const std::map<std::string,std::string> &regionsRemap) override {
        return false;
    }

    std::string toString() const override {
        return "TODO: cft";
    }

    CodegenType *withRemapRegions(CodegenContext &context, const std::map<std::string, std::string> &map) override {
        assert(0);
        return nullptr;
    }

    std::string instanceRegion; // TODO: shouldn't be part of type bad value
    std::vector<std::string> regions;
    std::vector<CodegenType*> argTypes;
};

CodegenValue *ClassType::getChild(CodegenContext &ctx, CodegenValue *value, std::string name) {
    llvm::Value *valThis = value->value;

    ClassType *clst = this;
    while(clst) {
        {
            auto it = clst->children.find(name);
            if (it != clst->children.end()) {
                llvm::Value *ptr = ctx.builder.CreateGEP(
                        valThis,
                        (std::vector<llvm::Value *>) {
                                llvm::Constant::getNullValue(llvm::IntegerType::getInt64Ty(ctx.llvmContext)),
                                llvm::ConstantInt::get(ctx.llvmContext,
                                                       llvm::APInt((unsigned) 32, (uint64_t) it->second.first))
                        },
                        "child." + name + ".addr"
                );
                return new CodegenValue(it->second.second, ctx.builder.CreateLoad(ptr, "child." + name), ptr);
            }
        }
        {
            auto it = clst->functions.find(name);
            if (it != clst->functions.end()) {
                llvm::StructType *lType = StructType::get(
                        ctx.llvmContext,
                        {
                                storeType,
                                PointerType::get(it->second->type->storeType, 0)
                        }
                );
                ::FunctionType* ftype = (::FunctionType *) it->second->type;
                assert(ftype->regions.size() > 0);
                ClassType *clsType = dynamic_cast<ClassType*>(value->type);
                assert(clsType);
                ClassFunctionType *type = new ClassFunctionType(lType, ftype->callReturnType, clsType->region, ftype->regions, ftype->argTypes);
                Value *val = ConstantStruct::get(lType, {
                        Constant::getNullValue(storeType),
                        Constant::getNullValue(PointerType::get(it->second->type->storeType, 0)),
                });
                val = ctx.builder.CreateInsertValue(val, valThis, {0});
                val = ctx.builder.CreateInsertValue(val, it->second->value, {1});

                return new CodegenValue(type, val);
            }
        }
        clst = clst->super;
        if(clst) {
            valThis = ctx.builder.CreateGEP(valThis, {
                    ConstantInt::getNullValue(Type::getInt64Ty(ctx.llvmContext)),
                    ConstantInt::getNullValue(Type::getInt32Ty(ctx.llvmContext))
            });
        }
    }
    return nullptr;
}

bool ClassType::equals(CodegenType *pType, const std::map<std::string,std::string> &regionsRemap) {
    ClassType *ct = dynamic_cast<ClassType*>(pType);
    if(ct == nullptr)
        return false;
    if(regionsRemap.size() > 0) {
        auto it = regionsRemap.find(this->region);
        // TODO check if found, what next if not found???
        if (it->second != ct->region) {
            return false;
        }
    } else {
        if (this->region != ct->region) {
            return false;
        }
    }
    return base == ct->base;
}

std::string ClassType::toString() const {
    return name + " @" + region;
}

CodegenType *ClassType::withRegions(CodegenContext &ctx, const std::vector<std::string> &regions) {
    std::string region = ctx.defaultRegion;
    if(regions.size() == 1)
        region = regions[0];
    else
        assert(regions.size() == 0);
    assert("" != region);
    ClassType* ct = new ClassType(name,storeType,super,region);
    ct->initF = initF;
    ct->functions = functions;
    ct->children = children;
    ct->base = this->base;
    return ct;
}

CodegenType *ClassType::withRemapRegions(CodegenContext &context, const std::map<std::string, std::string> &map) {
    std::string r;
    auto it = map.find(region);
    if(it != map.end()) {
        r = it->second;
    } else {
        fprintf(stderr,"Can't find region to remap ``%s'', self ``%s''\n",region.c_str(),this->toString().c_str());
        for(auto it : map) {
            fprintf(stderr, "\t%s -> %s\n", it.first.c_str(), it.second.c_str());
        }
        throw "AAA";
        exit(1);
    }
    return this->withRegions(context, { r } );
}
