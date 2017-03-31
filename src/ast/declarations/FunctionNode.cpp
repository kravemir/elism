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

class FunctionContext: public CodegenContext {
public:
    FunctionContext(CodegenContext &parent,BasicBlock *entryBlock)
            : CodegenContext(parent.llvmContext, parent.module, parent.builder),
              parent(parent),
              entryBlock(entryBlock)
    {}

    void addVariable(std::string name, CodegenValue *value) override {
        //printf("Add variable: %s\n",name.c_str());
        IRBuilder<> builder(entryBlock, entryBlock->begin());
        AllocaInst* alloca = builder.CreateAlloca(value->value->getType(), 0, "var." + name);
        variables[name] = alloca;
        this->builder.CreateStore(value->value, alloca);
    }

    void storeValue(std::string name, CodegenValue *value) override {
        auto it = variables.find(name);
        if(it != variables.end()) {
            builder.CreateStore(value->value, it->second);
        } else {
            assert(0);
        }
    }

    CodegenValue * getValue(std::string name) override {
        //printf("Get function: %s\n",name.c_str());
        auto it = variables.find(name);
        if(it != variables.end()) {
            return new CodegenValue(builder.CreateLoad(it->second));
        }
        CodegenValue *value = CodegenContext::getValue(name);
        if(!value)
            return parent.getValue(name);
        return value;
    }

    void codegenReturn(CodegenValue *value) override {
        builder.CreateRet(value->value);
    }

    CodegenContext &parent;
    std::map<std::string,llvm::AllocaInst*> variables;
    BasicBlock *entryBlock;
};

struct FunctionValue: CodegenValue {
    FunctionValue(Value *value, Type *const callReturnType) : CodegenValue(value, callReturnType) {}

    CodegenValue *doCall(CodegenContext &ctx) override {
        return new CodegenValue(ctx.builder.CreateCall(value,{},"call"));
    }
};

void FunctionNode::codegen(CodegenContext &context) {
    Type *returnType = this->returnType->codegen(context);
    std::vector<llvm::Type*> args_llvmtypes;
    // TODO: args

    llvm::FunctionType *FT = llvm::FunctionType::get(returnType, args_llvmtypes, false);
    Function *F = Function::Create(FT, Function::ExternalLinkage, name, context.module);

    // Create a new basic block to start insertion into.
    BasicBlock *BB = BasicBlock::Create(context.llvmContext, "entry", F);
    context.builder.SetInsertPoint(BB);

    FunctionContext functionContext(context, BB);
    for(StatementNode *stmt : statements) {
        stmt->codegen(functionContext);
    }

    F->dump();
    context.addValue(name,new FunctionValue(F,returnType));
}
