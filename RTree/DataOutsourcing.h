#ifndef DATAOUTSOURCING_H
#define DATAOUTSOURCING_H

#include <seal/seal.h>
#include "matVecMul.h"
#include "EncRTree.h"
#include "Node.h"
#include "RTree.h"
#include "EncNode.h"

using namespace std;
using namespace seal;
using namespace gemini;

extern std::shared_ptr<seal::SEALContext> context;
extern std::shared_ptr<seal::SecretKey> sk;
extern seal::PublicKey pk;
extern seal::RelinKeys relin_keys;
extern HomFCSS hom_fcss;

void initializeGlobals();


class DataOutsourcing {
    public:
    // DataPoint readDataPointFromLine(const std::string &line);//一次读取一条数据
    // DataPoint parseDataPoint(const std::string& line);
    static DataPoint* parseDataPoint(std::string& line);

    static EncRTree* encryptRTree(RTree* root); //加密树
    
    static EncRTree* encryptRTree(
     HomFCSS& hom_fcss, 
     HomFCSS::Meta& meta, 
     std::shared_ptr<seal::SEALContext> context,
     Decryptor& decryptor,
     RTree* tree);


    static EncNode* encryptNode(RTreeNode* rNode); //加密节点

    static EncNode* encryptNode(
    HomFCSS& hom_fcss, 
    HomFCSS::Meta& meta, 
    std::shared_ptr<seal::SEALContext> context,
    Decryptor &decryptor,
    RTreeNode *rNode); 
    
    static vector<Ciphertext> encryptNodeVector(vector<int> vector);
    static std::vector<std::vector<seal::Ciphertext>> encryptNodeMatric(std::vector<std::vector<int>> matric);
    
    static encRectangle* encryptRectangle(Rectangle r); //加密矩形
    static encRectangle* encryptRectangle(
    HomFCSS& hom_fcss, 
    HomFCSS::Meta& meta, 
    std::shared_ptr<seal::SEALContext> context,
    Decryptor &decryptor,
    Rectangle r);

    static encDataPoint* encryptDP(DataPoint* dp);


    static int encryptValue(const TensorShape& input_shape, Tensor<uint64_t>& input_data, vector<Ciphertext>& encrypted_output);
    
    static int encryptValue(
    HomFCSS& hom_fcss, 
    HomFCSS::Meta& meta, 
    std::shared_ptr<seal::SEALContext> context,
    Decryptor &decryptor,
    const TensorShape& input_shape,
    Tensor<uint64_t>& input_data,
    vector<Ciphertext>& encrypted_output);
    
    
    static int encryptVector(const TensorShape& input_shape, Tensor<uint64_t>& input_data, vector<vector<Ciphertext>>& encrypted_output);
    
    static int encryptVector(
        HomFCSS& hom_fcss, 
        HomFCSS::Meta& meta, 
        std::shared_ptr<seal::SEALContext> context,
        Decryptor &decryptor,
        const TensorShape& input_shape, 
        Tensor<uint64_t>& input_data, 
        vector<vector<Ciphertext>>& encrypted_output);
    
    
    static void print_plaintext(const Plaintext &plain);
};

#endif //DATAOUTSOURCING_H
