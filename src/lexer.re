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
    str[oi] = 0;
    return str;
}

int lex(const char * &YYCURSOR, YYSTYPE &yylval)
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
        "//" [^\n]* "\n" { goto SKIP_WS; }

        bin { return TOKEN_NUMBER; }
        dec {

            yylval.int_value = 0;
            while(tok<YYCURSOR){ yylval.int_value = (yylval.int_value * 10) + (*(tok++) - '0'); }
            return TOKEN_NUMBER;
        }
        oct {

            yylval.int_value = 0;
            while(tok<YYCURSOR){ yylval.int_value = (yylval.int_value << 3) + (*(tok++) - '0'); }
            return TOKEN_NUMBER;
        }
        hex { return TOKEN_NUMBER; }

        "fn"        { return TOKEN_FN; }
        "return"    { return TOKEN_RETURN; }
        "let"       { return TOKEN_LET; }
        "var"       { return TOKEN_VAR; }
        "class"     { return TOKEN_CLASS; }
        "if"        { return TOKEN_IF; }
        "else"        { return TOKEN_ELSE; }
        "while"        { return TOKEN_WHILE; }
        "for"        { return TOKEN_FOR; }
        "on"        { return TOKEN_ON; }

        [a-zA-Z][_a-zA-Z0-9]* {

            char *val = new char[YYCURSOR-tok+1];
            memcpy(val,tok,YYCURSOR-tok);
            val[YYCURSOR-tok] = 0;
            yylval.str_value = val;
            return TOKEN_IDENTIFIER;
        }

        "+"  { return TOKEN_PLUS; }
        "-"  { return TOKEN_MINUS; }
        "*"  { return TOKEN_MULTIPLY; }
        "/"  { return TOKEN_DIVIDE; }

        "="  { return TOKEN_ASSIGN; }
        "."  { return TOKEN_DOT; }
        ","  { return TOKEN_COMMA; }

        "=="  { return TOKEN_EQUALS; }
        "<"  { return TOKEN_LT; }
        ">"  { return TOKEN_GT; }
        "<="  { return TOKEN_LE; }
        ">="  { return TOKEN_GE; }

        "("  { return TOKEN_LPAREN; }
        ")"  { return TOKEN_RPAREN; }
        "{"  { return TOKEN_LBRACE; }
        "}"  { return TOKEN_RBRACE; }
        "["  { return TOKEN_LBRACKET; }
        "]"  { return TOKEN_RBRACKET; }
        ":"  { return TOKEN_COLON; }
        ";"  { return TOKEN_SEMICOLON; }
        "->" { return TOKEN_BEAK; }

        "\"" ( "\\". | [^"] )* "\"" {

            char *val = new char[YYCURSOR-tok];
            memcpy(val,tok+1,YYCURSOR-tok-2);
            val[YYCURSOR-tok-2] = 0;
            yylval.str_value = make_str(val,YYCURSOR-tok-2);
            return TOKEN_STRING;
        }

        end  { return 0; }
        [^]  { return 0; }
    */
}

#endif // LEXER_H
