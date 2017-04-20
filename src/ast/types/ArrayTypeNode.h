/**
 * @author Miroslav Kravec
 */

#ifndef BP_ARRAYTYPENODE_H
#define BP_ARRAYTYPENODE_H


#include <CodegenContext.h>
#include "ast/TypeNode.h"

class ArrayTypeNode: public TypeNode {
public:
    ArrayTypeNode(TypeNode *baseType, std::vector<std::string> &&regions);

    virtual ~ArrayTypeNode();

    std::string toString() override;

    CodegenType *codegen(CodegenContext &context) override;

private:
    TypeNode *baseType;
};


#endif //BP_ARRAYTYPENODE_H
