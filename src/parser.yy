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
    printf("Matched function\n");
}

function ::= FN IDENTIFIER LPAREN RPAREN statement_block. {
}

statement_block ::= LBRACE statement RBRACE. {
}

statement ::= RETURN NUMBER TERM. {
}
