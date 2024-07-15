#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <locale>
#include <codecvt>
#include <climits> // For CHAR_BIT
#include <random>

#include "Node.h"
#include "RTree.h"
#include <seal/seal.h>
#include "matVecMul.h"
#include "DataProcessor.h"
#include "DataOutsourcing.h"
#include "Token.h"

#include <vector>
#include <cmath>

using namespace std;
using namespace seal;
using namespace gemini;

int main()
{

    EncryptionParameters parms(scheme_type::bfv);
    size_t poly_modulus_degree = 8192;                                      // 修改多项式模数度以提供更大的容错空间
    parms.set_poly_modulus_degree(poly_modulus_degree);                     // 自动设置模数
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree)); // 设置系数模数
    vector<int> moduli_bits{60, 49}; // 与源代码一致
    parms.set_coeff_modulus(CoeffModulus::Create(poly_modulus_degree, moduli_bits));
    HomFCSS hom_fcss;
    uint64_t large_plain_modulus = static_cast<uint64_t>(pow(2, 50)); // Compute 2^37
    parms.set_plain_modulus(large_plain_modulus);
    // 创建 SEALContext
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
    // 创建和初始化Meta信息
    HomFCSS::Meta meta;
    meta.input_shape = TensorShape({1}); // 假设的输入形状
    meta.weight_shape = TensorShape({1, 1});
    meta.is_shared_input = false; // 假设的共享输入标志

    /**
     * 创建数据集已通过
     * 50_4
     * 10000_4
     * 100_1300
     * 10000_1300
     */
    int maxLines = 10000; // 设置要读取的数据行数
    int maxDimension = 1300; // 数据集维度
    // std::cout << "Creating new data set..." << std::endl;
    // std::string locFilePath = "../../RTree/yelp-loc.txt";
    // std::string docFilePath = "../../RTree/yelp-doc.txt";
    // std::string basePath = "../../RTree/dataPoint"; //写入数据集地址
    // std::string outputFilePath = basePath + "_" + std::to_string(maxLines) + "_" + std::to_string(maxDimension) + ".txt";
    // DataProcessor processor(locFilePath, docFilePath);
    // processor.processFiles(maxLines, maxDimension);
    // RTree::calculateDistances(processor.dataList);
    // processor.writeDataToFile(outputFilePath);

    /**
     * 测试数据集加密50_4已通过
     * Time taken to build the tree: 0.000331101 seconds
     * Time taken to encrypt the tree: 0.470035 seconds
     * 测试数据集加密10000_4已通过
     * Time taken to build the tree: 0.106072 seconds
     * Time taken to encrypt the tree: 91.8519 seconds
     * 测试数据集加密100_1300已通过
     * Time taken to build the tree: Time taken to build the tree: 21.7354 seconds
     * Time taken to encrypt the tree: Time taken to encrypt the tree: 5.19078 seconds
     * 测试数据集加密10000_1300
     * 
     */
    std::string Pathtree = "../../RTree/tree_output_" + std::to_string(maxLines) + "_" + std::to_string(maxDimension) + ".txt"; //写入数据集地址
    std::string Pathenctree = "../../RTree/enctree_output_" + std::to_string(maxLines) + "_" + std::to_string(maxDimension) + ".txt"; //写入数据集地址
    auto start = std::chrono::high_resolution_clock::now(); // 记录开始时间
    std::cout << "Reading data from file..." << std::endl;
    RTree rtree(4);  //每棵树的最大节点数
    std::string filename = "../../RTree/dataPoint_10000_1300.txt"; // 替换为你的文件名
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
    {
        rtree.insert(DataOutsourcing::parseDataPoint(line));
    }
    file.close();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time taken to build the tree: " << elapsed.count() << " seconds" << std::endl;


    // 重定向 std::cout 到文件
    std::ofstream outFile(Pathtree);
    std::streambuf* coutBuf = std::cout.rdbuf(); // 保存原来的缓冲区
    std::cout.rdbuf(outFile.rdbuf()); // 重定向到文件
    rtree.printTree2();  //将明文树结构的输出写入文件
    // 恢复 std::cout 到原来的缓冲区
    std::cout.rdbuf(coutBuf);
    outFile.close();

    
    start = std::chrono::high_resolution_clock::now();// 记录加密开始时间
    EncRTree *enc_rtree = DataOutsourcing::encryptRTree(hom_fcss, meta, context, decryptor, &rtree);
    end = std::chrono::high_resolution_clock::now();// 记录加密结束时间并计算加密的时间
    elapsed = end - start;
    std::cout << "Time taken to encrypt the tree: " << elapsed.count() << " seconds" << std::endl;
    
    // 重定向 std::cout 到文件
    std::ofstream outFile2(Pathenctree);
    std::streambuf* coutBuf2 = std::cout.rdbuf(); // 保存原来的缓冲区
    std::cout.rdbuf(outFile2.rdbuf()); // 重定向到文件
    DataOutsourcing::decryptRTree(decryptor, enc_rtree); //将解密的所有输出输出到文件
    // 恢复 std::cout 到原来的缓冲区
    std::cout.rdbuf(coutBuf2);

    /**
     * 测试树加密已通过
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
    //     {9, {4, 7}, {0, 0, 1, 1}}};
    // rtree2.calculateDistances(data_points);
    // for (const DataPoint &data_point : data_points)
    // {
    //     rtree2.insert(new DataPoint(data_point));
    // }
    // rtree2.printTree2();
    // // 加密树
    // EncRTree *enc_rtree2 = DataOutsourcing::encryptRTree(hom_fcss, meta, context, decryptor, &rtree2);
    // std::cout<<"--------------------Decrypting tree-----------------------------"<<endl;
    // DataOutsourcing::decryptRTree(decryptor, enc_rtree2);
    

    /**生成随机Token已通过 */
    /**(第一个值1-10，x坐标，y坐标，从数据集指定行读取；带权重的维度个数) */
    // Token token = Token::generateRandomToken(2);
    // std::string filename = "../../RTree/dataPoint_50_4.txt"; //指定要读取的数据集
    // int lineNumber = 5; // 指定要读取的行号

    // std::pair<int, int> coordinate = Token::getCoordinateFromLine(filename, lineNumber-1);
    // std::cout << "Coordinate: {" << coordinate.first << ", " << coordinate.second << "}" << std::endl;
    // token.coordinate = coordinate;
    // token.print(); //打印生成的随机token
    // encToken enc_token;
    // Token::encryptToken(hom_fcss, context, decryptor, token, enc_token);

    return 0;
}
