#include "EncRTree.h"

// 实现打印树的函数
void EncRTree::printTree() const {
    printNode(root, 0);
}

// 实现辅助递归函数，用于打印树
void EncRTree::printNode(const EncNode* node, int depth) const {
    if (!node) return;

    // 打印缩进
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }

    // 打印节点信息
    std::cout << (node->isLeaf ? "Leaf " : "Internal ") << "Node with " 
              << node->data_points.size() << " data points and "
              << node->children.size() << " children." << std::endl;
              
    // 递归打印子节点
    for (const auto& child : node->children) {
        printNode(child, depth + 1);
    }
}
