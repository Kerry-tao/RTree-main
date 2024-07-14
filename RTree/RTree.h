#ifndef RTREE_H
#define RTREE_H

#include "Node.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>

using namespace std;
// using namespace seal;
// using namespace gemini;

class RTree
{
public:
    RTreeNode *root;
    RTree(int maxNodeSize = 4);
    void insert(DataPoint *p);
    void printTree2() const;
    static void calculateDistances(std::vector<DataPoint> &dataPoints); //计算distance
    void traverseAndEncrpt(RTreeNode *node);
    void processVector(const std::vector<int> &vec) const;
private:
    int maxNodeSize;
    int maxDimension;
    // 第一种分裂方式
    //  void insert(RTreeNode* node, Point p);
    // void insert(RTreeNode *node, DataPoint p);
    void insert(RTreeNode *node, DataPoint *p);
    // int chooseSubtree(RTreeNode* node, Point p) const;
    int chooseSubtree(RTreeNode *node, DataPoint *p) const;
    // void splitNode(RTreeNode* parent, int index);
    void splitNode2(RTreeNode *parent, int index);
    void printTree2(RTreeNode *node, int depth) const;
};

#endif // RTREE_H
