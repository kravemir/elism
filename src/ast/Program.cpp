/**
 * @author Miroslav Kravec
 */

#include "Program.h"

void Program::addFunction(FunctionNode *function) {
    functions.push_back(function);
}

void Program::print(Printer &printer) {
    for(FunctionNode *fn : functions) {
        fn->print(printer);
    }
}

Program::~Program() {
    for(FunctionNode *fn : functions) {
        delete fn;
    }
}
