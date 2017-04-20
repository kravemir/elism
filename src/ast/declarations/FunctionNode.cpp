/**
 * @author Miroslav Kravec
 */
#include "FunctionNode.h"
#include "ClassNode.h"

#include <codegen/FunctionType.h>
#include <codegen/VoidType.h>
#include <ast/expressions/CallExprNode.h>
#include <ast/expressions/NameExprNode.h>

using namespace llvm;

FunctionNode::FunctionNode(const std::string &name,
                           TypeNode *returnType,
                           const std::vector<std::string> &regions,
                           const std::vector<std::pair<std::string, TypeNode *>> arguments,
                           const std::vector<StatementNode *> &statements)
        : name(name),
          regions(regions),
          arguments(arguments),
          returnType(returnType),
          statements(statements)
{}

void FunctionNode::print(Printer &printer) const {
    printer.print("fn ");
    printer.print(name);
    printer.print(" (");
    bool firstArg = true;
    for(auto arg : arguments) {
        if(firstArg)
            firstArg = false;
        else
            printer.print(", ");
        printer.print(arg.first);
        printer.print(": ");
        printer.print(arg.second->toString());
    }
    printer.print(") ");
    if(returnType) {
        printer.print("-> ");
        printer.print(returnType->toString());
        printer.print(" ");
    }
    printer.beginBlock();
    for(StatementNode *s : this->statements) {
        s->print(printer);
    }
    printer.endBlock();
}

FunctionNode::~FunctionNode() {
    if(returnType)
        delete returnType;
    for(auto arg : arguments) {
        delete arg.second;
    }
    for(StatementNode *s : this->statements) {
        delete s;
    }
}

class FunctionContext: public ChildCodegenContext {
public:
    FunctionContext(CodegenContext &parent)
            : ChildCodegenContext(parent)
    {}

    AllocaInst* createAlloca(std::string name, CodegenValue *value) override {
        IRBuilder<> builder(entryBlock, entryBlock->begin());
        AllocaInst* alloca = builder.CreateAlloca(value->value->getType(), 0, "var." + name);
        this->builder.CreateStore(value->value, alloca);
        return alloca;
    }

    CodegenValue * getValue(std::string name) override {
        //printf("Get function: %s\n",name.c_str());
        auto it = variables.find(name);
        if(it != variables.end()) {
            return new CodegenValue(it->second.first,builder.CreateLoad(it->second.second,name),it->second.second);
        }
        CodegenValue *value = CodegenContext::getValue(name);
        if(value == nullptr && thisType != nullptr) {
            value = thisType->getChild(*this, classInstance, name);
        }
        if(value == nullptr) {
            return parent.getValue(name);
        }
        return value;
    }

    void codegenReturn(CodegenValue *value) override {
        builder.CreateRet(value->value);
    }

    BasicBlock *entryBlock;
    ClassType *thisType = nullptr;
    CodegenValue *classInstance = nullptr;
};

void FunctionNode::codegen(CodegenContext &context) {
    context.addValue(name,codegenFunction(context, nullptr));
}


void FunctionNode::codegenAsClassStatement(ClassTypeContext &context) {
    auto f = codegenFunction(context,context.classType);
    context.addValue(name,f);
    context.classType->functions[name] = f;
}

CodegenValue* FunctionNode::codegenFunction(CodegenContext &context, ClassType *classType) {
    if(regions.size() == 0)
        regions.push_back("fn_default");

    FunctionContext functionContext(context);
    functionContext.defaultRegion = regions[0];

    CodegenType *returnType = this->returnType->codegen(functionContext);
    //fprintf(stderr,"aaa: %s\n",returnType->toString().c_str());
    std::vector<CodegenType*> arg_types;
    std::vector<llvm::Type*> args_llvmtypes = {context.regionType};

    if(classType) {
        arg_types.push_back(classType);
        args_llvmtypes.push_back(classType->storeType);
    }

    for(auto arg : arguments) {
        CodegenType *argType = arg.second->codegen(functionContext);
        arg_types.push_back(argType);
        args_llvmtypes.push_back(argType->storeType);
    }

    llvm::FunctionType *FT = llvm::FunctionType::get(returnType->storeType, args_llvmtypes, false);
    Function *F = Function::Create(FT, Function::ExternalLinkage, name, context.module);

    // Create a new basic block to start insertion into.
    BasicBlock *BB = BasicBlock::Create(context.llvmContext, "entry", F);
    functionContext.entryBlock = BB;

    functionContext.builder.SetInsertPoint(BB);
    functionContext.thisType = classType;
    Function::arg_iterator arg = F->arg_begin();
    arg->setName("region");
    functionContext.region = (Argument*)(arg++);

    if(classType) {
        arg->setName("this");
        functionContext.classInstance = new CodegenValue(classType, (Argument *) (arg++));
    }

    for(int i = 0; i < arguments.size(); i++) {
        arg->setName("arg." + arguments[i].first);
        functionContext.addValue(arguments[i].first, new CodegenValue(arg_types[i], (Argument*)(arg++)));
    }
    for(StatementNode *stmt : statements) {
        stmt->codegen(functionContext);
    }
    if(dynamic_cast<VoidType*>(returnType))
        functionContext.builder.CreateRetVoid();

    ::FunctionType *CFT = new ::FunctionType(FT,returnType,regions,arg_types);
    return new CodegenValue(CFT,F);
}