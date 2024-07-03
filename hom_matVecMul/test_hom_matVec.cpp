#include <iostream>
#include <sstream> // 包含字符串流头文件
#include <seal/seal.h>
#include "matVecMul.h"
#include "tensor.h"
#include "tensor_shape.h"

using namespace seal;
using namespace gemini;
using namespace std;

int main() {
    // 初始化加密参数
    EncryptionParameters parms(scheme_type::bfv);
    //size_t poly_modulus_degree = 4096;
    size_t poly_modulus_degree = 8192;  // 修改多项式模数度以提供更大的容错空间
    parms.set_poly_modulus_degree(poly_modulus_degree);

    //自动设置模数
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    
    // 设置系数模数
    //vector<int> moduli_bits{60, 49}; // 与源代码一致
    //parms.set_coeff_modulus(CoeffModulus::Create(poly_modulus_degree, moduli_bits));

    HomFCSS hom_fcss;

    uint64_t large_plain_modulus = static_cast<uint64_t>(pow(2, 37)); // Compute 2^37
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

    // 创建和初始化Meta信息
    HomFCSS::Meta meta;
    meta.input_shape = TensorShape({6});  // 假设的输入形状
    meta.weight_shape = TensorShape({4, 6});  // 假设的权重形状
    meta.is_shared_input = false;  // 假设的共享输入标志

    // 创建和初始化 Tensor<uint64_t> 输入向量
    Tensor<uint64_t> input_vector(meta.input_shape);
    for (size_t i = 0; i < input_vector.length(); ++i) {
        input_vector(i) = i + 1; // 1, 2, 3, ..., 6
    }
    cout << "input_vector." << endl;

    // 加密输入向量
    vector<Serializable<Ciphertext>> encrypted_share_serialized;
    if (hom_fcss.encryptInputVector(input_vector, meta, encrypted_share_serialized) != Code::OK) {
        cerr << "Encryption failed." << endl;
        return -1;
    }

    // 将Serializable<Ciphertext>转换为Ciphertext，用于输入向量
    vector<Ciphertext> encrypted_input_vector;
    encrypted_input_vector.resize(encrypted_share_serialized.size());
    for (size_t i = 0; i < encrypted_share_serialized.size(); ++i) {
        stringstream ss;
        encrypted_share_serialized[i].save(ss);  // 序列化到流中
        ss.seekg(0);  // 重置流的位置到开始
        encrypted_input_vector[i].load(*context, ss);  // 从流中加载到Ciphertext对象
    }


    // 加密权重矩阵
    // 初始化并填充4x6的权重矩阵
    Tensor<uint64_t> weight_matrix(meta.weight_shape);
    for (size_t row = 0; row < meta.weight_shape.rows(); row++) {
        for (size_t col = 0; col < meta.weight_shape.cols(); col++) {
            /** |  1 |  2 |  3 |  4 |  5 |  6 |
                |  7 |  8 |  9 | 10 | 11 | 12 |
                | 13 | 14 | 15 | 16 | 17 | 18 |
                | 19 | 20 | 21 | 22 | 23 | 24 |*/

            weight_matrix(row, col) = row * meta.weight_shape.cols() + col + 1;
        }
    }

    //vector<Serializable<Ciphertext>> encrypted_weights;
    std::vector<std::vector<seal::Serializable<seal::Ciphertext>>> encrypted_weights_serialized;
    if (hom_fcss.encryptWeightMatrix(weight_matrix, meta, encrypted_weights_serialized) != Code::OK) {
        cerr << "Weight matrix encryption failed." << endl;
        return -1;
    }

    // 将Serializable<Ciphertext>转换为Ciphertext
    vector<vector<Ciphertext>> encrypted_weights(encrypted_weights_serialized.size());
    for (size_t i = 0; i < encrypted_weights_serialized.size(); ++i) {
        encrypted_weights[i].resize(encrypted_weights_serialized[i].size());
        for (size_t j = 0; j < encrypted_weights_serialized[i].size(); ++j) {
            stringstream ss;
            encrypted_weights_serialized[i][j].save(ss);
            ss.seekg(0);
            encrypted_weights[i][j].load(*context, ss);
        }
    }


    // 使用matVecMul函数执行矩阵向量乘法
    std::vector<seal::Ciphertext> encrypted_result;
    // out_share1用于存储掩码向量，长度和输出的长度一样，也即矩阵的行数。
    // 假设 meta.weight_shape.rows() 返回的是一个 int64_t 值，表示权重矩阵的行数
    Tensor<uint64_t> out_share1(TensorShape({meta.weight_shape.rows()}));
    //Tensor<uint64_t> out_share1(meta.weight_shape.rows());

    if (hom_fcss.matVecMul(encrypted_weights, encrypted_input_vector, meta, encrypted_result, out_share1, relin_keys) != Code::OK) {
        cerr << "Matrix-vector multiplication failed." << endl;
        return -1;
    }

    // 解密输出向量
    Tensor<uint64_t> output_vector;
    if (hom_fcss.decryptToVector(encrypted_result, meta, output_vector) != Code::OK) {
        cerr << "Decryption failed." << endl;
        return -1;
    }

    // 输出解密后的结果
    cout << "Decrypted output vector: ";
    for (size_t i = 0; i < output_vector.length(); ++i) {
        cout << output_vector(i) << " ";
    }
    
    cout << "OK";
    cout << endl;

    return 0;
}
