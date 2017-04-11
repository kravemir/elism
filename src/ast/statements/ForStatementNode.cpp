//
// Created by miroslav on 4/7/17.
//

#include "ForStatementNode.h"

using namespace llvm;

ForStatementNode::ForStatementNode(const std::string &name, ExprNode *expr, StatementNode *statement) : name(name), expr(expr),
                                                                                                statement(statement) {}

void ForStatementNode::print(Printer &printer) const {
    printer.println("TODO");
}

void ForStatementNode::codegen(CodegenContext &context) {
    Function *TheFunction = context.builder.GetInsertBlock()->getParent();

    // create blocks
    BasicBlock *ConditionBB = BasicBlock::Create(context.llvmContext, "for.condition");
    BasicBlock *ContentBB = BasicBlock::Create(context.llvmContext, "for.content");
    BasicBlock *ContinueBB = BasicBlock::Create(context.llvmContext, "for.continue");

    CodegenValue *iterator = expr->codegen(context);
    context.builder.CreateBr(ConditionBB);

    TheFunction->getBasicBlockList().push_back(ConditionBB);
    context.builder.SetInsertPoint(ConditionBB);
    CodegenValue *condCall = iterator->type->getChild(context, iterator, "hasNext");
    CodegenValue *cond = condCall->doCall(context, {});
    context.builder.CreateCondBr(cond->value, ContentBB, ContinueBB);

    TheFunction->getBasicBlockList().push_back(ContentBB);
    context.builder.SetInsertPoint(ContentBB);
    {
        ChildCodegenContext forContext(context);
        CodegenValue *valueCall = iterator->type->getChild(forContext, iterator, "next");
        CodegenValue *value = valueCall->doCall(forContext, {});
        context.addValue(name, value);
        statement->codegen(forContext);
    }
    context.builder.CreateBr(ConditionBB);

    TheFunction->getBasicBlockList().push_back(ContinueBB);
    context.builder.SetInsertPoint(ContinueBB);
}

ForStatementNode::~ForStatementNode() {
    delete expr;
    delete statement;
}
