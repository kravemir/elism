/**
 * @author Miroslav Kravec
 */

#ifndef BP_LEXER_H_H
#define BP_LEXER_H_H

#include <string>
#include "parser.h"

typedef struct {
    int int_value;
    const char *str_value;
} YYSTYPE;

int lex(const char * &YYCURSOR, YYSTYPE &yylval);

#endif //BP_LEXER_H_H
