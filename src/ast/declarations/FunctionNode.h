/**
 * @author Miroslav Kravec
 */

#ifndef BP_FUNCTIONNODE_H
#define BP_FUNCTIONNODE_H

#include <vector>

#include "ast/StatementNode.h"
#include "ast/TypeNode.h"
#include "CodegenContext.h"

class FunctionNode: public StatementNode {
public:
    FunctionNode(const char *name,
                 TypeNode *returnType,
                 const std::vector<std::pair<std::string,TypeNode*>> arguments,
                 const std::vector<StatementNode *> &statements);
    virtual ~FunctionNode();

    virtual void print(Printer &printer) const;

    void codegen(CodegenContext &context);

    void codegenAsClassStatement(ClassTypeContext &context) override;

private:
    const char *name;
    TypeNode *returnType;
    std::vector<std::pair<std::string, TypeNode *>> arguments;
    std::vector<StatementNode*> statements;
};


#endif //BP_FUNCTIONNODE_H
