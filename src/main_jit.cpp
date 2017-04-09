/**
 * @author Miroslav Kravec
 */

#include <iostream>


#include "llvm/Analysis/Passes.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"

#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>

#include "llvm/IR/LegacyPassManager.h"

#include <lexer.h>
#include <parser.cpp>
#include <codegen/IntType.h>
#include <codegen/FunctionType.h>

using namespace std;
using namespace llvm;

static int dummy_printf(const char *__restrict __format, ...) {
    return 0;
}

static ExecutionEngine *TheExecutionEngine;

static void register_printf(CodegenContext &ctx) {
    std::vector<llvm::Type*> printf_arg_types;
    printf_arg_types.push_back(llvm::Type::getInt8PtrTy(ctx.llvmContext));

    llvm::FunctionType* printf_type =
            llvm::FunctionType::get(
                    llvm::Type::getInt32Ty(ctx.llvmContext), printf_arg_types, true);

    llvm::Function *func = llvm::Function::Create(
            printf_type, llvm::Function::ExternalLinkage,
            llvm::Twine("printf"),
            ctx.module
    );
    func->setCallingConv(llvm::CallingConv::C);
    ctx.addValue("printf", new CodegenValue(new ::FunctionType(printf_type,IntType::get32(ctx)), func));
}

extern char _binary_stdlib_bp_start[];
extern char _binary_stdlib_bp_end[];

int main(int argc, char **argv)
{
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();
    LLVMLinkInMCJIT();

    Program p;
    {
        size_t length = _binary_stdlib_bp_end - _binary_stdlib_bp_start;
        char *buffer;
        const char *buffer_ptr;

        // read whole stdin
        buffer = new char[length + 1];
        memcpy(buffer,_binary_stdlib_bp_start,length);
        buffer[length] = 0;
        buffer_ptr = buffer;

        // perform lexical analysis
        YYSTYPE yylval = {0, 0};
        void *pParser = ParseAlloc(malloc);
        int tokenID;
        while (tokenID = lex(buffer_ptr, dummy_printf, yylval)) {
            Parse(pParser, tokenID, yylval, &p);
            yylval.str_value = 0;
        }
        Parse(pParser, 0, yylval, &p);
        ParseFree(pParser, free);

        // free resources
        delete[] buffer;
    }
    {
        long length;
        char *buffer;
        const char *buffer_ptr;

        // read whole stdin
        std::cin.seekg(0, std::ios::end);
        length = std::cin.tellg();
        std::cin.seekg(0, std::ios::beg);
        buffer = new char[length + 1];
        std::cin.read(buffer, length);
        buffer[length] = 0;
        buffer_ptr = buffer;

        // perform lexical analysis
        YYSTYPE yylval = {0, 0};
        void *pParser = ParseAlloc(malloc);
        int tokenID;
        while (tokenID = lex(buffer_ptr, dummy_printf, yylval)) {
            Parse(pParser, tokenID, yylval, &p);
            yylval.str_value = 0;
        }
        Parse(pParser, 0, yylval, &p);
        ParseFree(pParser, free);

        // free resources
        delete[] buffer;
    }

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
        IRBuilder<> Builder(llvmContext);
        CodegenContext ctx(llvmContext, TheModule, Builder);
        register_printf(ctx);
        p.codegen(ctx);
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