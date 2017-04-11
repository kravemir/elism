/**
 * @author Miroslav Kravec
 */

#include "Program.h"

void Program::addFunction(FunctionNode *function) {
    functions.push_back(function);
}

void Program::addClass(ClassNode *cls) {
    classes.push_back(cls);
}

void Program::print(Printer &printer) {
    for(ClassNode *cls : classes) {
        cls->print(printer);
    }
    for(FunctionNode *fn : functions) {
        fn->print(printer);
    }
}

Program::~Program() {
    for(ClassNode *cls : classes) {
        delete cls;
    }
    for(FunctionNode *fn : functions) {
        delete fn;
    }
}

void Program::codegen(CodegenContext &ctx) {
    for(ClassNode *cls : classes) {
        cls->codegen(ctx);
    }
    for(FunctionNode *fn : functions) {
        fn->codegen(ctx);
    }
}
