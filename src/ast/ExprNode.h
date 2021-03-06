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
    virtual ~ExprNode() = 0;

    virtual std::string toString() const = 0;

    virtual CodegenValue * codegen(CodegenContext &context, const llvm::Twine &Name = "") = 0;
};


#endif //BP_EXPRNODE_H
