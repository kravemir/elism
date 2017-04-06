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

top_level_node ::= class(C). {
    p->addClass(C);
}

%type class { ClassNode* }
class(C) ::= CLASS IDENTIFIER(NAME) LBRACE class_statements(CS) RBRACE. {
    C = new ClassNode(NAME.str_value,std::move(*CS));
    delete CS;
}

%type class_statements { std::vector<VarStatementNode*>* }
class_statements(CS) ::= . { CS = new std::vector<VarStatementNode*>(); }
class_statements(CS) ::= class_statements(CS_) VAR IDENTIFIER(NAME) ASSIGN expr(VALUE) SEMICOLON. {
  CS = CS_;
  CS->push_back(new VarStatementNode(tokenToString(NAME),0,VALUE));
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
statement_list(SL) ::= statement_list(SL_old) statement(S). {
    SL_old->push_back(S);
    SL = SL_old;
}

statement_list(SL) ::= statement(S). {
    SL = new std::vector<StatementNode*>(1,S);
}

%type statement { StatementNode* }
statement(S) ::= RETURN expr(E) SEMICOLON. {
    S = new ReturnStatementNode(E);
}

statement(S) ::= LET IDENTIFIER(NAME) ASSIGN expr(VALUE) SEMICOLON. {
    S = new LetStatementNode(tokenToString(NAME),VALUE);
}

statement(S) ::= VAR IDENTIFIER(NAME) ASSIGN expr(VALUE) SEMICOLON. {
    S = new VarStatementNode(tokenToString(NAME),0,VALUE);
}

statement(S) ::= expr(TARGET) ASSIGN expr(VALUE) SEMICOLON. {
    S = new AssignStatementNode(TARGET,VALUE);
}

statement(S) ::= expr(E) SEMICOLON. {
    S = new ExprStatementNode(E);
}

statement(S) ::= WHILE LPAREN expr(E) RPAREN statement(S_). {
    S = new WhileStatementNode(E,S_);
}

statement(S) ::= IF LPAREN expr(E) RPAREN statement(S_). {
    S = new IfStatementNode(E, S_);
}

statement(S) ::= statement_block(SB). {
    S = new BlockStatementNode(*SB);
}

%type expr { ExprNode* }
expr(E) ::= comparison(E_). { E = E_; }

%type comparison { ExprNode* }
comparison(E) ::= add_expr(E_). { E = E_; }

comparison(E) ::= comparison(E1) EQUALS add_expr(E2). {
    E = new BinaryOperationExprNode(TOKEN_EQUALS, E1, E2);
}
comparison(E) ::= comparison(E1) LT add_expr(E2). {
    E = new BinaryOperationExprNode(TOKEN_LT, E1, E2);
}
comparison(E) ::= comparison(E1) GT add_expr(E2). {
    E = new BinaryOperationExprNode(TOKEN_GT, E1, E2);
}
comparison(E) ::= comparison(E1) LE add_expr(E2). {
    E = new BinaryOperationExprNode(TOKEN_LE, E1, E2);
}
comparison(E) ::= comparison(E1) GE add_expr(E2). {
    E = new BinaryOperationExprNode(TOKEN_GE, E1, E2);
}

%type add_expr { ExprNode* }
add_expr(E) ::= add_expr(E1) PLUS mult_expr(E2). {
    E = new BinaryOperationExprNode('+', E1,E2);
}
add_expr(E) ::= add_expr(E1) MINUS mult_expr(E2). {
    E = new BinaryOperationExprNode('-', E1,E2);
}
add_expr(E) ::= mult_expr(E1). { E = E1; }

%type mult_expr { ExprNode* }
mult_expr(E) ::= mult_expr(E1) MULTIPLY atom_expr(E2). {
    E = new BinaryOperationExprNode('*', E1,E2);
}
mult_expr(E) ::= mult_expr(E1) DIVIDE atom_expr(E2). {
    E = new BinaryOperationExprNode('/', E1,E2);
}
mult_expr(E) ::= atom_expr(E1). { E = E1; }

%type atom_expr { ExprNode* }
atom_expr(AE) ::= atom(A). {
    AE = A;
}

atom_expr(AE) ::= atom_expr(AE_) LPAREN call_args(CA) RPAREN. {
    AE = new CallExprNode(AE_, *CA);
}

atom_expr(AE) ::= atom_expr(AE_) LPAREN RPAREN. {
    AE = new CallExprNode(AE_, {});
}

atom_expr(AE) ::= atom_expr(AE_) LBRACKET expr(IDX) RBRACKET. {
    AE = new ArrayElementExprNode(AE_, IDX);
}

atom_expr(AE) ::= atom_expr(AE_) DOT IDENTIFIER(CHILD_NAME). {
    AE = new ChildExprNode(AE_, tokenToString(CHILD_NAME));
}

%type atom { ExprNode* }
atom(E) ::= NUMBER(N). {
    E = new ConstIntExprNode(N.int_value);
}
atom(E) ::= STRING(NAME). {
    E = new StringExprNode(tokenToString(NAME));
}
atom(E) ::= IDENTIFIER(NAME). {
    E = new NameExprNode(tokenToString(NAME));
}
atom(E) ::= LPAREN expr(E1) RPAREN. {
    E = E1;
}
atom(E) ::= LBRACKET call_args(CA) RBRACKET. {
    E = new ArrayLiteralExprNode(*CA);
}
atom(E) ::= LBRACKET expr(E_) SEMICOLON NUMBER(COUNT) RBRACKET. {
    E = new ArrayInitializerExprNode(E_, COUNT.int_value);
}

%type call_args { std::vector<ExprNode*>* }
call_args(CA) ::= expr(E). {
    CA = new std::vector<ExprNode*>();
    CA->push_back(E);
}

call_args(CA) ::= call_args(CA_) COMMA expr(E). {
    CA = CA_;
    CA->push_back(E);
}