#include <iostream>

#include <lexer.h>
#include <parser.cpp>

static int dummy_printf(const char *__restrict __format, ...) {
    return 0;
}

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
    YYSTYPE yylval;
    void *pParser = ParseAlloc(malloc);
    int tokenID;
    while(tokenID = lex(buffer_ptr,dummy_printf,yylval)) {
        Parse(pParser, tokenID, yylval);
    }
    Parse(pParser, 0, yylval);
    ParseFree(pParser, free);

    // free resources
    delete[] buffer;

    return 0;
}