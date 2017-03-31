//
// Created by miroslav on 3/28/17.
//

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
    Value* value = expr->codegen(context);
    context.addVariable(name,value);
}
