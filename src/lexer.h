#ifndef BP_LEXER_H_H
#define BP_LEXER_H_H

#include "parser.h"

typedef union {
    int int_value;
} YYSTYPE;

int lex(const char * &YYCURSOR, int printf(const char *__restrict __format, ...));

#endif //BP_LEXER_H_H
