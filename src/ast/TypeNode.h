/**
 * @author Miroslav Kravec
 */

#ifndef BP_TYPENODE_H
#define BP_TYPENODE_H

#include <string>

class TypeNode {
public:
    virtual ~TypeNode();

    virtual std::string toString() = 0;
};


#endif //BP_TYPENODE_H
