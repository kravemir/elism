/**
 * @author Miroslav Kravec
 */

#ifndef BP_EXPRNODE_H
#define BP_EXPRNODE_H

#include <string>
#include <llvm/IR/Value.h>
#include <CodegenContext.h>

class ExprNode {
public:
    virtual std::string toString() const = 0;

    virtual CodegenValue * codegen(CodegenContext &context) = 0;
};


#endif //BP_EXPRNODE_H
