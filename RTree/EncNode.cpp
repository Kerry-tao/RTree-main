#include "EncNode.h"
#include <iostream>
using namespace std;

// 全局默认向量，用于引用初始化
std::vector<seal::Ciphertext> default_vector;
std::vector<std::vector<seal::Ciphertext>> default_ciphertext_2dvector;

//坐标
encPoint::encPoint():x(x), y(y) {}
//矩形
// encRectangle::encRectangle():bottomLeft(encPoint()), topRight(encPoint()) {}
// encRectangle::encRectangle(encPoint& bl, encPoint& tr) : bottomLeft(bl), topRight(tr) {}

//数据点
encDataPoint::encDataPoint():name(name), coordinate(), vector(default_vector), distance(default_vector) {}
// encDataPoint::encDataPoint(std::vector<seal::Ciphertext>& n, const encPoint& c, std::vector<std::vector<seal::Ciphertext>>& v,std::vector<std::vector<seal::Ciphertext>>& d): name(n), coordinate(c), vector(v), distance(d) {}

// print 方法的实现
void encDataPoint::print() const {
    std::cout << "encDataPoint:" << std::endl;
    std::cout << "  Name: [encrypted data]" << std::endl;
    // coordinate.print();
    std::cout << "  Vector: [encrypted data]" << std::endl;
    std::cout << "  Distance: [encrypted data]" << std::endl;
}

// 节点
EncNode::EncNode(): isLeaf(true), matrix(default_ciphertext_2dvector), vector(default_vector) {}
EncNode::EncNode(std::vector<std::vector<seal::Ciphertext>>& mat, std::vector<seal::Ciphertext>& vec, bool leaf): isLeaf(leaf), matrix(mat), vector(vec) {}