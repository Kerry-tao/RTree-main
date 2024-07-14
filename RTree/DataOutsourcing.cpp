#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <locale>
#include <codecvt>
#include <seal/seal.h>
#include "matVecMul.h"
#include "Node.h"
#include "EncNode.h"
#include "DataOutsourcing.h"

using namespace std;
using namespace seal;
using namespace gemini;

// seal::SEALContext context;
// auto context = SEALContext::Create(SEALContext::default_parms());
// std::shared_ptr<seal::SEALContext> context;
// std::shared_ptr<seal::SecretKey> sk;
// seal::PublicKey pk;
// seal::RelinKeys relin_keys;
// HomFCSS hom_fcss;

// Encryptor* encryptor;
// Decryptor* decryptor;
// Evaluator* evaluator;

void initializeGlobals() {
        // using namespace seal;
        // EncryptionParameters parms(scheme_type::bfv);
        // size_t poly_modulus_degree = 8192;  // 修改多项式模数度以提供更大的容错空间
        // parms.set_poly_modulus_degree(poly_modulus_degree);//自动设置模数
        // parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));// 设置系数模数
        // // HomFCSS hom_fcss;
        // uint64_t large_plain_modulus = static_cast<uint64_t>(pow(2, 50)); // Compute 2^37
        // parms.set_plain_modulus(large_plain_modulus);
        // // 创建SEALContext
        // // auto context = make_shared<SEALContext>(parms, true, sec_level_type::tc128);
        // context = std::make_shared<SEALContext>(parms, true, sec_level_type::tc128);
        // // 密钥生成
        // KeyGenerator keygen(*context);
        // // auto sk = make_shared<SecretKey>(keygen.secret_key());
        // sk = std::make_shared<SecretKey>(keygen.secret_key());
        // // 创建公钥
        // // PublicKey pk;
        // keygen.create_public_key(pk);
        // // 创建重新线性化密钥
        // // RelinKeys relin_keys;
        // keygen.create_relin_keys(relin_keys);
        // // 模拟FCSS组件设置
        // hom_fcss.setUp(*context, *sk, std::make_shared<PublicKey>(pk));
        // cout << "Setup completed........." << endl;
        // Encryptor encryptor(*context, pk);
        // Decryptor decryptor(*context, *sk);
        // Evaluator evaluator(*context);
}

// 解析字符串并将其转换为 DataPoint 对象
DataPoint* DataOutsourcing::parseDataPoint(std::string &line)
{

    DataPoint *dp = new DataPoint();
    std::stringstream ss(line);
    std::string item;
    // 解析 name
    std::getline(ss, item, ',');
    dp->name = std::stoull(item);

    // 解析 coordinate
    std::getline(ss, item, '}');
    item = item.substr(1); // 去掉前面的 '{'
    std::stringstream coordStream(item);
    std::getline(coordStream, item, ',');
    dp->coordinate.x = std::stoull(item);
    std::getline(coordStream, item, ',');
    dp->coordinate.y = std::stoull(item);
    // 解析 vector
    std::getline(ss, item, '}');
    if (item[0] == ',' && item[1] == '{') {
        item = item.substr(2); // 去掉前面的 ',{'
    }
    std::stringstream vectorStream(item);
    while (std::getline(vectorStream, item, ','))
    {
        dp->vector.push_back(std::stoull(item));
    }
    // 解析 distance
    std::getline(ss, item, '}');
    item = item.substr(2); // 去掉前面的 ',{'
    std::stringstream distanceStream(item);
    while (std::getline(distanceStream, item, ','))
    {
        dp->distance.push_back(std::stoull(item));
    }
    return dp;
}

//加密树
EncRTree* DataOutsourcing::encryptRTree(RTree* tree)
{
    // 初始化全局变量
    // initializeGlobals();
    EncRTree *encTree = new EncRTree();//新建一颗加密的空R树
    encTree->root = encryptNode(tree->root); //加密根节点
    return encTree;
}


EncRTree* DataOutsourcing::encryptRTree(
     HomFCSS& hom_fcss, 
     HomFCSS::Meta& meta, 
     std::shared_ptr<seal::SEALContext> context,
     Decryptor& decryptor,
     RTree* tree)
{
    // 初始化全局变量
    // initializeGlobals();
    EncRTree *encTree = new EncRTree();//新建一颗加密的空R树
    encTree->root = encryptNode(hom_fcss,meta,context,decryptor,tree->root); //加密根节点
    return encTree;
}

//加密节点 
/**
 * isLeaf：无需加密
 * 叶子节点：加密数据点
 * 内部节点：加密vector和matrix，并遍历加密孩子节点
 */
