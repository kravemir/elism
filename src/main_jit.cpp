/**
 * @author Miroslav Kravec
 */

#include <iostream>


#include "llvm/Analysis/Passes.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Pass.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"

#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/SourceMgr.h>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/Scalar.h"

#include <lexer.h>
#include <parser.cpp>
#include "lexer.h"
#include "ast/Program.h"

using namespace std;
using namespace llvm;

static int dummy_printf(const char *__restrict __format, ...) {
    return 0;
}

static ExecutionEngine *TheExecutionEngine;

int main(int argc, char **argv)
{
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();

    long length;
    char *buffer;
    const char *buffer_ptr;

    // read whole stdin
    std::cin.seekg(0, std::ios::end);
    length = std::cin.tellg();
    std::cin.seekg(0, std::ios::beg);
    buffer = new char[length+1];
    std::cin.read(buffer, length);
    buffer[length] = 0;
    buffer_ptr = buffer;

    // perform lexical analysis
    YYSTYPE yylval = {0,0};
    void *pParser = ParseAlloc(malloc);
    int tokenID;
    Program p;
    while(tokenID = lex(buffer_ptr,dummy_printf,yylval)) {
        Parse(pParser, tokenID, yylval, &p);
        yylval.str_value = 0;
    }
    Parse(pParser, 0, yylval, &p);
    ParseFree(pParser, free);

    Printer printer;
    p.print(printer);

    // free resources
    delete[] buffer;

    LLVMContext llvmContext;
    SMDiagnostic error;
    //Module *m = parseIRFile("decl.ll", error, llvmContext);
    std::unique_ptr<llvm::Module> Owner = llvm::make_unique<llvm::Module>("my cool jit", llvmContext);
    //std::unique_ptr<llvm::Module> Owner = std::unique_ptr<llvm::Module>(m);
    //std::unique_ptr<llvm::Module> Owner = parseIRFile("decl.ll", error, llvmContext);
    Module *TheModule = Owner.get();

    std::string ErrStr;
    TheExecutionEngine =
            EngineBuilder(std::move(Owner))
                    .setErrorStr(&ErrStr)
                    .setMCJITMemoryManager(llvm::make_unique<SectionMemoryManager>())
                    .create();

    if (!TheExecutionEngine) {
        fprintf(stderr, "Could not create ExecutionEngine: %s\n", ErrStr.c_str());
        exit(1);
    }

    TheModule->setDataLayout(TheExecutionEngine->getDataLayout());

    // generate
    {
        IntegerType *type = llvm::Type::getInt32Ty(TheModule->getContext());
        std::vector<llvm::Type*> args_llvmtypes;

        llvm::FunctionType *FT = llvm::FunctionType::get(type, args_llvmtypes, false);
        Function *F = Function::Create(FT, Function::ExternalLinkage, "main", TheModule);

        // Create a new basic block to start insertion into.
        BasicBlock *BB = BasicBlock::Create(llvmContext, "entry", F);
        IRBuilder<> Builder(llvmContext);
        Builder.SetInsertPoint(BB);

        Builder.CreateRet(ConstantInt::get(llvmContext,APInt((unsigned)32,42)));

        F->dump();
    }

    // dump
    TheExecutionEngine->finalizeObject();

    // run
    Function *LF = TheModule->getFunction("main");
    void *FPtr = TheExecutionEngine->getPointerToFunction(LF);

    // Cast it to the right type (takes no arguments, returns a double) so we
    // can call it as a native function.
    int (*FP)() = (int (*)())(intptr_t)FPtr;
    int result = FP();

    return result;
}