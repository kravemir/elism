//
// Created by miroslav on 3/28/17.
//

#include "VarNode.h"

using namespace llvm;

VarNode::VarNode(const std::string &name,
                 const TypeNode *type,
                 ExprNode *const expr)
        : name(name),
          type(type),
          expr(expr)
{
    assert(expr);
}

void VarNode::print(Printer &printer) const {
    printer.print("var " + name + " = " + expr->toString() + ";" );
}

void VarNode::codegen(CodegenContext &context) {
    Value* value = expr->codegen(context);
    context.addVariable(name,value);
}
