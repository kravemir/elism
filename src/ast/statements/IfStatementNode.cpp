//
// Created by miroslav on 4/6/17.
//

#include <ast/ExprNode.h>
#include <ast/StatementNode.h>
#include "IfStatementNode.h"

using namespace llvm;

IfStatementNode::IfStatementNode(ExprNode *conditionExpr, StatementNode *statement) : conditionExpr(conditionExpr),
                                                                                      statement(statement) {}

void IfStatementNode::print(Printer &printer) const {
    printer.println("TODO");
}

void IfStatementNode::codegen(CodegenContext &context) {
    Function *TheFunction = context.builder.GetInsertBlock()->getParent();

    // create blocks
    BasicBlock *ThenBB = BasicBlock::Create(context.llvmContext, "if.then", TheFunction);
    BasicBlock *ContinueBB = BasicBlock::Create(context.llvmContext, "if.continue");
    
    CodegenValue *cond = conditionExpr->codegen(context);
    context.builder.CreateCondBr(cond->value, ThenBB, ContinueBB);
    
    context.builder.SetInsertPoint(ThenBB);
    statement->codegen(context);
    context.builder.CreateBr(ContinueBB);
    
    context.builder.SetInsertPoint(ContinueBB);
    TheFunction->getBasicBlockList().push_back(ContinueBB);
}
