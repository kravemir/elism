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
    while (tokenID = lex(buffer_ptr, dummy_printf, yylval)) {
        Parse(pParser, tokenID, yylval, &p);
        yylval.str_value = 0;
    }
    Parse(pParser, 0, yylval, &p);
    ParseFree(pParser, free);
}

void parseInput(Program &p) {
    char *buffer;
    {
        size_t length = _binary_stdlib_bp_end - _binary_stdlib_bp_start;

        // read whole stdlib
        buffer = new char[length + 1];
        memcpy(buffer,_binary_stdlib_bp_start,length);
        buffer[length] = 0;

        parse(buffer,p);

        // free resources
        delete[] buffer;
    }
    {
        long length;

        // read whole stdin
        std::cin.seekg(0, std::ios::end);
        length = std::cin.tellg();
        std::cin.seekg(0, std::ios::beg);
        buffer = new char[length + 1];
        std::cin.read(buffer, length);
        buffer[length] = 0;

        parse(buffer,p);

        // free resources
        delete[] buffer;
    }
}

struct Region {
    std::vector<void*> heapAllocatedStuff;
};

extern "C" {

Region *NewRegion() {
    Region *r = new Region;
    //printf("New region created\n");
    return r;
}
char *RegionAlloc(Region *r, size_t size) {
    void *a = malloc(size);
    r->heapAllocatedStuff.push_back(a);
    return (char *) a;
}

void DeleteRegion(Region *r){
    for(void *a : r->heapAllocatedStuff) {
        free(a);
    }
    delete r;
    //printf("Region deleted\n");
}
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

int runJit(Program &p) {
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();
    LLVMLinkInMCJIT();

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
        register_regions(ctx);

        p.codegen(ctx);
    }

    // dump
    TheExecutionEngine->finalizeObject();

    // run
    Function *LF = TheModule->getFunction("main");
    void *FPtr = TheExecutionEngine->getPointerToFunction(LF);

    // Cast it to the right type (takes no arguments, returns a double) so we
    // can call it as a native function.
    Region *region = NewRegion();
    int (*FP)(Region *) = (int (*)(Region *))(intptr_t)FPtr;
    int result = FP(region);
    DeleteRegion(region);

    return result;
}

int main(int argc, char **argv) {
    Program p;
    parseInput(p);
    return runJit(p);
}