EncNode* DataOutsourcing::encryptNode(RTreeNode *rNode) 
{
    rNode->printNode();
    EncNode *enc_node = new EncNode(); //创建一个新的节点
    enc_node->isLeaf = rNode->isLeaf;  //isLeaf无需加密
    enc_node->mbr = encryptRectangle(rNode->mbr); //加密MBR
    // enc_node->vector = encryptNodeVector(rNode->vector);
    // enc_node->matrix = encryptNodeMatric(rNode->matrix);
    if (rNode == nullptr)  //当节点为空，返回空指针
    {
        return nullptr;
    }
    if (rNode->isLeaf) {
        std::cout << "leaf" << std::endl;
        for (const auto &dp : rNode->data_points)
        {
            // encDataPoint *enc_data_points = encryptDP(dp); //加密数据点
            // enc_node->data_points.push_back(enc_data_points);
        }
    }else{ 
        std::cout << "internal" << std::endl;
        //遍历所有孩子节点，加密孩子节点
        for (const auto &node : rNode->children) 
        {
            EncNode *enc_node = encryptNode(node);
            enc_node->children.push_back(enc_node);
        }
    }
    return enc_node;
}

EncNode* DataOutsourcing::encryptNode(
    HomFCSS& hom_fcss, 
    HomFCSS::Meta& meta, 
    std::shared_ptr<seal::SEALContext> context,
    Decryptor &decryptor,
    RTreeNode *rNode) 
{
    rNode->printNode();
    EncNode *enc_node = new EncNode(); //创建一个新的节点
    enc_node->isLeaf = rNode->isLeaf;  //isLeaf无需加密
    enc_node->mbr = encryptRectangle(hom_fcss,meta,context,decryptor,rNode->mbr); //加密MBR
    // enc_node->vector = encryptNodeVector(rNode->vector);
    // enc_node->matrix = encryptNodeMatric(rNode->matrix);
    if (rNode == nullptr)  //当节点为空，返回空指针
    {
        return nullptr;
    }
    if (rNode->isLeaf) {
        std::cout << "leaf" << std::endl;
        for (const auto &dp : rNode->data_points)
        {
            // encDataPoint *enc_data_points = encryptDP(dp); //加密数据点
            // enc_node->data_points.push_back(enc_data_points);
        }
    }else{ 
        std::cout << "internal" << std::endl;
        //遍历所有孩子节点，加密孩子节点
        for (const auto &node : rNode->children) 
        {
            EncNode *enc_node = encryptNode(node);
            enc_node->children.push_back(enc_node);
        }
    }
    return enc_node;
}

// 加密矩形
/**
 * @brief 加密矩形
 * @param r 矩形
 * @return 加密后的矩形
 * 加密bottomLeft和topRight
 */
encRectangle* DataOutsourcing::encryptRectangle(Rectangle r)
{
    // encRectangle *enc_rectangle = new encRectangle(); 
    // TensorShape input_shape({1}); //一个值
    // TensorShape weight_shape({1,maxDimension});
    // //依次加密四个值

    // vector<Ciphertext> encrypted_value;
    // Tensor<uint64_t> value(input_shape);
    
    // value(0) =  (uint64_t)r.bottomLeft.x;
    // if (encryptValue(input_shape, value, encrypted_value) != 0)
    // {
    //     cerr << "Encryption process failed." << endl;
    // }
    // enc_rectangle->bottomLeft.x = encrypted_value[0];

    // value(0) = (uint64_t)r.bottomLeft.y;
    // if (encryptValue(input_shape, value, encrypted_value) != 0)
    // {
    //     cerr << "Encryption process failed." << endl;
    // }
    // enc_rectangle->bottomLeft.y = encrypted_value[0];

    // value(0) = (uint64_t)r.topRight.x;
    // if (encryptValue(input_shape, value, encrypted_value) != 0)
    // {
    //     cerr << "Encryption process failed." << endl;
    // }
    // enc_rectangle->topRight.x = encrypted_value[0];

    // value(0) = (uint64_t)r.topRight.y;
    // if (encryptValue(input_shape, value, encrypted_value) != 0)
    // {
    //     cerr << "Encryption process failed." << endl;
    // }
    // enc_rectangle->topRight.y = encrypted_value[0];
    // return enc_rectangle;
}

