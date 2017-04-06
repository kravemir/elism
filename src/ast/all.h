//
// Created by miroslav on 3/31/17.
//

#ifndef BP_AST_ALL_H
#define BP_AST_ALL_H

#include "Program.h"
#include "StatementNode.h"
#include "TypeNode.h"
#include "ExprNode.h"

#include "expressions/BinaryOperationExprNode.h"
#include "expressions/ConstIntExprNode.h"
#include "expressions/NameExprNode.h"
#include "expressions/CallExprNode.h"
#include "expressions/ChildExprNode.h"
#include "expressions/StringExprNode.h"
#include "expressions/ArrayExprNode.h"

#include "statements/AssignStatementNode.h"
#include "statements/LetStatementNode.h"
#include "statements/ReturnStatementNode.h"
#include "statements/VarStatementNode.h"
#include "statements/ExprStatementNode.h"
#include "statements/BlockStatementNode.h"
#include "statements/IfStatementNode.h"

#include "declarations/FunctionNode.h"
#include "declarations/ClassNode.h"

#include "types/ArrayTypeNode.h"
#include "types/NamedTypeNode.h"

#endif //BP_AST_ALL_H
