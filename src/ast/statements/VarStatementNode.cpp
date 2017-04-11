//
// Created by miroslav on 3/28/17.
//

#include <ast/expressions/NameExprNode.h>
#include "VarStatementNode.h"

using namespace llvm;

VarStatementNode::VarStatementNode(const std::string &name,
                 TypeNode *type,
                 ExprNode *const expr)
        : name(name),
          type(type),
          expr(expr)
{
}

void VarStatementNode::codegenAsClassStatement(ClassTypeContext &context) {
    // TODO
}

void VarStatementNode::print(Printer &printer) const {
    printer.print("var " + name + " = " + expr->toString() + ";" );
}

void VarStatementNode::codegen(CodegenContext &context) {
    CodegenValue *value;
    CodegenType *type = 0;
    if(this->type != nullptr) {
        type = this->type->codegen(context);
        assert(type);
    }

    if(expr == nullptr) {
        assert(type);
        value = type->getDefault(context);
        assert(value);
    } else {
        value = expr->codegen(context);
        assert(value);
    }

    context.addVariable(name,value);
}
