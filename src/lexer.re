/**
 * @author Miroslav Kravec
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <cstring>
#include "lexer.h"

const char * make_str(const char *src, int len){
    char *str = new char[len+1];
    int oi = 0;
    for(int i = 0; i < len; i++) {
        if(src[i] == '\\' && src[i+1] == 'n') {
            str[oi++] = '\n';
            i++;
        } else {
            str[oi++] = src[i];
        }
    }
    return str;
}

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
        dec {
            printf ("       dec: %.*s\n", (int)(YYCURSOR - tok), tok);
            yylval.int_value = 0;
            while(tok<YYCURSOR){ yylval.int_value = (yylval.int_value * 10) + (*(tok++) - '0'); }
            return TOKEN_NUMBER;
        }
        oct {
            printf ("       oct: %.*s\n", (int)(YYCURSOR - tok), tok);
            yylval.int_value = 0;
            while(tok<YYCURSOR){ yylval.int_value = (yylval.int_value << 3) + (*(tok++) - '0'); }
            return TOKEN_NUMBER;
        }
        hex { printf ("       hex: %.*s\n", (int)(YYCURSOR - tok), tok); return TOKEN_NUMBER; }

        "fn"        { printf ("   keyword: fn\n"); return TOKEN_FN; }
        "return"    { printf ("   keyword: return\n"); return TOKEN_RETURN; }
        "let"       { printf ("   keyword: ket\n"); return TOKEN_LET; }
        "var"       { printf ("   keyword: var\n"); return TOKEN_VAR; }
        "class"     { printf ("   keyword: class\n"); return TOKEN_CLASS; }
        "if"        { printf ("   keyword: if\n"); return TOKEN_IF; }

        [a-zA-Z][a-zA-Z0-9]* {
            printf ("identifier: %.*s\n", (int)(YYCURSOR - tok), tok);
            char *val = new char[YYCURSOR-tok+1];
            memcpy(val,tok,YYCURSOR-tok);
            val[YYCURSOR-tok] = 0;
            yylval.str_value = val;
            return TOKEN_IDENTIFIER;
        }

        "+"  { printf ("    symbol: (\n"); return TOKEN_PLUS; }
        "-"  { printf ("    symbol: (\n"); return TOKEN_MINUS; }
        "*"  { printf ("    symbol: (\n"); return TOKEN_MULTIPLY; }
        "/"  { printf ("    symbol: (\n"); return TOKEN_DIVIDE; }

        "="  { printf ("    symbol: (\n"); return TOKEN_ASSIGN; }
        "."  { printf ("    symbol: (\n"); return TOKEN_DOT; }
        ","  { printf ("    symbol: (\n"); return TOKEN_COMMA; }

        "=="  { printf ("    symbol: (\n"); return TOKEN_EQUALS; }
        "<"  { printf ("    symbol: (\n"); return TOKEN_LT; }
        ">"  { printf ("    symbol: (\n"); return TOKEN_GT; }
        "<="  { printf ("    symbol: (\n"); return TOKEN_LE; }
        ">="  { printf ("    symbol: (\n"); return TOKEN_GE; }

        "("  { printf ("    symbol: (\n"); return TOKEN_LPAREN; }
        ")"  { printf ("    symbol: )\n"); return TOKEN_RPAREN; }
        "{"  { printf ("    symbol: {\n"); return TOKEN_LBRACE; }
        "}"  { printf ("    symbol: }\n"); return TOKEN_RBRACE; }
        "["  { printf ("    symbol: {\n"); return TOKEN_LBRACKET; }
        "]"  { printf ("    symbol: }\n"); return TOKEN_RBRACKET; }
        ":"  { printf ("    symbol: ;\n"); return TOKEN_COLON; }
        ";"  { printf ("    symbol: ;\n"); return TOKEN_SEMICOLON; }
        "->" { printf ("    symbol: ->\n"); return TOKEN_BEAK; }

        "\"" ( "\\". | [^"] )* "\"" {
            printf ("    symbol: STRING\n");
            char *val = new char[YYCURSOR-tok];
            memcpy(val,tok+1,YYCURSOR-tok-2);
            val[YYCURSOR-tok-2] = 0;
            yylval.str_value = make_str(val,YYCURSOR-tok-2);
            return TOKEN_STRING;
        }

        end  { printf ("       end: success\n"); return 0; }
        [^]  { printf ("       end: error\n"); return 0; }
    */
}

#endif // LEXER_H