encRectangle* DataOutsourcing::encryptRectangle(
    HomFCSS& hom_fcss, 
    HomFCSS::Meta& meta, 
    std::shared_ptr<seal::SEALContext> context,
    Decryptor &decryptor,
    Rectangle r)
{
    encRectangle *enc_rectangle = new encRectangle(); 
    TensorShape input_shape({1}); //一个值
    TensorShape weight_shape({1,maxDimension});
    //依次加密四个值

    vector<Ciphertext> encrypted_value;
    Tensor<uint64_t> value(input_shape);
    
    value(0) =  (uint64_t)r.bottomLeft.x;
    if (encryptValue(hom_fcss,meta,context,decryptor,input_shape,value,encrypted_value) != 0)
    {
        cerr << "Encryption process failed." << endl;
    }
    enc_rectangle->bottomLeft.x = encrypted_value[0];

    value(0) = (uint64_t)r.bottomLeft.y;
    if (encryptValue(hom_fcss,meta,context,decryptor,input_shape, value, encrypted_value) != 0)
    {
        cerr << "Encryption process failed." << endl;
    }
    enc_rectangle->bottomLeft.y = encrypted_value[0];

    value(0) = (uint64_t)r.topRight.x;
    if (encryptValue(hom_fcss,meta,context,decryptor,input_shape, value, encrypted_value) != 0)
    {
        cerr << "Encryption process failed." << endl;
    }
    enc_rectangle->topRight.x = encrypted_value[0];

    value(0) = (uint64_t)r.topRight.y;
    if (encryptValue(hom_fcss,meta,context,decryptor,input_shape, value, encrypted_value) != 0)
    {
        cerr << "Encryption process failed." << endl;
    }
    enc_rectangle->topRight.y = encrypted_value[0];
    return enc_rectangle;
}

std::vector<seal::Ciphertext> DataOutsourcing::encryptNodeVector(std::vector<int> vector){
    for(int i = 0; i < vector.size(); i++){
        std::cout<<vector[i]<<" ";
    }
    TensorShape weight_shape_vector({1,maxDimension}); //加密向量的形状
    Tensor<uint64_t> input_vector(weight_shape_vector);

    for (size_t row = 0; row < weight_shape_vector.rows(); row++) {
        for (size_t col = 0; col < weight_shape_vector.cols(); col++) {
            input_vector(row, col) = vector[col];
        }
    }
    std::vector<std::vector<seal::Ciphertext>> encrypted_vector;
    if (encryptVector(weight_shape_vector, input_vector, encrypted_vector) != 0)
    {
        cerr << "Encryption process failed." << endl;
    }
    return encrypted_vector[0];
}
std::vector<std::vector<seal::Ciphertext>> DataOutsourcing::encryptNodeMatric(std::vector<std::vector<int>> matric){
    TensorShape weight_shape_vector({maxDimension,maxDimension}); //加密向量的形状
    Tensor<uint64_t> input_vector(weight_shape_vector);

    for (size_t row = 0; row < weight_shape_vector.rows(); row++) {
        for (size_t col = 0; col < weight_shape_vector.cols(); col++) {
            input_vector(row, col) = matric[row][col];
        }
    }
    std::vector<std::vector<seal::Ciphertext>> encrypted_vector;
    if (encryptVector(weight_shape_vector, input_vector, encrypted_vector) != 0)
    {
        cerr << "Encryption process failed." << endl;
    }
    return encrypted_vector;
}

/** 加密数据点
 * @param dp 数据点
 * @return 加密后的数据点
 * 加密，name,coordinate,vector,distance
 */
