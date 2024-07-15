#include "Token.h"
#include <random>
#include <algorithm>
#include <numeric>
#include <stdexcept>

// 构造函数
Token::Token(int value, int x, int y, size_t vector1Size, size_t vector2Size)
    : value(value), coordinate(x, y), vector1(vector1Size, 0), vector2(vector2Size, 0) {}

Token Token::generateRandomToken(int n) {
    Token token = Token(0, 0, 0, maxDimension, maxDimension);

    // 随机选择 vector1 中的一个维度并设置为 1
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> value_dis(1, 10);
    int value = value_dis(gen);  // 随机生成1到10之间的整数作为value
    token.value = value;

    std::uniform_int_distribution<> dis1(0, maxDimension - 1); //给vector1随机选择一个维度设置1
    int randomIndex1 = dis1(gen);
    token.vector1[randomIndex1] = 1.0;

    // 随机选择 vector2 中的 n 个维度并设置为 0 到 1 之间的随机数，使其和为 10
    if (n > maxDimension-1) {
        throw std::invalid_argument("n cannot be greater than vector2 size");
    }

    std::vector<int> indices(maxDimension);
    std::iota(indices.begin(), indices.end(), 0); // 生成 0 到 vector2Size-1 的序列
    std::shuffle(indices.begin(), indices.end(), gen); // 随机打乱序列
    
    indices.erase(std::remove(indices.begin(), indices.end(), randomIndex1), indices.end()); // 移除 vector1 为 1 的位置

    std::vector<double> randomParts(n);
    std::uniform_real_distribution<> dis2(0.0, 1.0);
    double sum = 0.0;
    // 生成随机分数
    for (int i = 0; i < n; ++i) {
        randomParts[i] = dis2(gen);
        sum += randomParts[i];
    }
    // 按比例分配到各个维度，确保每个值在 0 到 10 之间
    double scale = 10.0 / sum;
    for (int i = 0; i < n; ++i) {
        token.vector2[indices[i]] = std::min(10.0, randomParts[i] * scale);
    }

    // 调整最后一个值以确保总和为 20，并且不超过 10
    double currentSum = std::accumulate(token.vector2.begin(), token.vector2.end(), 0.0);
    if (currentSum < 10.0) {
        for (int i = 0; i < n; ++i) {
            if (token.vector2[indices[i]] + (10.0 - currentSum) <= 10.0) {
                token.vector2[indices[i]] += (10.0 - currentSum);
                break;
            }
        }
    } else if (currentSum > 10.0) {
        for (int i = 0; i < n; ++i) {
            if (token.vector2[indices[i]] - (currentSum - 10.0) >= 0.0) {
                token.vector2[indices[i]] -= (currentSum - 10.0);
                break;
            }
        }
    }
    return token;
}
// 打印函数
void Token::print() const {
    std::cout << "Token(value=" << value
              << ", coordinate=(" << coordinate.first << ", " << coordinate.second << ")"
              << ", vector1={";
    for (const auto& v : vector1) {
        std::cout << v << " ";
    }
    std::cout << "}, vector2={";
    for (const auto& v : vector2) {
        std::cout << v << " ";
    }
    std::cout << "})" << std::endl;
}


