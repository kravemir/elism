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

#include "ast/all.h"

std::string tokenToString(YYSTYPE &yystype) {
    if(yystype.str_value) {
        std::string val = yystype.str_value;
        delete[] yystype.str_value;
        return val;
    }
    return "";
}

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
statement_list(SL) ::= statement_list(SL_old) statement(S) SEMICOLON. {
    SL_old->push_back(S);
    SL = SL_old;
}

statement_list(SL) ::= statement(S) SEMICOLON. {
    SL = new std::vector<StatementNode*>(1,S);
}

%type statement { StatementNode* }
statement(S) ::= RETURN expr(E). {
    S = new ReturnStatementNode(E);
}

statement(S) ::= LET IDENTIFIER(NAME) ASSIGN expr(VALUE). {
    S = new LetStatementNode(tokenToString(NAME),VALUE);
}

statement(S) ::= VAR IDENTIFIER(NAME) ASSIGN expr(VALUE). {
    S = new VarStatementNode(tokenToString(NAME),0,VALUE);
}

statement(S) ::= IDENTIFIER(NAME) ASSIGN expr(VALUE). {
    S = new AssignStatementNode(tokenToString(NAME),VALUE);
}

%type expr { ExprNode* }
expr(E) ::= add_expr(E_). { E = E_; }

%type add_expr { ExprNode* }
add_expr(E) ::= add_expr(E1) PLUS mult_expr(E2). {
    E = new BinaryOperationExprNode('+', E1,E2);
}
add_expr(E) ::= add_expr(E1) MINUS mult_expr(E2). {
    E = new BinaryOperationExprNode('-', E1,E2);
}
add_expr(E) ::= mult_expr(E1). { E = E1; }

%type mult_expr { ExprNode* }
mult_expr(E) ::= mult_expr(E1) MULTIPLY atom(E2). {
    E = new BinaryOperationExprNode('*', E1,E2);
}
mult_expr(E) ::= mult_expr(E1) DIVIDE atom(E2). {
    E = new BinaryOperationExprNode('/', E1,E2);
}
mult_expr(E) ::= atom(E1). { E = E1; }

%type atom { ExprNode* }
atom(E) ::= NUMBER(N). {
    E = new ConstIntExprNode(N.int_value);
}
atom(E) ::= IDENTIFIER(NAME). {
    E = new NameExprNode(tokenToString(NAME));
}
atom(E) ::= LPAREN expr(E1) RPAREN. {
    E = E1;
}