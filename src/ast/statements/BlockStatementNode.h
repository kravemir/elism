//
// Created by miroslav on 4/6/17.
//

#ifndef BP_BLOCKSTATEMENTNODE_H
#define BP_BLOCKSTATEMENTNODE_H


#include <ast/StatementNode.h>

class BlockStatementNode: public StatementNode {
public:
    BlockStatementNode(const std::vector<StatementNode *> &statements);

    void print(Printer &printer) const override;

    void codegen(CodegenContext &context) override;

private:
    std::vector<StatementNode*> statements;

};


#endif //BP_BLOCKSTATEMENTNODE_H
