#ifndef DATAOUTSOURCING_H
#define DATAOUTSOURCING_H

#include <seal/seal.h>
#include "matVecMul.h"
#include "EncRTree.h"
#include "Node.h"
#include "RTree.h"
#include "EncRTree.h"
#include "EncNode.h"
#include <vector>

using namespace std;
using namespace seal;
using namespace gemini;

class DataOutsourcing {
    public:
    static DataPoint* parseDataPoint(std::string& line);

    //加密树
    static EncRTree* encryptRTree(
     HomFCSS& hom_fcss, 
     HomFCSS::Meta& meta, 
     std::shared_ptr<seal::SEALContext> context,
     Decryptor& decryptor,
     RTree* tree);
    // 解密树
    static void decryptRTree(
     Decryptor& decryptor,
     EncRTree* tree);

private:
    //加密节点
    static EncNode* encryptNode(
    HomFCSS& hom_fcss, 
    HomFCSS::Meta& meta, 
    std::shared_ptr<seal::SEALContext> context,
    Decryptor &decryptor,
    RTreeNode *rNode); 
    //加密节点向量
    static vector<Ciphertext> encryptNodeVector(
    HomFCSS& hom_fcss, 
    HomFCSS::Meta& meta, 
    std::shared_ptr<seal::SEALContext> context,
    Decryptor &decryptor,
    vector<int> vector);
    //加密节点矩阵
    static std::vector<std::vector<seal::Ciphertext>> encryptNodeMatric(
    HomFCSS &hom_fcss,
    HomFCSS::Meta &meta,
    std::shared_ptr<seal::SEALContext> context,
    Decryptor &decryptor,
    std::vector<std::vector<int>> matric);
    //加密节点矩形
    static encRectangle* encryptRectangle(
    HomFCSS& hom_fcss, 
    HomFCSS::Meta& meta, 
    std::shared_ptr<seal::SEALContext> context,
    Decryptor &decryptor,
    Rectangle r);
    //加密数据点
    static encDataPoint* encryptDP(
    HomFCSS& hom_fcss, 
    HomFCSS::Meta& meta, 
    std::shared_ptr<seal::SEALContext> context,
    Decryptor &decryptor,
    DataPoint* dp);
    //加密单个数据值
    static int encryptValue(
    HomFCSS& hom_fcss, 
    HomFCSS::Meta& meta, 
    std::shared_ptr<seal::SEALContext> context,
    Decryptor &decryptor,
    const TensorShape& input_shape,
    Tensor<uint64_t>& input_data,
    vector<Ciphertext>& encrypted_output);
    //加密向量和矩阵
    static int encryptVector(
    HomFCSS& hom_fcss, 
    HomFCSS::Meta& meta, 
    std::shared_ptr<seal::SEALContext> context,
    Decryptor &decryptor,
    const TensorShape& input_shape, 
    Tensor<uint64_t>& input_data, 
    vector<vector<Ciphertext>>& encrypted_output);
    static void print_plaintext(const Plaintext &plain);
    // 解密节点
    static void decryptRNode(
    Decryptor& decryptor,
    EncNode* node);
    //解密一个值
    static void decryptRValue(
    Decryptor& decryptor,
    Ciphertext ciphertext);
    //解密向量
    static void decryptRVector(
    Decryptor& decryptor,
    vector<Ciphertext>& ciphertext);
    //解密矩阵
    static void decryptRMatric(
    Decryptor& decryptor,
    vector<vector<Ciphertext>>& ciphertext);
};

#endif //DATAOUTSOURCING_H