//加密Token
int Token::encryptToken(
    HomFCSS& hom_fcss, 
    std::shared_ptr<seal::SEALContext> context,
    Decryptor& decryptor,
    Token& token,
    encToken& encToken){
    //创建和初始化Meta信息
    HomFCSS::Meta meta_value;
    meta_value.input_shape = TensorShape({1});  // 假设的输入形状
    // meta.weight_shape = TensorShape({maxDimension, 1});  // 假设的权重形状
    meta_value.is_shared_input = false;  // 假设的共享输入标志

    //加密value
    vector<Ciphertext> encrypted_value;
    encryptTokenValue(token.value, hom_fcss, meta_value, context, encrypted_value);
    for (auto &ciphertext : encrypted_value)
    {
        Plaintext final_plain;
        decryptor.decrypt(ciphertext, final_plain);
        cout << "Decrypted result: ";
        print_plaintext(final_plain); // Implement this function if not already defined
    }
    encToken.value = encrypted_value[0];
    // 加密坐标x
    vector<Ciphertext> encrypted_coordinate_first;
    encryptTokenValue(token.coordinate.first, hom_fcss, meta_value, context, encrypted_coordinate_first);
    for (auto &ciphertext : encrypted_coordinate_first)
    {
        Plaintext final_plain;
        decryptor.decrypt(ciphertext, final_plain);
        cout << "Decrypted result: ";
        print_plaintext(final_plain); // Implement this function if not already defined
    }
    encToken.coordinate.first = encrypted_coordinate_first[0];
    //加密坐标y
    vector<Ciphertext> encrypted_coordinate_second;
    encryptTokenValue(token.coordinate.second, hom_fcss, meta_value, context, encrypted_coordinate_second);
    for (auto &ciphertext : encrypted_coordinate_second)
    {
        Plaintext final_plain;
        decryptor.decrypt(ciphertext, final_plain);
        cout << "Decrypted result: ";
        print_plaintext(final_plain); // Implement this function if not already defined
    }
    encToken.coordinate.second = encrypted_coordinate_second[0];

    HomFCSS::Meta meta_vector;
    meta_vector.input_shape = TensorShape({maxDimension});  // 假设的输入形状
    meta_vector.weight_shape = TensorShape({maxDimension,maxDimension});  // 假设的权重形状
    meta_vector.is_shared_input = false;  // 假设的共享输入标志
    //加密vector1
    Tensor<uint64_t> input_vector(meta_vector.input_shape);
    for (size_t i = 0; i < input_vector.length(); ++i) {
        input_vector(i) = token.vector1[i];
        cout<<input_vector(i)<<" ";
    }
    // 加密输入向量
    vector<Serializable<Ciphertext>> encrypted_share_serialized;
    if (hom_fcss.encryptInputVector(input_vector, meta_vector, encrypted_share_serialized) != Code::OK) {
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
    encToken.vector1 = encrypted_input_vector;

    //加密vector2
    Tensor<uint64_t> input_vector2(meta_vector.input_shape);
    for (size_t i = 0; i < input_vector2.length(); ++i) {
        input_vector2(i) = token.vector2[i];
        cout<<input_vector2(i)<<" ";
    }
    // 加密输入向量
    vector<Serializable<Ciphertext>> encrypted_share_serialized2;
    if (hom_fcss.encryptInputVector(input_vector2, meta_vector, encrypted_share_serialized2) != Code::OK) {
        cerr << "Encryption failed." << endl;
        return -1;
    }
    // 将Serializable<Ciphertext>转换为Ciphertext，用于输入向量
    vector<Ciphertext> encrypted_input_vector2;
    encrypted_input_vector2.resize(encrypted_share_serialized2.size());
    for (size_t i = 0; i < encrypted_share_serialized2.size(); ++i) {
        stringstream ss;
        encrypted_share_serialized2[i].save(ss);  // 序列化到流中
        ss.seekg(0);  // 重置流的位置到开始
        encrypted_input_vector2[i].load(*context, ss);  // 从流中加载到Ciphertext对象
    }
    encToken.vector2 = encrypted_input_vector2;
    return 0;
}

void Token::print_plaintext(const Plaintext &plain) {
    cout << "Plaintext: ";
    for (size_t i = 0; i < plain.coeff_count(); i++) {
        cout <<plain[i] << " ";
    }
    cout << endl;
}

int Token::encryptTokenValue(int value, 
                        HomFCSS& hom_fcss, 
                        HomFCSS::Meta& meta, 
                        std::shared_ptr<seal::SEALContext> context, 
                        std::vector<seal::Ciphertext>& encrypted_input_vector) {
    Tensor<uint64_t> input_value(meta.input_shape);
    input_value(0) = value;
    vector<Serializable<Ciphertext>> encrypted_share_serialized;
    if (hom_fcss.encryptOneValue(input_value, meta, encrypted_share_serialized) != Code::OK)
    {
        cerr << "Encryption failed." << endl;
        return -1;
    }
    encrypted_input_vector.resize(encrypted_share_serialized.size());
    for (size_t i = 0; i < encrypted_share_serialized.size(); ++i)
    {
        stringstream ss;
        encrypted_share_serialized[i].save(ss);       // 序列化到流中
        ss.seekg(0);                                  // 重置流的位置到开始
        encrypted_input_vector[i].load(*context, ss);       // 从流中加载到Ciphertext对象
    }
    return 0;
}

std::pair<int, int> Token::getCoordinateFromLine(const std::string& filename, int lineNumber) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file");
    }

    std::string line;
    int currentLine = 0;

    while (std::getline(file, line)) {
        if (currentLine == lineNumber) {
            std::stringstream ss(line);
            std::string item;

            // 跳过第一个元素
            std::getline(ss, item, ',');

            // 读取坐标
            std::getline(ss, item, '}');
            item = item.substr(1); // 去掉前面的 '{'
            std::stringstream coordStream(item);
            std::getline(coordStream, item, ',');
            int x = std::stoi(item);
            std::getline(coordStream, item, ',');
            int y = std::stoi(item);

            return {x, y};
        }
        ++currentLine;
    }

    throw std::out_of_range("Line number out of range");
}

