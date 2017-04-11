//
// Created by miroslav on 3/28/17.
//

#include "AssignStatementNode.h"

using namespace llvm;

AssignStatementNode::AssignStatementNode(ExprNode *const targetExpr, ExprNode *const valueExpr)
        : targetExpr(targetExpr),
          valueExpr(valueExpr)
{}

void AssignStatementNode::print(Printer &printer) const {
    printer.println("TODO");
}

void AssignStatementNode::codegen(CodegenContext &context) {
    CodegenValue* target = targetExpr->codegen(context);
    CodegenValue* value = valueExpr->codegen(context);
    target->doStore(context,value);
}

AssignStatementNode::~AssignStatementNode() {
    if(targetExpr)
        delete targetExpr;
    if(valueExpr)
        delete valueExpr;
}
