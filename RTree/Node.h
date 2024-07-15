#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cfloat>

static int maxDimension = 1300;

struct Point
{
    float x, y;
    Point(float x = 0, float y = 0);
};

class DataPoint
{
public:
    int name;
    Point coordinate;
    std::vector<int> vector;
    std::vector<int> distance;
    DataPoint() : name(0), coordinate((0, 0)){};
    DataPoint(const int name, const Point &point, const std::vector<int> &vector) : name(name), coordinate(point), vector(vector){};
    void print() const;
};

struct Rectangle
{
    Point bottomLeft, topRight;
    Rectangle(Point bottomLeft = Point(), Point topRight = Point());

    bool contains(Point p) const;
    bool intersects(Rectangle other) const;
    float area() const;
};


//构建一颗R树，内部节点为RTreeNode，存储RTreeNode类型的子节点数组，叶子节点需要存储DataPoint的数组，
class RTreeNode
{
public:
    bool isLeaf;
    std::vector<Point> points; // for leaf nodes
    std::vector<DataPoint *> data_points;
    std::vector<RTreeNode *> children; // for internal nodes

    Rectangle mbr; // minimum bounding rectangle
    std::vector<int> vector;
    std::vector<std::vector<int>> matrix;

    RTreeNode(bool isLeaf = true);
    void updateMBR();
    void updateVec();
    void updateMatrix();
    void printNode();
};
#endif // NODE_H
