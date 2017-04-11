//
// Created by miroslav on 4/6/17.
//

#include <ast/ExprNode.h>
#include <ast/StatementNode.h>
#include "IfStatementNode.h"

using namespace llvm;

IfStatementNode::IfStatementNode(ExprNode *conditionExpr, StatementNode *then, StatementNode *el)
        : conditionExpr(conditionExpr),
          then (then),
          el(el)
{}

void IfStatementNode::print(Printer &printer) const {
    printer.println("TODO");
}

void IfStatementNode::codegen(CodegenContext &context) {
    Function *TheFunction = context.builder.GetInsertBlock()->getParent();

    // create blocks
    BasicBlock *ThenBB = BasicBlock::Create(context.llvmContext, "if.then", TheFunction);
    BasicBlock *ContinueBB = BasicBlock::Create(context.llvmContext, "if.continue");
    BasicBlock *ElseBB;

    if(el != nullptr) {
        ElseBB = BasicBlock::Create(context.llvmContext, "if.else");
    } else {
        ElseBB = ContinueBB;
    }
    
    CodegenValue *cond = conditionExpr->codegen(context);
    context.builder.CreateCondBr(cond->value, ThenBB, ElseBB);
    
    context.builder.SetInsertPoint(ThenBB);
    then->codegen(context);
    context.builder.CreateBr(ContinueBB);

    if(el != nullptr) {
        TheFunction->getBasicBlockList().push_back(ElseBB);
        context.builder.SetInsertPoint(ElseBB);
        el->codegen(context);
        context.builder.CreateBr(ContinueBB);
    }
    
    context.builder.SetInsertPoint(ContinueBB);
    TheFunction->getBasicBlockList().push_back(ContinueBB);
}
