#ifndef ENCRTREE_H
#define ENCRTREE_H

#include <vector>
#include "EncNode.h"

class EncRTree {
public:
    EncNode* root;
    EncRTree():root(nullptr){};
    void printTree() const;
private:
    void printNode(const EncNode* node, int depth) const; // 辅助递归函数，用于打印树
};

#endif //ENCRTREE_H

