/**
 * @author Miroslav Kravec
 */
#include "FunctionNode.h"

#include <CodegenContext.h>
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

FunctionNode::FunctionNode(const char *name, TypeNode *returnType,
                           const std::vector<std::pair<std::string, TypeNode *>> arguments,
                           const std::vector<StatementNode *> &statements)
        : name(name),
          arguments(arguments),
          returnType(returnType),
          statements(statements)
{}

void FunctionNode::print(Printer &printer) {
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
    delete[] name;
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
    FunctionContext(CodegenContext &parent, BasicBlock *entryBlock)
            : ChildCodegenContext(parent),
              entryBlock(entryBlock)
    {}

    void addVariable(std::string name, CodegenValue *value) override {
        //printf("Add variable: %s\n",name.c_str());
        IRBuilder<> builder(entryBlock, entryBlock->begin());
        AllocaInst* alloca = builder.CreateAlloca(value->value->getType(), 0, "var." + name);
        variables[name] = std::make_pair(value->type,alloca);
        this->builder.CreateStore(value->value, alloca);
    }

    void storeValue(std::string name, CodegenValue *value) override {
        auto it = variables.find(name);
        if(it != variables.end()) {
            builder.CreateStore(value->value, it->second.second);
        } else {
            assert(0);
        }
    }

    CodegenValue * getValue(std::string name) override {
        //printf("Get function: %s\n",name.c_str());
        auto it = variables.find(name);
        if(it != variables.end()) {
            return new CodegenValue(it->second.first,builder.CreateLoad(it->second.second,name),it->second.second);
        }
        CodegenValue *value = CodegenContext::getValue(name);
        if(!value)
            return parent.getValue(name);
        return value;
    }

    void codegenReturn(CodegenValue *value) override {
        builder.CreateRet(value->value);
    }

    std::map<std::string,std::pair<CodegenType*,llvm::AllocaInst*>> variables;
    BasicBlock *entryBlock;
};

void FunctionNode::codegen(CodegenContext &context) {
    CodegenType *returnType = this->returnType->codegen(context);
    std::vector<llvm::Type*> args_llvmtypes;
    // TODO: args

    llvm::FunctionType *FT = llvm::FunctionType::get(returnType->storeType, args_llvmtypes, false);
    Function *F = Function::Create(FT, Function::ExternalLinkage, name, context.module);

    // Create a new basic block to start insertion into.
    BasicBlock *BB = BasicBlock::Create(context.llvmContext, "entry", F);
    context.builder.SetInsertPoint(BB);

    FunctionContext functionContext(context, BB);
    for(StatementNode *stmt : statements) {
        stmt->codegen(functionContext);
    }

    ::FunctionType *CFT = new ::FunctionType(returnType);

    F->dump();
    context.addValue(name,new CodegenValue(CFT,F));
}
