#ifndef ENCNODE_H
#define ENCNODE_H
#include <string>
#include <seal/seal.h>
#include "matVecMul.h"

using namespace std;
using namespace seal;
using namespace gemini;

//加密的数据点
// struct encPoint
// {
//     seal::Ciphertext x, y;
//     encPoint();
//     encPoint(Ciphertext x_ref, Ciphertext y_ref): x(x_ref), y(y_ref) {}
// };

struct encPoint
{
    seal::Ciphertext x, y;
    encPoint()
    {
        //std::cout << "encPoint default constructor called" << std::endl;
    }
    encPoint(Ciphertext x_ref, Ciphertext y_ref): x(x_ref), y(y_ref)
    {
       // std::cout << "encPoint parameterized constructor called" << std::endl;
    }
};

class encRectangle{
public:
    encPoint bottomLeft, topRight;
    // 默认构造函数
    encRectangle() : bottomLeft(encPoint()), topRight(encPoint())
    {
        //std::cout << "encRectangle default constructor called" << std::endl;
    }
    // 参数化构造函数
    encRectangle(encPoint& bl, encPoint& tr) : bottomLeft(bl), topRight(tr)
    {
        //std::cout << "encRectangle parameterized constructor called" << std::endl;
    }
};


// //加密的矩形
// class encRectangle{
// public:
//     encPoint bottomLeft, topRight;
//     // 默认构造函数
//     encRectangle() : bottomLeft(encPoint()), topRight(encPoint()) {}
//     // 参数化构造函数
//     encRectangle(encPoint& bl, encPoint& tr) : bottomLeft(bl), topRight(tr) {}
// };

class encDataPoint
{
public:
    seal::Ciphertext name;
    encPoint coordinate;
    std::vector<seal::Ciphertext> vector; //存储加密的位置向量
    std::vector<seal::Ciphertext> distance; //存储加密的距离向量
    encDataPoint();
    void print() const;
};

class EncNode{
public:
    bool isLeaf;
    encRectangle* mbr;
    std::vector<EncNode*> children; //内部节点存储孩子
    std::vector<encDataPoint*> data_points; //叶子节点存储加密数据点
    std::vector<std::vector<seal::Ciphertext>> matrix; //存储加密的矩阵
    std::vector<seal::Ciphertext> vector; //存储加密的向量
    EncNode();
    EncNode(std::vector<std::vector<seal::Ciphertext>>& mat, std::vector<seal::Ciphertext>& vec, bool isLeaf = true);
};
#endif //ENCNODE_H
