/**
 * @author Miroslav Kravec
 */

%token_prefix TOKEN_

%token_type { YYSTYPE }

%include {
#include <iostream>
#include <vector>
#include <assert.h>

#include "parser.h"
#include "lexer.h"

#include "ast/StatementNode.h"
#include "ast/ReturnNode.h"

#include "ast/ExprNode.h"
#include "ast/IntValNode.h"

#include "ast/FunctionNode.h"
#include "ast/Program.h"
#include "ast/TypeNode.h"
#include "ast/NamedTypeNode.h"
#include "ast/ArrayTypeNode.h"
}

%token_destructor {
    if($$.str_value) {
        delete[] $$.str_value;
        $$.str_value = 0;
    }
}

%syntax_error {
    fprintf(stderr, "Syntax error\n");
}

%parse_failure {
    fprintf(stderr,"Giving up.  Parser is hopelessly lost...\n");
}

%extra_argument { Program *p }

%start_symbol program

program ::= top_level_nodes.

top_level_nodes ::= top_level_nodes top_level_node.
top_level_nodes ::= top_level_node.

top_level_node ::= function(F). {
    p->addFunction(F);
}

%type function { FunctionNode* }
function(F) ::= FN IDENTIFIER(NAME) fn_arg_def(ARGS) BEAK type_def(RETURN_TYPE) statement_block(SB). {
    F = new FunctionNode(NAME.str_value,RETURN_TYPE,std::move(*ARGS),std::move(*SB));
    delete SB;
    delete ARGS;
}

function(F) ::= FN IDENTIFIER(NAME) fn_arg_def(ARGS) statement_block(SB). {
    F = new FunctionNode(NAME.str_value,0,std::move(*ARGS),std::move(*SB));
    delete SB;
    delete ARGS;
}

%type fn_arg_def { std::vector<std::pair<std::string,TypeNode*>>* }
fn_arg_def(FAD) ::= LPAREN arg_list(AL) RPAREN. {
    FAD = AL;
}
fn_arg_def(FAD) ::= LPAREN RPAREN. {
    FAD = new std::vector<std::pair<std::string,TypeNode*>>();;
}

%type type_def { TypeNode* }
%destructor type_def { if($$) delete $$; }
type_def(TD) ::= type_def(TD_) LBRACKET RBRACKET. {
    TD = new ArrayTypeNode(TD_);
}

type_def(TD) ::= IDENTIFIER(id). {
    TD = new NamedTypeNode(id.str_value);
    delete[] id.str_value;
}

%type arg_list { std::vector<std::pair<std::string,TypeNode*>>* }
arg_list(AL) ::= arg_def(AD) . {
    AL = new std::vector<std::pair<std::string,TypeNode*>>();
    AL->push_back(std::move(*AD));
    delete AD;
}
arg_list(AL) ::= arg_list(AL_) COMMA arg_def(AD). {
    AL = AL_;
    AL->push_back(std::move(*AD));
    delete AD;
}

%type arg_def { std::pair<std::string,TypeNode*>* }
arg_def(AD) ::= IDENTIFIER(NAME) COLON type_def(TD). {
    AD = new std::pair<std::string,TypeNode*>(std::string(NAME.str_value),TD);
    delete[] NAME.str_value;
}

%type statement_block { std::vector<StatementNode*>* }
statement_block(SB) ::= LBRACE statement_list(SL) RBRACE. {
    SB = SL;
}

%type statement_list { std::vector<StatementNode*>* }
statement_list(SL) ::= statement_list(SL_old) SEMICOLON statement(S). {
    SL_old->push_back(S);
    SL = SL_old;
}

statement_list(SL) ::= statement(S) SEMICOLON. {
    SL = new std::vector<StatementNode*>(1,S);
}

%type statement { StatementNode* }
statement(S) ::= RETURN expr(E). {
    S = new ReturnNode(E);
}

%type expr { ExprNode* }
expr(E) ::= NUMBER(N). {
    E = new IntValNode(N.int_value);
}