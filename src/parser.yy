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

template<typename T>
T moveDelete(T* val) {
    T v = std::move(*val);
    delete val;
    return v;
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
    C = new ClassNode(tokenToString(NAME),moveDelete(CS));
}
class(C) ::= CLASS IDENTIFIER(NAME) COLON IDENTIFIER(SUPER) LBRACE class_statements(CS) RBRACE. {
    C = new ClassNode(tokenToString(NAME),tokenToString(SUPER),moveDelete(CS));
}

%type class_statements { std::vector<StatementNode*>* }
class_statements(CS) ::= . { CS = new std::vector<StatementNode*>(); }
class_statements(CS) ::= class_statements(CS) VAR IDENTIFIER(NAME) ASSIGN expr(VALUE) SEMICOLON. {
  CS->push_back(new VarStatementNode(tokenToString(NAME),0,VALUE));
}
class_statements(CS) ::= class_statements(CS) VAR IDENTIFIER(NAME) COLON type_def(TYPE) SEMICOLON. {
  CS->push_back(new VarStatementNode(tokenToString(NAME),TYPE,0));
}
class_statements(CS) ::= class_statements(CS) VAR IDENTIFIER(NAME) COLON type_def(TYPE) ASSIGN expr(VALUE) SEMICOLON. {
  CS->push_back(new VarStatementNode(tokenToString(NAME),TYPE,VALUE));
}
class_statements(CS) ::= class_statements(CS) FN IDENTIFIER(NAME) region_decl(RD) fn_arg_def(ARGS) statement_block(SB). {
  CS->push_back(new FunctionNode(tokenToString(NAME),new NamedTypeNode("void",{}),moveDelete(RD),std::move(*ARGS),std::move(*SB)));
}
class_statements(CS) ::= class_statements(CS) FN IDENTIFIER(NAME) region_decl(RD) fn_arg_def(ARGS) BEAK type_def(RETURN_TYPE) statement_block(SB). {
  CS->push_back(new FunctionNode(tokenToString(NAME),RETURN_TYPE,moveDelete(RD),std::move(*ARGS),std::move(*SB)));
}

%type function { FunctionNode* }
function(F) ::= FN IDENTIFIER(NAME) region_decl(RD) fn_arg_def(ARGS) BEAK type_def(RETURN_TYPE) statement_block(SB). {
    F = new FunctionNode(tokenToString(NAME),RETURN_TYPE,moveDelete(RD),moveDelete(ARGS),moveDelete(SB));
}

