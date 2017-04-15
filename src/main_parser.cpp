/**
 * @author Miroslav Kravec
 */

#include <iostream>

#include <lexer.h>
#include <parser.cpp>
#include "lexer.h"

int main(int argc, char **argv)
{
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
    while(tokenID = lex(buffer_ptr,yylval)) {
        Parse(pParser, tokenID, yylval, &p);
        yylval.str_value = 0;
    }
    Parse(pParser, 0, yylval, &p);
    ParseFree(pParser, free);

    Printer printer;
    p.print(printer);

    // free resources
    delete[] buffer;

    return 0;
}