encDataPoint* DataOutsourcing::encryptDP(DataPoint *dp)
{

    dp->print();
    encDataPoint *newdp = new encDataPoint(); // 定义输入形状和权重形状
    TensorShape input_shape({1}); // 一个值的形状

    try {
        // 加密函数
        auto encrypt_and_set = [&](uint64_t value, Ciphertext &target) {
            vector<Ciphertext> encrypted_value;
            Tensor<uint64_t> tensor_value(input_shape);
            tensor_value(0) = value;
            std::cout << "Encrypting value: " << value << std::endl; // 调试输出
            if (encryptValue(input_shape, tensor_value, encrypted_value) != 0)
            {
                throw runtime_error("Encryption process failed.");
            }
            target = encrypted_value[0];
        };

        // 加密name
        encrypt_and_set(static_cast<uint64_t>(dp->name), newdp->name);
        // 加密坐标x
        encrypt_and_set(static_cast<uint64_t>(dp->coordinate.x), newdp->coordinate.x);
        // 加密坐标y
        encrypt_and_set(static_cast<uint64_t>(dp->coordinate.y), newdp->coordinate.y);

        // newdp->vector = encryptNodeVector(dp->vector);
        // newdp->distance = encryptNodeVector(dp->distance);
    } catch (const std::overflow_error &e) {
        cerr << "Overflow error: " << e.what() << endl;
        delete newdp;
        return nullptr;
    } catch (const std::exception &e) {
        cerr << "Exception: " << e.what() << endl;
        delete newdp;
        return nullptr;
    }

    return newdp;


    // dp->print();
    // encDataPoint *newdp = new encDataPoint();// 定义输入形状和权重形状
    // TensorShape input_shape({1}); //一个值的形状
    // TensorShape weight_shape({1,maxDimension}); //加密向量的形状

    // //加密name
    // vector<Ciphertext> encrypted_value;
    // Tensor<uint64_t> value(input_shape);
    // value(0) =  (uint64_t)dp->name;
    // if (encryptValue(input_shape, value, encrypted_value) != 0)
    // {
    //     cerr << "Encryption process failed." << endl;
    // }
    // newdp->name = encrypted_value[0];

    // //加密坐标x
    // vector<Ciphertext> encrypted_value2;
    // value(0) =  (uint64_t)dp->coordinate.x;
    // if (encryptValue(input_shape, value, encrypted_value2) != 0)
    // {
    //     cerr << "Encryption process failed." << endl;
    // }
    // newdp->coordinate.x = encrypted_value2[0];

    // //加密坐标y
    // vector<Ciphertext> encrypted_value3;
    // value(0) =  (uint64_t)dp->coordinate.y;
    // if (encryptValue(input_shape, value, encrypted_value3) != 0)
    // {
    //     cerr << "Encryption process failed." << endl;
    // }
    // newdp->coordinate.y = encrypted_value3[0];
    // // newdp->vector = encryptNodeVector(dp->vector);
    // // newdp->distance = encryptNodeVector(dp->distance);
    // return newdp;
}

int DataOutsourcing::encryptValue(const TensorShape& input_shape,Tensor<uint64_t>& input_data,vector<Ciphertext>& encrypted_output)
{
    // // 创建和初始化Meta信息
    // HomFCSS::Meta meta;
    // meta.input_shape = input_shape;
    // meta.is_shared_input = false; // 假设的共享输入标志

    // // 加密输入向量
    // vector<Serializable<Ciphertext>> encrypted_share_serialized;
    // if (hom_fcss.encryptOneValue(input_data, meta, encrypted_share_serialized) != Code::OK)
    // {
    //     cerr << "Encryption failed." << endl;
    //     return -1;
    // }
    // // 将Serializable<Ciphertext>转换为Ciphertext，用于输入向量
    // encrypted_output.resize(encrypted_share_serialized.size());
    // for (size_t i = 0; i < encrypted_share_serialized.size(); ++i)
    // {
    //     stringstream ss;
    //     encrypted_share_serialized[i].save(ss);       // 序列化到流中
    //     ss.seekg(0);                                  // 重置流的位置到开始
    //     encrypted_output[i].load(*context, ss);       // 从流中加载到Ciphertext对象
    // }

    // Encryptor encryptor(*context, pk);
    // Decryptor decryptor(*context, *sk);
    // Evaluator evaluator(*context);
    // for (auto &ciphertext : encrypted_output)
    // {
    //     Plaintext final_plain;
    //     decryptor.decrypt(ciphertext, final_plain);
    //     cout << "Decrypted result: ";
    //     print_plaintext(final_plain); // Implement this function if not already defined
    // }
    return 0;
}


int DataOutsourcing::encryptValue(
    HomFCSS& hom_fcss, 
    HomFCSS::Meta& meta, 
    std::shared_ptr<seal::SEALContext> context,
    Decryptor &decryptor,
    const TensorShape& input_shape,
    Tensor<uint64_t>& input_data,
    vector<Ciphertext>& encrypted_output)
{
    // 创建和初始化Meta信息
    HomFCSS::Meta meta_own;
    meta_own.input_shape = input_shape;
    meta_own.is_shared_input = false; // 假设的共享输入标志

    // 加密输入向量
    vector<Serializable<Ciphertext>> encrypted_share_serialized;
    if (hom_fcss.encryptOneValue(input_data, meta_own, encrypted_share_serialized) != Code::OK)
    {
        cerr << "Encryption failed." << endl;
        return -1;
    }
    // 将Serializable<Ciphertext>转换为Ciphertext，用于输入向量
    encrypted_output.resize(encrypted_share_serialized.size());
    for (size_t i = 0; i < encrypted_share_serialized.size(); ++i)
    {
        stringstream ss;
        encrypted_share_serialized[i].save(ss);       // 序列化到流中
        ss.seekg(0);                                  // 重置流的位置到开始
        encrypted_output[i].load(*context, ss);       // 从流中加载到Ciphertext对象
    }

    // Encryptor encryptor(*context, pk);
    // Decryptor decryptor(*context, *sk);
    // Evaluator evaluator(*context);
    for (auto &ciphertext : encrypted_output)
    {
        Plaintext final_plain;
        decryptor.decrypt(ciphertext, final_plain);
        cout << "Decrypted result: ";
        print_plaintext(final_plain); // Implement this function if not already defined
    }
    return 0;
}


