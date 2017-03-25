#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <cstring>
#include "lexer.h"

int lex(const char * &YYCURSOR, int printf(const char *__restrict __format, ...), YYSTYPE &yylval)
{
    const char *tok = YYCURSOR;
    const char *YYMARKER;

SKIP_WS:;
    tok = YYCURSOR;
    /*!re2c
        re2c:define:YYCTYPE = char;
        re2c:yyfill:enable = 0;

        end = "\x00";
        bin = '0b' [01]+;
        oct = "0" [0-7]*;
        dec = [1-9][0-9]*;
        hex = '0x' [0-9a-fA-F]+;
        WS = " "|"\n";

        WS { goto SKIP_WS; }
        bin { printf ("       bin: %.*s\n", (int)(YYCURSOR - tok), tok); return TOKEN_NUMBER; }
        dec { printf ("       dec: %.*s\n", (int)(YYCURSOR - tok), tok); return TOKEN_NUMBER; }
        oct { printf ("       oct: %.*s\n", (int)(YYCURSOR - tok), tok); return TOKEN_NUMBER; }
        hex { printf ("       hex: %.*s\n", (int)(YYCURSOR - tok), tok); return TOKEN_NUMBER; }

        "fn"        { printf ("   keyword: fn\n"); return TOKEN_FN; }
        "return"    { printf ("   keyword: return\n"); return TOKEN_RETURN; }

        [a-zA-Z][a-zA-Z0-9]* {
            printf ("identifier: %.*s\n", (int)(YYCURSOR - tok), tok);
            char *val = new char[YYCURSOR-tok+1];
            memcpy(val,tok,YYCURSOR-tok);
            val[YYCURSOR-tok] = 0;
            yylval.str_value = val;
            return TOKEN_IDENTIFIER;
        }

        "("  { printf ("    symbol: (\n"); return TOKEN_LPAREN; }
        ")"  { printf ("    symbol: )\n"); return TOKEN_RPAREN; }
        "{"  { printf ("    symbol: {\n"); return TOKEN_LBRACE; }
        "}"  { printf ("    symbol: }\n"); return TOKEN_RBRACE; }
        "["  { printf ("    symbol: {\n"); return TOKEN_LBRACKET; }
        "]"  { printf ("    symbol: }\n"); return TOKEN_RBRACKET; }
        ":"  { printf ("    symbol: ;\n"); return TOKEN_COLON; }
        ";"  { printf ("    symbol: ;\n"); return TOKEN_SEMICOLON; }
        "->" { printf ("    symbol: ->\n"); return TOKEN_BEAK; }

        end  { printf ("       end: success\n"); return 0; }
        [^]  { printf ("       end: error\n"); return 0; }
    */
}

#endif // LEXER_H