function(F) ::= FN IDENTIFIER(NAME) region_decl(RD) fn_arg_def(ARGS) statement_block(SB). {
    F = new FunctionNode(tokenToString(NAME),new NamedTypeNode("void",{}),moveDelete(RD),moveDelete(ARGS),moveDelete(SB));
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

type_def(TD) ::= type_def(TD_) LBRACKET RBRACKET region_decl(RD). {
    TD = new ArrayTypeNode(TD_,moveDelete(RD));
}

type_def(TD) ::= IDENTIFIER(id) region_decl(RD). {
    TD = new NamedTypeNode(tokenToString(id),moveDelete(RD));
}

%type region_decl { std::vector<std::string>* }
%type region_list { std::vector<std::string>* }
region_decl(RD) ::= . { RD = new std::vector<std::string>(); }
region_decl(RD) ::= region_list(RD).

region_list(RL) ::= AMPIDENTIFIER(I). { RL = new std::vector<std::string>(); RL->push_back(tokenToString(I)); }
region_list(RL) ::= region_list(RL) COMMA AMPIDENTIFIER(I). { RL->push_back(tokenToString(I)); }

%type arg_list { std::vector<std::pair<std::string,TypeNode*>>* }
arg_list(AL) ::= arg_def(AD) . {
    AL = new std::vector<std::pair<std::string,TypeNode*>>();
    AL->push_back(moveDelete(AD));
}
arg_list(AL) ::= arg_list(AL) COMMA arg_def(AD). {
    AL->push_back(moveDelete(AD));
}

%type arg_def { std::pair<std::string,TypeNode*>* }
arg_def(AD) ::= type_def(TD) IDENTIFIER(NAME). {
    AD = new std::pair<std::string,TypeNode*>(tokenToString(NAME),TD);
}

%type statement_block { std::vector<StatementNode*>* }
statement_block(SB) ::= LBRACE statement_list(SL) RBRACE. {
    SB = SL;
}

%type statement_list { std::vector<StatementNode*>* }
statement_list(SL) ::= statement_list(SL) statement(S). {
    SL->push_back(S);
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
statement(S) ::= LET IDENTIFIER(NAME) COLON type_def ASSIGN expr(VALUE) SEMICOLON. {
    S = new LetStatementNode(tokenToString(NAME),VALUE);
}
statement(S) ::= VAR IDENTIFIER(NAME) ASSIGN expr(VALUE) SEMICOLON. {
    S = new VarStatementNode(tokenToString(NAME),0,VALUE);
}
statement(S) ::= VAR IDENTIFIER(NAME) COLON type_def ASSIGN expr(VALUE) SEMICOLON. {
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
statement(S) ::= FOR LPAREN IDENTIFIER(NAME) COLON expr(E) RPAREN statement(S_). {
    S = new ForStatementNode(tokenToString(NAME),E,S_);
}
statement(S) ::= ON IDENTIFIER(NAME) ASSIGN expr(E) statement_block(S_). {
    S = new OnStatementNode(tokenToString(NAME),E,new BlockStatementNode(moveDelete(S_)));
}

%nonassoc IF.
%nonassoc ELSE.

statement(S) ::= IF LPAREN expr(E) RPAREN statement(S1) ELSE statement(S2). {
    S = new IfStatementNode(E, S1, S2);
}
statement(S) ::= IF LPAREN expr(E) RPAREN statement(S_). {
    S = new IfStatementNode(E, S_, nullptr);
}
statement(S) ::= statement_block(SB). {
    S = new BlockStatementNode(moveDelete(SB));
}

%type expr { ExprNode* }
expr(E) ::= comparison(E).

%type comparison { ExprNode* }
comparison(E) ::= add_expr(E).
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
add_expr(E) ::= mult_expr(E).
add_expr(E) ::= add_expr(E1) PLUS mult_expr(E2). {
    E = new BinaryOperationExprNode('+', E1,E2);
}
add_expr(E) ::= add_expr(E1) MINUS mult_expr(E2). {
    E = new BinaryOperationExprNode('-', E1,E2);
}

%type mult_expr { ExprNode* }
mult_expr(E) ::= atom_expr(E).
mult_expr(E) ::= mult_expr(E1) MULTIPLY atom_expr(E2). {
    E = new BinaryOperationExprNode('*', E1,E2);
}
mult_expr(E) ::= mult_expr(E1) DIVIDE atom_expr(E2). {
    E = new BinaryOperationExprNode('/', E1,E2);
}

%type atom_expr { ExprNode* }
atom_expr(A) ::= atom(A).
atom_expr(AE) ::= atom_expr(AE_) region_decl(RD) LPAREN call_args(CA) RPAREN. {
    AE = new CallExprNode(AE_, moveDelete(RD), *CA);
}
atom_expr(AE) ::= atom_expr(AE_) region_decl(RD) LPAREN RPAREN. {
    AE = new CallExprNode(AE_, moveDelete(RD), {});
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
atom(E) ::= LBRACKET expr(E_) SEMICOLON expr(COUNT) RBRACKET. {
    E = new ArrayInitializerExprNode(E_, COUNT);
}

%type call_args { std::vector<ExprNode*>* }
call_args(CA) ::= expr(E). {
    CA = new std::vector<ExprNode*>();
    CA->push_back(E);
}

call_args(CA) ::= call_args(CA) COMMA expr(E). {
    CA->push_back(E);
}