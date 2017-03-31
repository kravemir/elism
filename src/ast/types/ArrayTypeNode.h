/**
 * @author Miroslav Kravec
 */

#ifndef BP_ARRAYTYPENODE_H
#define BP_ARRAYTYPENODE_H


#include <CodegenContext.h>
#include "ast/TypeNode.h"

class ArrayTypeNode: public TypeNode {
public:
    ArrayTypeNode(TypeNode *baseType);

    virtual ~ArrayTypeNode();

    std::string toString() override;

    llvm::Type *codegen(CodegenContext &context) override;

private:
    TypeNode *baseType;
};


#endif //BP_ARRAYTYPENODE_H
