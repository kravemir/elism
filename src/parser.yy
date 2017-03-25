%token_prefix TOKEN_

%token_type { YYSTYPE }

%include {
#include <iostream>
#include <assert.h>

#include "parser.h"
#include "lexer.h"
}

%syntax_error {
    fprintf(stderr, "Syntax error\n");
}

%parse_failure {
    fprintf(stderr,"Giving up.  Parser is hopelessly lost...\n");
}

%start_symbol program

program ::= function. {
    printf("Matched program\n");
}

function ::= FN IDENTIFIER fn_arg_def BEAK type_def statement_block. {
    printf("Matched function\n");
}

function ::= FN IDENTIFIER fn_arg_def statement_block. {
    printf("Matched function\n");
}

fn_arg_def ::= LPAREN arg_list RPAREN. {
    printf("Matched function arguments def\n");
}

type_def ::= type_def LBRACKET RBRACKET. {
    printf("Matched array type def\n");
}

type_def ::= IDENTIFIER(id). {
    printf("Matched type def: %s\n", id.str_value);
}

arg_list ::= arg_def COMMA arg_list.
arg_list ::= arg_def .
arg_list ::= .

arg_def ::= IDENTIFIER COLON type_def.

statement_block ::= LBRACE statement_list RBRACE. {
    printf("Matched block\n");
}

statement_list ::= statement SEMICOLON statement_list. {
    printf("Matched statement list\n");
}

statement_list ::= statement SEMICOLON. {
    printf("Matched statement list\n");
}

statement ::= RETURN NUMBER. {
    printf("Matched return statement\n");
}
