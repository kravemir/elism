/**
 * @author Miroslav Kravec
 */

#ifndef BP_FUNCTIONNODE_H
#define BP_FUNCTIONNODE_H

#include <vector>
#include "StatementNode.h"
#include "TypeNode.h"

class FunctionNode {
public:
    FunctionNode(const char *name,
                 TypeNode *returnType,
                 const std::vector<std::pair<std::string,TypeNode*>> arguments,
                 const std::vector<StatementNode *> &statements);
    virtual ~FunctionNode();

    virtual void print(Printer &printer);
private:
    const char *name;
    TypeNode *returnType;
    std::vector<std::pair<std::string, TypeNode *>> arguments;
    std::vector<StatementNode*> statements;
};


#endif //BP_FUNCTIONNODE_H
