#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <locale>
#include <codecvt>
#include <climits>  // For CHAR_BIT
#include <random> 

#include "Node.h"
#include "RTree.h"
#include <seal/seal.h>
#include "matVecMul.h"
#include "DataProcessor.h"
#include "DataOutsourcing.h"
#include "Token.h"

using namespace std;
using namespace seal;
using namespace gemini;


int main()
{
    int maxDimension = 4;  //数据集维度
    EncryptionParameters parms(scheme_type::bfv);
    size_t poly_modulus_degree = 8192;  // 修改多项式模数度以提供更大的容错空间
    parms.set_poly_modulus_degree(poly_modulus_degree);
    //自动设置模数
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    // 设置系数模数
    HomFCSS hom_fcss;
    uint64_t large_plain_modulus = static_cast<uint64_t>(pow(2, 50)); // Compute 2^37
    parms.set_plain_modulus(large_plain_modulus);
    // 创建SEALContext
    auto context = make_shared<SEALContext>(parms, true, sec_level_type::tc128);
    
    // 密钥生成
    KeyGenerator keygen(*context);
    auto sk = make_shared<SecretKey>(keygen.secret_key());
    // 创建公钥
    PublicKey pk;
    keygen.create_public_key(pk);
    // 创建重新线性化密钥
    RelinKeys relin_keys;
    keygen.create_relin_keys(relin_keys);
    // 模拟FCSS组件设置
    hom_fcss.setUp(*context, *sk, make_shared<PublicKey>(pk));
    cout << "Setup completed........." << endl;

    Encryptor encryptor(*context, pk);
    Decryptor decryptor(*context, *sk);
    Evaluator evaluator(*context);

    //创建和初始化Meta信息
    // HomFCSS::Meta meta;
    // meta.input_shape = TensorShape({1});  // 假设的输入形状
    // meta.weight_shape = TensorShape({1,1});
    // meta.is_shared_input = false;  // 假设的共享输入标志

    /**
     * 创建数据集
     */
    // std::cout << "Creating new data set..." << std::endl;
    // int maxLines = 50; // 设置要读取的数据行数
    // std::string locFilePath = "../../RTree/yelp-loc.txt";
    // std::string docFilePath = "../../RTree/yelp-doc.txt";
    // std::string basePath = "../../RTree/dataPoint"; //写入数据集地址
    // std::string outputFilePath = basePath + "_" + std::to_string(maxLines) + "_" + std::to_string(maxDimension) + ".txt";
    // DataProcessor processor(locFilePath, docFilePath);
    // processor.processFiles(maxLines, maxDimension);
    // RTree::calculateDistances(processor.dataList);
    // processor.writeDataToFile(outputFilePath);

    /**
     * 测试数据集加密
     */
    // std::cout << "Reading data from file..." << std::endl;
    // RTree rtree(4);  //每棵树的最大节点数
    // std::string filename = "../../RTree/dataPoint10.txt"; // 替换为你的文件名
    // std::ifstream file(filename);
    // std::string line;
    // while (std::getline(file, line))
    // {
    //     rtree.insert(DataOutsourcing::parseDataPoint(line));
    // }
    // file.close();
    // rtree.printTree2();


    /**
     * 测试树加密
     */
    // RTree rtree2;
    // std::vector<DataPoint> data_points = {
    //     {1, {4, 1}, {1, 0, 0, 1}},
    //     {2, {5, 2}, {0, 1, 0, 1}},
    //     {3, {4, 5}, {0, 0, 1, 0}},
    //     {4, {9, 10}, {1, 0, 0, 0}},
    //     {5, {10, 9}, {0, 1, 1, 0}},
    //     {6, {9, 9}, {0, 1, 0, 0}},
    //     {7, {2, 4}, {1, 0, 0, 1}},
    //     {8, {8, 4}, {0, 1, 1, 0}},
    //     {9, {4, 7}, {0, 0, 1, 1}}
    //     };
    // rtree2.calculateDistances(data_points);
    // for (const DataPoint &data_point : data_points)
    // {
    //     rtree2.insert(new DataPoint(data_point));
    // }
    // rtree2.printTree2();
    // // // 加密树
    // EncRTree *enc_rtree2 = DataOutsourcing::encryptRTree(hom_fcss, meta, context, decryptor, &rtree2);
    // enc_rtree2->printTree();

    /**生成随机Token */
    /**(第一个值，x坐标，y坐标，带权重的维度个数) */
    // Token token = Token::generateRandomToken(5, 0, 0, 2);
    // try {
    //     std::string filename = "../../RTree/dataPoint_50_4.txt";
    //     int lineNumber = 5; // 指定要读取的行号
    //     std::pair<int, int> coordinate = Token::getCoordinateFromLine(filename, lineNumber);
    //     std::cout << "Coordinate: {" << coordinate.first << ", " << coordinate.second << "}" << std::endl;
    //     token.coordinate = coordinate;
    // } catch (const std::exception& e) {
    //     std::cerr << "Error: " << e.what() << std::endl;
    // }
    // token.print();
    // encToken enc_token;
    // Token::encryptToken(token, enc_token);

   

    // // 创建和初始化Meta信息
    // HomFCSS::Meta meta;
    // meta.input_shape = TensorShape({6});  // 假设的输入形状
    // meta.weight_shape = TensorShape({4, 6});  // 假设的权重形状
    // meta.is_shared_input = false;  // 假设的共享输入标志
    // // 创建和初始化 Tensor<uint64_t> 输入向量
    // Tensor<uint64_t> input_vector(meta.input_shape);
    // for (size_t i = 0; i < input_vector.length(); ++i) {
    //     input_vector(i) = i + 1; // 1, 2, 3, ..., 6
    // }
    // cout << "input_vector." << endl;
    // // 加密输入向量
    // vector<Serializable<Ciphertext>> encrypted_share_serialized;
    // if (hom_fcss.encryptInputVector(input_vector, meta, encrypted_share_serialized) != Code::OK) {
    //     cerr << "Encryption failed." << endl;
    //     return -1;
    // }
    // // 将Serializable<Ciphertext>转换为Ciphertext，用于输入向量
    // vector<Ciphertext> encrypted_input_vector;
    // encrypted_input_vector.resize(encrypted_share_serialized.size());
    // for (size_t i = 0; i < encrypted_share_serialized.size(); ++i) {
    //     stringstream ss;
    //     encrypted_share_serialized[i].save(ss);  // 序列化到流中
    //     ss.seekg(0);  // 重置流的位置到开始
    //     encrypted_input_vector[i].load(*context, ss);  // 从流中加载到Ciphertext对象
    // }
    // // 加密权重矩阵
    // // 初始化并填充4x6的权重矩阵
    // Tensor<uint64_t> weight_matrix(meta.weight_shape);
    // for (size_t row = 0; row < meta.weight_shape.rows(); row++) {
    //     for (size_t col = 0; col < meta.weight_shape.cols(); col++) {
    //         /** |  1 |  2 |  3 |  4 |  5 |  6 |
    //             |  7 |  8 |  9 | 10 | 11 | 12 |
    //             | 13 | 14 | 15 | 16 | 17 | 18 |
    //             | 19 | 20 | 21 | 22 | 23 | 24 |*/

    //         weight_matrix(row, col) = row * meta.weight_shape.cols() + col + 1;
    //     }
    // }
    // //vector<Serializable<Ciphertext>> encrypted_weights;
    // std::vector<std::vector<seal::Serializable<seal::Ciphertext>>> encrypted_weights_serialized;
    // if (hom_fcss.encryptWeightMatrix(weight_matrix, meta, encrypted_weights_serialized) != Code::OK) {
    //     cerr << "Weight matrix encryption failed." << endl;
    //     return -1;
    // }
    // // 将Serializable<Ciphertext>转换为Ciphertext
    // vector<vector<Ciphertext>> encrypted_weights(encrypted_weights_serialized.size());
    // for (size_t i = 0; i < encrypted_weights_serialized.size(); ++i) {
    //     encrypted_weights[i].resize(encrypted_weights_serialized[i].size());
    //     for (size_t j = 0; j < encrypted_weights_serialized[i].size(); ++j) {
    //         stringstream ss;
    //         encrypted_weights_serialized[i][j].save(ss);
    //         ss.seekg(0);
    //         encrypted_weights[i][j].load(*context, ss);
    //     }
    // }
    // // 使用matVecMul函数执行矩阵向量乘法
    // std::vector<seal::Ciphertext> encrypted_result;
    // // out_share1用于存储掩码向量，长度和输出的长度一样，也即矩阵的行数。
    // // 假设 meta.weight_shape.rows() 返回的是一个 int64_t 值，表示权重矩阵的行数
    // Tensor<uint64_t> out_share1(TensorShape({meta.weight_shape.rows()}));
    // //Tensor<uint64_t> out_share1(meta.weight_shape.rows());
    // if (hom_fcss.matVecMul(encrypted_weights, encrypted_input_vector, meta, encrypted_result, out_share1, relin_keys) != Code::OK) {
    //     cerr << "Matrix-vector multiplication failed." << endl;
    //     return -1;
    // }
    //  // 解密输出向量
    // Tensor<uint64_t> output_vector;
    // if (hom_fcss.decryptToVector(encrypted_result, meta, output_vector) != Code::OK) {
    //     cerr << "Decryption failed." << endl;
    //     return -1;
    // }

    // // 输出解密后的结果
    // cout << "Decrypted output vector: ";
    // for (size_t i = 0; i < output_vector.length(); ++i) {
    //     cout << output_vector(i) << " ";
    // }
    return 0;
}

