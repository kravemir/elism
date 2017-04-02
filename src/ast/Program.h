/**
 * @author Miroslav Kravec
 */

#ifndef BP_PROGRAM_H
#define BP_PROGRAM_H


#include <vector>
#include <CodegenContext.h>
#include <ast/declarations/ClassNode.h>
#include "ast/declarations/FunctionNode.h"

class Program {
public:
    virtual ~Program();

    void addFunction(FunctionNode *function);
    void addClass(ClassNode *cls);

    void print(Printer &printer);

    void codegen(CodegenContext &ctx);

private:
    std::vector<FunctionNode*> functions;
    std::vector<ClassNode*> classes;
};


#endif //BP_PROGRAM_H
