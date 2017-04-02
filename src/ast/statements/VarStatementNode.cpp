//
// Created by miroslav on 3/28/17.
//

#include <ast/expressions/NameExprNode.h>
#include "VarStatementNode.h"

using namespace llvm;

VarStatementNode::VarStatementNode(const std::string &name,
                 const TypeNode *type,
                 ExprNode *const expr)
        : name(name),
          type(type),
          expr(expr)
{
    assert(expr);
}

void VarStatementNode::print(Printer &printer) const {
    printer.print("var " + name + " = " + expr->toString() + ";" );
}

void VarStatementNode::codegen(CodegenContext &context) {
    CodegenValue* value = expr->codegen(context);
    assert(value);
    context.addVariable(name,value);
}
