/**
 * @author Miroslav Kravec
 */

#ifndef BP_STATEMENTNODE_H
#define BP_STATEMENTNODE_H

#include <iostream>
#include <CodegenContext.h>
#include "ast/ExprNode.h"

struct Printer {
    void print(const std::string &str) {
        if(newline) {
            std::cout << indentStrCurrent;
            newline = false;
        }
        std::cout << str;
    }
    void println(const std::string &str) {
        if(newline) {
            std::cout << indentStrCurrent;
        }
        std::cout << str << std::endl;
        newline = true;
    }
    void beginBlock() {
        if(newline) {
            std::cout << indentStrCurrent;
            newline = false;
        }
        setIndent(indent+4);
        std::cout << "{\n";
        newline = true;
    }
    void endBlock() {
        setIndent(indent-4);
        std::cout << indentStrCurrent << "}\n";
        newline = true;
    }

    Printer() {
        indentCapacity = 32;
        indentStr = (char *) malloc(indentCapacity + 1);
        for(int i = 0; i < indentCapacity; i++)
            indentStr[i] = ' ';
        indentStr[indentCapacity] = 0;
    }

    ~Printer() {
        free(indentStr);
    }

    void setIndent(int i) {
        while(i > indentCapacity) {
            indentCapacity *= 4;
            free(indentStr);
            indentStr = (char *) malloc(indentCapacity);
            for(int i = 0; i < indentCapacity; i++)
                indentStr[i] = ' ';
            indentStr[indentCapacity] = 0;
        }
        indentStrCurrent = &indentStr[indentCapacity - i];
        indent = i;
    }

    bool newline = false;
    int indent = 0, indentCapacity = 0;
    char *indentStr = 0, *indentStrCurrent;
};

class StatementNode {
public:
    virtual void print(Printer &printer) const = 0;
    virtual ~StatementNode();

    virtual void codegen(CodegenContext &context) = 0;
};

StatementNode* createReturnStatementNode(ExprNode *expr);


#endif //BP_STATEMENTNODE_H
