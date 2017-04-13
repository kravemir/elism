//
// Created by miroslav on 4/13/17.
//

#include "OnStatementNode.h"

#include <llvm/IR/Module.h>

OnStatementNode::OnStatementNode(const std::string &name, ExprNode *expr, StatementNode *stmt) : name(name), expr(expr),
                                                                                                 stmt(stmt) {}

void OnStatementNode::print(Printer &printer) const {
    // TODO
}

void OnStatementNode::codegen(CodegenContext &context) {
    CodegenValue* region = expr->codegen(context,"on." + name);
    ChildCodegenContext ctx = ChildCodegenContext(context);
    ctx.region = region->value;

    stmt->codegen(ctx);

    context.builder.CreateCall(
            context.module->getFunction("DeleteRegion"),
            {
                    region->value
            }
    );
}