int DataOutsourcing::encryptVector(const TensorShape& input_shape, Tensor<uint64_t>& input_data,vector<vector<Ciphertext>>& encrypted_vector)
{
    // 创建和初始化Meta信息
    // HomFCSS::Meta meta;
    // TensorShape input_shape_tmp({maxDimension});
    // meta.input_shape = input_shape_tmp;
    // meta.weight_shape = input_shape;
    // meta.is_shared_input = false; // 假设的共享输入标志

    // std::vector<std::vector<seal::Serializable<seal::Ciphertext>>> encrypted_weights_serialized;
    // if (hom_fcss.encryptWeightMatrix(input_data, meta, encrypted_weights_serialized) != Code::OK)
    // {
    //     cerr << "Weight matrix encryption failed." << endl;
    //     return -1;
    // }
    
    // encrypted_vector.resize(encrypted_weights_serialized.size());
    // for (size_t i = 0; i < encrypted_weights_serialized.size(); ++i)
    // {
    //     encrypted_vector[i].resize(encrypted_weights_serialized[i].size());
    //     for (size_t j = 0; j < encrypted_weights_serialized[i].size(); ++j)
    //     {
    //         stringstream ss;
    //         encrypted_weights_serialized[i][j].save(ss);
    //         ss.seekg(0);
    //         encrypted_vector[i][j].load(*context, ss);
    //     }
    // }

    // Encryptor encryptor(*context, pk);
    // Decryptor decryptor(*context, *sk);
    // Evaluator evaluator(*context);

    // for (auto& inner_vector : encrypted_vector) {
    //     for (auto& ciphertext : inner_vector) {
    //         Plaintext final_plain;
    //         decryptor.decrypt(ciphertext, final_plain);
    //         cout << "Decrypted matric result: ";
    //         print_plaintext(final_plain); // Implement this function if not already defined
    //     }
    // }
    return 0;
}


int DataOutsourcing::encryptVector(
    HomFCSS& hom_fcss, 
    HomFCSS::Meta& meta, 
    std::shared_ptr<seal::SEALContext> context,
    Decryptor &decryptor,
    const TensorShape& input_shape, 
    Tensor<uint64_t>& input_data,
    vector<vector<Ciphertext>>& encrypted_vector)
{
    // 创建和初始化Meta信息
    HomFCSS::Meta meta_own;
    TensorShape input_shape_tmp({maxDimension});
    meta_own.input_shape = input_shape_tmp;
    meta_own.weight_shape = input_shape;
    meta_own.is_shared_input = false; // 假设的共享输入标志

    std::vector<std::vector<seal::Serializable<seal::Ciphertext>>> encrypted_weights_serialized;
    if (hom_fcss.encryptWeightMatrix(input_data, meta_own, encrypted_weights_serialized) != Code::OK)
    {
        cerr << "Weight matrix encryption failed." << endl;
        return -1;
    }
    
    encrypted_vector.resize(encrypted_weights_serialized.size());
    for (size_t i = 0; i < encrypted_weights_serialized.size(); ++i)
    {
        encrypted_vector[i].resize(encrypted_weights_serialized[i].size());
        for (size_t j = 0; j < encrypted_weights_serialized[i].size(); ++j)
        {
            stringstream ss;
            encrypted_weights_serialized[i][j].save(ss);
            ss.seekg(0);
            encrypted_vector[i][j].load(*context, ss);
        }
    }

    // Encryptor encryptor(*context, pk);
    // Decryptor decryptor(*context, *sk);
    // Evaluator evaluator(*context);

    for (auto& inner_vector : encrypted_vector) {
        for (auto& ciphertext : inner_vector) {
            Plaintext final_plain;
            decryptor.decrypt(ciphertext, final_plain);
            cout << "Decrypted matric result: ";
            print_plaintext(final_plain); // Implement this function if not already defined
        }
    }
    return 0;
}

void DataOutsourcing::print_plaintext(const Plaintext &plain) {
    cout << "Plaintext: ";
    for (size_t i = 0; i < plain.coeff_count(); i++) {
        cout << plain[i] << " ";
    }
    cout << endl;
}