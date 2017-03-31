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
    FunctionContext(LLVMContext &llvmContext, Module *const module, IRBuilder<> &builder, BasicBlock *entryBlock)
            : CodegenContext(llvmContext, module, builder),
              entryBlock(entryBlock)
    {}

    void addVariable(std::string name, llvm::Value *value) override {
        IRBuilder<> builder(entryBlock, entryBlock->begin());
        AllocaInst* alloca = builder.CreateAlloca(value->getType(), 0, "var." + name);
        variables[name] = alloca;
        builder.CreateStore(value, alloca);
    }

    void storeValue(std::string name, llvm::Value *value) override {
        auto it = variables.find(name);
        if(it != variables.end()) {
            builder.CreateStore(value, it->second);
        } else {
            assert(0);
        }
    }

    Value *getValue(std::string name) override {
        auto it = variables.find(name);
        if(it != variables.end()) {
            return builder.CreateLoad(it->second);
        }
        return CodegenContext::getValue(name);
    }

    void codegenReturn(llvm::Value *value) override {
        builder.CreateRet(value);
    }

    std::map<std::string,llvm::AllocaInst*> variables;
    BasicBlock *entryBlock;
};

void FunctionNode::codegen(CodegenContext &context) {
    Type *returnType = this->returnType->codegen(context);
    std::vector<llvm::Type*> args_llvmtypes;
    // TODO: args

    llvm::FunctionType *FT = llvm::FunctionType::get(returnType, args_llvmtypes, false);
    Function *F = Function::Create(FT, Function::ExternalLinkage, "main", context.module);

    // Create a new basic block to start insertion into.
    BasicBlock *BB = BasicBlock::Create(context.llvmContext, "entry", F);
    context.builder.SetInsertPoint(BB);

    FunctionContext functionContext(context.llvmContext, context.module, context.builder, BB);
    for(StatementNode *stmt : statements) {
        stmt->codegen(functionContext);
    }

    F->dump();
}
