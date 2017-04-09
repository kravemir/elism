//
// Created by miroslav on 4/6/17.
//

#include "WhileStatementNode.h"

using namespace llvm;

WhileStatementNode::WhileStatementNode(ExprNode *condition, StatementNode *statement) : condition(condition),
                                                                                        statement(statement) {}

void WhileStatementNode::print(Printer &printer) const {

}

void WhileStatementNode::codegen(CodegenContext &context) {

    Function *TheFunction = context.builder.GetInsertBlock()->getParent();

    // create blocks
    BasicBlock *ConditionBB = BasicBlock::Create(context.llvmContext, "while.condition");
    BasicBlock *ContentBB = BasicBlock::Create(context.llvmContext, "while.content");
    BasicBlock *ContinueBB = BasicBlock::Create(context.llvmContext, "while.continue");

    context.builder.CreateBr(ConditionBB);

    TheFunction->getBasicBlockList().push_back(ConditionBB);
    context.builder.SetInsertPoint(ConditionBB);
    CodegenValue *cond = condition->codegen(context);
    context.builder.CreateCondBr(cond->value, ContentBB, ContinueBB);

    TheFunction->getBasicBlockList().push_back(ContentBB);
    context.builder.SetInsertPoint(ContentBB);
    {
        ChildCodegenContext cycleContext(context);
        statement->codegen(cycleContext);
    }
    context.builder.CreateBr(ConditionBB);

    TheFunction->getBasicBlockList().push_back(ContinueBB);
    context.builder.SetInsertPoint(ContinueBB);
}
