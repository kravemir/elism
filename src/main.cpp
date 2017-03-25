#include <iostream>

#include <lexer.h>

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
    while(lex(buffer_ptr,printf)) {}

    // free resources
    delete[] buffer;

    return 0;
}