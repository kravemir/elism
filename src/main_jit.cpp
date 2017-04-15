/**
 * @author Miroslav Kravec
 */

#include <iostream>
#include <fstream>

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

#include "rtlib/regions.h"

using namespace std;
using namespace llvm;

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
    auto cft = new ::FunctionType(printf_type,IntType::get32(ctx));
    cft->native = true;
    ctx.addValue("printf", new CodegenValue(cft, func));
}

extern char _binary_stdlib_bp_start[];
extern char _binary_stdlib_bp_end[];

void parse(const char *buffer_ptr, Program &p) {
    // perform lexical analysis
    YYSTYPE yylval = {0, 0};
    void *pParser = ParseAlloc(malloc);
    int tokenID;
    while (tokenID = lex(buffer_ptr, yylval)) {
        Parse(pParser, tokenID, yylval, &p);
        yylval.str_value = 0;
    }
    Parse(pParser, 0, yylval, &p);
    ParseFree(pParser, free);
}

void parseBuffer(Program &p, const char *start, size_t length) {
    char *buffer = new char[length + 1];
    memcpy(buffer,start,length);
    buffer[length] = 0;

    parse(buffer,p);

    delete[] buffer;
}

void parseFile(Program &p, std::string filename) {
    ifstream in(filename);
    in.seekg(0, ios::end);

    long length = in.tellg();
    char * buffer = new char[length + 1];

    in.seekg(0, ios::beg);
    in.read(buffer, length);
    buffer[length] = 0;

    parse(buffer,p);

    delete[] buffer;
}

static void register_regions(CodegenContext &ctx) {
    llvm::Type *regionType = llvm::Type::getInt64PtrTy(ctx.llvmContext);
    ctx.regionType = regionType;

    llvm::FunctionType* newregion_type = llvm::FunctionType::get(regionType, {}, true);
    llvm::FunctionType* deleteregion_type = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx.llvmContext), {regionType}, true);
    llvm::FunctionType* regionalloc_type = llvm::FunctionType::get(llvm::Type::getInt8PtrTy(ctx.llvmContext), {regionType,llvm::Type::getInt64Ty(ctx.llvmContext)}, true);


    llvm::Function *newregion = llvm::Function::Create(
            newregion_type, llvm::Function::ExternalLinkage,
            llvm::Twine("NewRegion"),
            ctx.module
    );
    newregion->setCallingConv(llvm::CallingConv::C);

    llvm::Function *deleteregion = llvm::Function::Create(
            deleteregion_type, llvm::Function::ExternalLinkage,
            llvm::Twine("DeleteRegion"),
            ctx.module
    );
    deleteregion->setCallingConv(llvm::CallingConv::C);

    llvm::Function *regionalloc = llvm::Function::Create(
            regionalloc_type, llvm::Function::ExternalLinkage,
            llvm::Twine("RegionAlloc"),
            ctx.module
    );
    regionalloc->setCallingConv(llvm::CallingConv::C);

    CodegenType *rtype = new CodegenType(regionType);
    ctx.addValue("NewRegion", new CodegenValue(new ::FunctionType(newregion_type,rtype), newregion));
}

std::unique_ptr<llvm::Module> compileProgram(LLVMContext &llvmContext, Program &p) {
    std::unique_ptr<llvm::Module> Owner = llvm::make_unique<llvm::Module>("my cool jit", llvmContext);
    Module *TheModule = Owner.get();

    IRBuilder<> Builder(llvmContext);
    CodegenContext ctx(llvmContext, TheModule, Builder);
    register_printf(ctx);
    register_regions(ctx);

    p.codegen(ctx);

    return Owner;
}

int runProgram(std::unique_ptr<llvm::Module> Owner) {
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();
    LLVMLinkInMCJIT();

    Module *TheModule = Owner.get();

    std::string ErrStr;
    ExecutionEngine *TheExecutionEngine =
            EngineBuilder(std::move(Owner))
                    .setErrorStr(&ErrStr)
                    .setMCJITMemoryManager(llvm::make_unique<SectionMemoryManager>())
                    .create();

    if (!TheExecutionEngine) {
        fprintf(stderr, "Could not create ExecutionEngine: %s\n", ErrStr.c_str());
        exit(1);
    }

    TheModule->setDataLayout(TheExecutionEngine->getDataLayout());
    TheExecutionEngine->finalizeObject();

    // run
    Function *LF = TheModule->getFunction("main");
    void *FPtr = TheExecutionEngine->getPointerToFunction(LF);

    Region *region = NewRegion();
    int (*FP)(Region *) = (int (*)(Region *))(intptr_t)FPtr;
    int result = FP(region);
    DeleteRegion(region);

    return result;
}

int main(int argc, char **argv) {
    Program p;
    parseBuffer(p, _binary_stdlib_bp_start, _binary_stdlib_bp_end - _binary_stdlib_bp_start);
    parseFile(p, argv[1]);

    LLVMContext llvmContext;
    std::unique_ptr<llvm::Module> compiled = compileProgram(llvmContext,p);
    return runProgram(std::move(compiled));
}