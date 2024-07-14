#include <iostream>
#include <seal/seal.h>
#include <vector>
#include "protocol.h"  // 包含协议头文件

#include <climits>  // For CHAR_BIT
#include <random>   // For std::mt19937 and std::uniform_int_distribution

using namespace std;
using namespace seal;
using namespace gemini;


void print_plaintext(const Plaintext &plain) {
    cout << "Plaintext: ";
    for (size_t i = 0; i < plain.coeff_count(); i++) {
        cout << plain[i] << " ";
    }
    cout << endl;
}

void print_ciphertext_metadata(const Ciphertext &cipher) {
    cout << "Ciphertext metadata: " << endl;
    cout << "    poly_modulus_degree: " << cipher.poly_modulus_degree() << endl;
    cout << "    coeff_modulus_size: " << cipher.coeff_modulus_size() << endl;
    cout << "    size: " << cipher.size() << endl;
}

// 随机数生成器初始化
std::mt19937 rng(std::random_device{}());  // 随机数发生器，种子为随机设备

uint64_t generate_random_bits(size_t bit_length) {
    if (bit_length >= sizeof(uint64_t) * CHAR_BIT) {
        return std::uniform_int_distribution<uint64_t>(0, UINT64_MAX)(rng);
    }
    uint64_t max_value = (1ULL << bit_length) - 1;
    return std::uniform_int_distribution<uint64_t>(0, max_value)(rng);
}

void update_random_numbers(uint64_t &r1, uint64_t &r2, uint64_t plain_modulus) {
    do {
        r1 = generate_random_bits(11);  // delta = 11 bits
        r2 = generate_random_bits(11); //
    } while (r1 <= r2 || r2 == 0);
}

Ciphertext s1DataComparProcotol(const Ciphertext &ciphertext1, const Ciphertext &ciphertext2, Decryptor &decryptor, 
                                const Evaluator &evaluator, const SEALContext &context, HomFCSS &protocols, const HomFCSS::Meta &meta, 
                                uint64_t plain_modulus) {
    Ciphertext enc_m;
    evaluator.sub(ciphertext1, ciphertext2, enc_m);

    uint64_t r1, r2_value;
    // 在函数中调用 update_random_numbers
    update_random_numbers(r1, r2_value, plain_modulus);
    /**do {
        r1 = rand() % plain_modulus;
        r2_value = rand() % plain_modulus;
    } while (r1 <= r2_value || r2_value == 0);*/

    cout << "r1: " << r1 << ", r2_value: " << r2_value << endl;

    // 编码 r1
    Tensor<uint64_t> r1_vector(meta.input_shape);
    r1_vector(0) = r1;
    vector<Plaintext> plain_r1_vec;
    protocols.encodeOneValue(r1_vector, meta, plain_r1_vec);  // 使用 protocols 对象
    Plaintext plain_r1 = plain_r1_vec[0];
    //cout << "Encoded plain_r1: ";
    //print_plaintext(plain_r1);

    // 编码并加密 r2_value
    Tensor<uint64_t> r2_vector(meta.input_shape);
    r2_vector(0) = r2_value;
    vector<seal::Serializable<seal::Ciphertext>> encrypted_r2_vec;
    protocols.encryptOneValue(r2_vector, meta, encrypted_r2_vec);  // 使用 protocols 对象

    // 反序列化 encrypted_r2_vec
    Ciphertext enc_r2;
    stringstream ss;
    encrypted_r2_vec[0].save(ss);
    ss.seekg(0);
    enc_r2.load(context, ss);

    Ciphertext enc_r1, enc_z;
    evaluator.multiply_plain(enc_m, plain_r1, enc_r1);
    evaluator.add(enc_r1, enc_r2, enc_z);

    return enc_z;
}

int s2DataComparProcotol(const Ciphertext &enc_z, Decryptor &decryptor, uint64_t plain_modulus) {  // 移除 const
    Plaintext plain;
    decryptor.decrypt(enc_z, plain);  // 调用非 const 的 decrypt 方法

    cout << "Decrypted enc_z: ";
    print_plaintext(plain);

    uint64_t decrypted_value = plain[0]; // 取多项式的常数项
    //return decrypted_value < 0 ? 1 : 0; // 修改比较逻辑，确保 value1 < value2 输出 1

    int decrypted_bits = std::floor(std::log2(decrypted_value < 0 ? -decrypted_value : decrypted_value + 1)); // 计算二进制位数
    int plain_modulus_bits = std::floor(std::log2(plain_modulus)) / 2; // 计算 plain_modulus 二进制长度的一半

    return decrypted_bits < plain_modulus_bits ? 0 : 1; // 修改比较逻辑

}

//PDC协议
int DataComparFunc(const Ciphertext &ciphertext1, const Ciphertext &ciphertext2, const Evaluator &evaluator,
                    const SEALContext &context, HomFCSS &protocols, Decryptor &decryptor, const HomFCSS::Meta &meta,
                    uint64_t plain_modulus) {  // 移除 const
    Ciphertext enc_z = s1DataComparProcotol(ciphertext1, ciphertext2, decryptor, evaluator, context, protocols, meta,
                         plain_modulus);
    int result = s2DataComparProcotol(enc_z, decryptor, plain_modulus);
    return result;
}

std::vector<std::pair<Ciphertext, uint64_t>> s1PMVProtocol(const std::vector<Ciphertext> &ciphertexts,
                                                         const Evaluator &evaluator, const SEALContext &context,
                                                          HomFCSS &protocols, const HomFCSS::Meta &meta) {
    std::vector<std::pair<Ciphertext, uint64_t>> encrypted_results;

    uint64_t r = generate_random_bits(50);  // 生成随机数 r
    uint64_t r_prime = generate_random_bits(50);  // 生成随机数 r'

    // 编码 r 和 r'
    Tensor<uint64_t> r_vector(meta.input_shape);
    r_vector(0) = r;
    vector<Plaintext> plain_r_vec;
    protocols.encodeOneValue(r_vector, meta, plain_r_vec);
    Plaintext plain_r = plain_r_vec[0];

    Tensor<uint64_t> r_prime_vector(meta.input_shape);
    r_prime_vector(0) = r_prime;
    vector<Plaintext> plain_r_prime_vec;
    protocols.encodeOneValue(r_prime_vector, meta, plain_r_prime_vec);
    Plaintext plain_r_prime = plain_r_prime_vec[0];

    // 计算 m' = cipher + plain_r * plain_r_prime
    for (const auto &cipher : ciphertexts) {
        Ciphertext m_middle;
        evaluator.add_plain(cipher, plain_r, m_middle);
        evaluator.multiply_plain_inplace(m_middle, plain_r_prime);

        uint64_t r_i;
        do {
            r_i = generate_random_bits(50);  // 为每个cipher生成一个r_i
        } while (r_i >= r_prime);  // 确保 r_i 小于 r_prime

        // 编码 r_i
        Tensor<uint64_t> r_i_vector(meta.input_shape);
        r_i_vector(0) = r_i;
        vector<Plaintext> plain_r_i_vec;
        protocols.encodeOneValue(r_i_vector, meta, plain_r_i_vec);
        Plaintext plain_r_i = plain_r_i_vec[0];

        // 计算 m_2 = m' + plain_r_i
        Ciphertext m_2;
        evaluator.add_plain(m_middle, plain_r_i, m_2);

        // 生成 hash(r_i)
        size_t hash_value = std::hash<uint64_t>{}(r_i);

        encrypted_results.emplace_back(m_2, hash_value);  // 将 m_2 和 hash(r_i) 存储在一起
    }

    return encrypted_results;
}

size_t s2PMVProtocol(const std::vector<std::pair<Ciphertext, size_t>> &encrypted_results, Decryptor &decryptor) {
    uint64_t max_value = 0;
    size_t max_hash = 0;
    for (const auto &enc_result : encrypted_results) {
        Plaintext plain;
        decryptor.decrypt(enc_result.first, plain);

        uint64_t decrypted_value = plain[0];
        if (decrypted_value > max_value) {
            max_value = decrypted_value;
            max_hash = enc_result.second;  // 更新最大值对应的 hash
        }
    }
    return max_hash;
}

Ciphertext getCiphertextByHash(const size_t max_hash, const std::vector<std::pair<Ciphertext, size_t>> &encrypted_results, 
                                const std::vector<Ciphertext> &ciphertexts) {
    for (size_t i = 0; i < encrypted_results.size(); ++i) {
        if (encrypted_results[i].second == max_hash) {
            return ciphertexts[i];
        }
    }
    throw std::runtime_error("Hash not found in encrypted results.");
}

//PMV协议
Ciphertext PMVFunc(const std::vector<Ciphertext> &ciphertexts, const Evaluator &evaluator, const SEALContext &context,
                     HomFCSS &protocols, Decryptor &decryptor, const HomFCSS::Meta &meta) {
    std::vector<std::pair<Ciphertext, size_t>> encrypted_results = s1PMVProtocol(ciphertexts, evaluator, context, protocols, meta);
    size_t max_hash = s2PMVProtocol(encrypted_results, decryptor);
    Ciphertext max_ciphertext = getCiphertextByHash(max_hash, encrypted_results, ciphertexts);
    return max_ciphertext;
}

/**encrypted_result 是一个向量，我们需要把整个向量都传到PPCR函数中。然后再把整个向量传到s1AddMaskPPCRProtocol函数中。然后s1AddMaskPPCRProtocol函数需要对每个encrypted_result[i]进行处理：1.对encrypted_result[i]的每个多项式系数都加上一个随机数r_ij（随机数的处理方法和之前相同）。2. 生成向量encrypted_z，每个encrypted_z[i]都是由encrypted_result[i]和随机数得来的。3.把每个encrypted_result[i]对应的随机数存储成一个向量r_i，把向量r_i编码成一个多项式r_i_overline。4.把向量encrypted_z作为返回结果。

在s2PPCRProtocol函数中，s2对于每个encrypted_z[i]进行以下处理：
1）s2利用解密密钥解开encrypted_z[i]，提取其中的某些项系数，提取系数的方法之前描述的一样。稍微不同的是，把向量记作z_i。
2）将向量z_i利用encodeOneValue函数编码为一个多项式，然后再调用公钥进行加密得到z_i_overline'。然后把z_i_overline'加入到新向量z_overline'中，最后把z_overline'返回。

在s1MinusMaskPPCRProtocol函数中，
1）收到s2的返回值后，s1对于每个多项式r_i_overline，进行和之前一样的操作，也即为每个r_i_overline生成一个向量r_i_extract，再把向量r_i_extract编码为多项式r_i_extract_overline。 
2）s1对于向量z_overline'中的每一个多项式z_i_overline'，加上r_i_extract_overline，得到最终结果，这个结果是一个向量。再把这个向量返回。
 
4.PPCR函数调用上面的三个函数，并给main函数返回结果，这个结果是一个向量。把这个结果解密，并调用print_plaintext函数输出系数。
 */
vector<Ciphertext> s1AddMaskPPCRProtocol(const vector<Ciphertext> &encrypted_result, std::vector<seal::Plaintext> &randomVec_encoded, 
                            const Evaluator &evaluator, Decryptor &decryptor, const Encryptor &encryptor, const HomFCSS &protocols, 
                            const HomFCSS::Meta &meta, size_t poly_modulus_degree) {
    //比如encrypted_result[0]=□+X+X^2+□X^3+...: 对应前两行（第 0 行和第 1 行，也即[1 2 3 4 5 6]和[7 8 9 10 11 12]）
    //encrypted_result[1]=△+X+X^2+△X^3+...: 对应最后两行（第 2 行和第 3 行，也即[13 14 15 16 17 18]和[19 20 21 22 23 24]） 
    //这个函数的目的就是给encrypted_result[0]、[1]的每个系数加上一个随机数。
    vector<Ciphertext> encrypted_z(encrypted_result.size());
    randomVec_encoded.resize(encrypted_result.size());

    long converted_poly_modulus_degree;
    if (poly_modulus_degree <= static_cast<size_t>(std::numeric_limits<long>::max())) {
        converted_poly_modulus_degree = static_cast<long>(poly_modulus_degree);
    } else {
    // 处理溢出或错误的情况
        std::cerr << "Value exceeds the limit of long type." << std::endl;
    }
    //randomVec_encoded存储:一个encrypted_result[i]对应这一个多项式randomVec_i_encoded[0]，并把randomVec_i_encoded[0]存在向量randomVec_encoded中

    for (size_t i = 0; i < encrypted_result.size(); i++) {
        // random_coeffs是存储encrypted_result[i]对应所有随机数的向量，[r1,r2,r3,...]
        gemini::Tensor<uint64_t> random_coeffs(gemini::TensorShape({converted_poly_modulus_degree})); // 使用 Tensor 来生成随机系数向量
        
        std::vector<seal::Plaintext> randomVec_i_encoded;//虽然定义为向量，但是只用第一个位置存储随机数编码后的多项式,r1+r2x+r3x^2+...
        // 填充 random_coeffs
        for (size_t j = 0; j < poly_modulus_degree; j++) {
            random_coeffs(j) = generate_random_bits(5);  // 这个函数可以直接调用
            //cout << "Random bit[" << j << "]: " << random_coeffs(j) << " ";
        }

        // Create a temporary Meta object with a new shape for random_coeffs
        HomFCSS::Meta temp_meta;
        temp_meta.input_shape = TensorShape({converted_poly_modulus_degree});  // Adjust this shape as needed

        // 把向量 random_coeffs按顺序编码成多项式
        protocols.encodeInputVectorPositiveSequence(random_coeffs, temp_meta, randomVec_i_encoded);//randomVec_i_encoded[0]=r1+r2x+r3x^2+...

        evaluator.add_plain(encrypted_result[i], randomVec_i_encoded[0], encrypted_z[i]);
        randomVec_encoded[i] = randomVec_i_encoded[0];  // 存储编码后的随机系数
        //打印前30个系数的输出
        /**Decrypted output vector: 91 217 343 469 Decrypted original[0]: 91 719108202253056 975534350352309 607991546454152 996751963601406 655561962300563 217 612291593943213 155112190639400 641098780691751 901111443251395 333988567424543 343 565234644459165 323229448550286 355809745768360 391375165217294 659881350182848 469 1026729461296895 621587769247591 195852013086298 1064078522375841 388993772623508 274863794667655 40110809526056 637164351661729 1013116331013006 123880135847063 729556696718629 
        randomVec_i_encoded[0][0]: 19 23 10 0 14 8 11 1 12 10 8 9 10 14 5 28 13 12 11 26 6 17 22 2 27 16 5 1 5 27 
        Decrypted result[0]: 110 719108202253079 975534350352319 607991546454152 996751963601420 655561962300571 228 612291593943214 155112190639412 641098780691761 901111443251403 333988567424552 353 565234644459179 323229448550291 355809745768388 391375165217307 659881350182860 480 1026729461296921 621587769247597 195852013086315 1064078522375863 388993772623510 274863794667682 40110809526072 637164351661734 1013116331013007 123880135847068 729556696718656*/
    
    }
    return encrypted_z;
}

vector<Ciphertext> s2PPCRProtocol(const vector<Ciphertext> &encrypted_z, Decryptor &decryptor, Encryptor &encryptor, 
                    const HomFCSS &protocols, const HomFCSS::Meta &meta) {
    vector<Ciphertext> enc_z_overline_prime(encrypted_z.size());
    for (size_t i = 0; i < encrypted_z.size(); i++) {
        Plaintext plain_z;
        decryptor.decrypt(encrypted_z[i], plain_z);

        // 下面开始提取plain_z的系数
        //例如在□+X+X^2+□X^3+...中，我们需要的系数正好在第0*3,1*3的位置

        //下面这段为了实现getSplit(meta, poly_degree())函数，以得到split_shape_rows，其实就是一块的行数split_shape.rows()
        int64_t split_shape_rows = 0;
        int64_t split_shape_cols = 0;
        size_t nrows = meta.weight_shape.rows();
        size_t ncols = meta.weight_shape.cols();
        size_t N = plain_z.coeff_count();
        size_t min_cost = -1;
        for (size_t d0 = 1; d0 <= std::min(N, nrows); ++d0) {
            for (size_t d1 = 1; d1 <= std::min(N, ncols); ++d1) {
            if (d0 * d1 > N) continue;
            size_t ct_in = CeilDiv(ncols, d1);
            size_t ct_out = CeilDiv(nrows, d0);
            size_t cost = ct_in + ct_out;
            if (cost < min_cost) {
                min_cost = cost;
                split_shape_rows = int64_t(d0);
                split_shape_cols = int64_t(d1);
            }
            }
        }

        //一块有多少个元素，由于我们保证了内积一定在前N项的系数里，所以小于upper的系数就已经都包含内积了。
        size_t upper = split_shape_rows * split_shape_cols;

        if (upper > static_cast<size_t>(std::numeric_limits<long>::max())) {
            // 处理溢出情况
            throw std::runtime_error("upper exceeds the maximum value that a 'long' can hold.");
        }
        gemini::Tensor<uint64_t> coefficients_extract(gemini::TensorShape({static_cast<long>(upper)}));

        // 对于每个多项式的索引
        size_t extract_index = 0;  // 用于追踪在 Tensor 中存储的有效系数位置
        for (size_t index = 0; index < plain_z.coeff_count(); ++index) {
            // 只提取特定位置的系数
            if (index < upper && index % split_shape_cols == 0) {
                coefficients_extract(extract_index++) = plain_z[index];
            }
        }

        // 将提取的系数向量调整为实际大小
        if (extract_index > static_cast<size_t>(std::numeric_limits<long>::max())) {
            // 处理溢出情况
            throw std::runtime_error("extract_index exceeds the maximum value that a 'long' can hold.");
        }
        gemini::Tensor<uint64_t> actual_extracted_coeffs(gemini::TensorShape({static_cast<long>(extract_index)}));
        for (size_t j = 0; j < extract_index; j++) {
            actual_extracted_coeffs(j) = coefficients_extract(j);
        }

        // Create a temporary Meta object with a new shape for actual_extracted_coeffs
        HomFCSS::Meta temp_meta;
        temp_meta.input_shape = TensorShape({actual_extracted_coeffs.length()});  // Adjust this shape as needed

        //将向量z_i(代码里是actual_extracted_coeffs)利用函数encodeInputVectorPositiveSequence编码为一个多项式encoded_z_i_ploy[0]，
        //然后再调用公钥进行加密得到enc_z_i_overline'[i]。最后得到新向量enc_z_i_overline'，最后把enc_z_i_overline'返回。
        std::vector<seal::Plaintext> encoded_z_i_ploy;
        protocols.encodeInputVectorPositiveSequence(actual_extracted_coeffs, temp_meta, encoded_z_i_ploy);
        encryptor.encrypt(encoded_z_i_ploy[0], enc_z_overline_prime[i]);
    }
    return enc_z_overline_prime;
}


//randomVec_encoded=[r1+r2x+r3x^2+..., r1'+r2'x+r3'x^2+..., ...]
//对于每个randomVec_encoded[i],需要取出0*3,1*3这些位置的随机数
vector<Ciphertext> s1MinusMaskPPCRProtocol(const vector<Ciphertext> &enc_z_overline_prime, const vector<Plaintext>
         &randomVec_encoded, const Evaluator &evaluator, const HomFCSS &protocols, const HomFCSS::Meta &meta) {
    vector<Ciphertext> final_result(enc_z_overline_prime.size());

    //提取每个randomVec_encoded[i]的特定的系数
    int64_t split_shape_rows = 0;
    int64_t split_shape_cols = 0;
    size_t nrows = meta.weight_shape.rows();
    size_t ncols = meta.weight_shape.cols();
    size_t N = randomVec_encoded[0].coeff_count();
    size_t min_cost = -1;
    for (size_t d0 = 1; d0 <= std::min(N, nrows); ++d0) {
        for (size_t d1 = 1; d1 <= std::min(N, ncols); ++d1) {
        if (d0 * d1 > N) continue;
        size_t ct_in = CeilDiv(ncols, d1);
        size_t ct_out = CeilDiv(nrows, d0);
        size_t cost = ct_in + ct_out;
        if (cost < min_cost) {
            min_cost = cost;
            split_shape_rows = int64_t(d0);
            split_shape_cols = int64_t(d1);
        }
        }
    }
    size_t upper = split_shape_rows * split_shape_cols;
    vector<gemini::Tensor<uint64_t>> extracted_coeffs_from_randomVec(randomVec_encoded.size());
    if (upper > static_cast<size_t>(std::numeric_limits<long>::max())) {
            // 处理溢出情况
        throw std::runtime_error("upper exceeds the maximum value that a 'long' can hold.");
    }

    for (size_t i = 0; i < randomVec_encoded.size(); i++) {
        gemini::Tensor<uint64_t> coefficients_extract(gemini::TensorShape({static_cast<long>(upper)}));  // 创建一个足够大的 Tensor 来存储系数
        // 从每个多项式中提取特定系数
        size_t extract_index = 0;  // 用于追踪在 Tensor 中存储的有效系数位置
        for (size_t index = 0; index < randomVec_encoded[i].coeff_count(); ++index) {
            // 只提取特定位置的系数
            if (index < upper && index % split_shape_cols == 0) {
                coefficients_extract(extract_index++) = randomVec_encoded[i][index];
            }
        }
        // 将提取的系数向量调整为实际大小
        if (extract_index > static_cast<size_t>(std::numeric_limits<long>::max())) {
            // 处理溢出情况
            throw std::runtime_error("extract_index exceeds the maximum value that a 'long' can hold.");
        }
        gemini::Tensor<uint64_t> actual_extracted_coeffs(gemini::TensorShape({static_cast<long>(extract_index)}));
        for (size_t j = 0; j < extract_index; j++) {
            actual_extracted_coeffs(j) = coefficients_extract(j);
        }
        //向量extracted_coeffs_from_randomVec=[[r1,r3],[r1',r3'],...]
        extracted_coeffs_from_randomVec[i] = actual_extracted_coeffs;

        // Create a temporary Meta object with a new shape for actual_extracted_coeffs
        HomFCSS::Meta temp_meta;
        temp_meta.input_shape = TensorShape({actual_extracted_coeffs.length()});  // Adjust this shape as needed

        //将向量extracted_coeffs_from_randomVec[i]利用函数encodeInputVectorPositiveSequence编码为一个多项式r1+r3x，
        //最后得到新向量enc_z_i_overline'，最后把enc_z_i_overline'返回。
        std::vector<seal::Plaintext> encoded_extracted_coeffs_ploy;
        protocols.encodeInputVectorPositiveSequence(actual_extracted_coeffs, temp_meta, encoded_extracted_coeffs_ploy);
        evaluator.sub_plain(enc_z_overline_prime[i], encoded_extracted_coeffs_ploy[0], final_result[i]);
    }
    return final_result;
}



//PPCR协议
vector<Ciphertext> PPCR(const vector<Ciphertext> &input, const Evaluator &evaluator, Encryptor &encryptor, 
                Decryptor &decryptor, const HomFCSS &protocols, const HomFCSS::Meta &meta, size_t poly_modulus_degree) {
    std::vector<seal::Plaintext> randomVec_encoded;
    vector<Ciphertext> encrypted_z = s1AddMaskPPCRProtocol(input, randomVec_encoded, evaluator,decryptor, encryptor, protocols, meta, poly_modulus_degree);
    vector<Ciphertext> enc_z_overline_prime = s2PPCRProtocol(encrypted_z, decryptor, encryptor, protocols, meta);
    vector<Ciphertext> final_result = s1MinusMaskPPCRProtocol(enc_z_overline_prime, randomVec_encoded, evaluator,protocols, meta);
    return final_result;
}



int main() {
    /**测试PDC和PMV协议时用的参数设置
    // 设置BFV加密参数
    EncryptionParameters parms(scheme_type::bfv);
    size_t poly_modulus_degree = 8192;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    uint64_t plain_modulus = static_cast<uint64_t>(pow(2, 50));
    parms.set_plain_modulus(plain_modulus);

    //测试PDC和PMV协议时用的参数设置
    SEALContext context(parms);
    // 密钥生成
    KeyGenerator keygen(context);
    SecretKey sk = keygen.secret_key();
    PublicKey pk;
    keygen.create_public_key(pk);
    // 创建重新线性化密钥
    RelinKeys relin_keys;
    keygen.create_relin_keys(relin_keys);
    // 生成所需的对象
    Encryptor encryptor(context, pk);
    Evaluator evaluator(context);
    Decryptor decryptor(context, sk);
    // 设置 HomFCSS 组件
    HomFCSS protocols;
    protocols.setUp(context, sk, make_shared<PublicKey>(pk));

    int64_t value1 = 1;
    int64_t value2 = 115078;
    int64_t value3 = 654321;


    // 创建和初始化Meta信息
    HomFCSS::Meta meta;
    meta.input_shape = TensorShape({1});
    //meta.weight_shape = TensorShape({4, 6});  // 假设的权重形状
    meta.is_shared_input = false;  // 假设的共享输入标志

    Tensor<uint64_t> input_vector1(meta.input_shape);
    input_vector1(0) = value1;

    Tensor<uint64_t> input_vector2(meta.input_shape);
    input_vector2(0) = value2;

    Tensor<uint64_t> input_vector3(meta.input_shape);
    input_vector3(0) = value3;

    vector<seal::Serializable<seal::Ciphertext>> encrypted_share1;
    vector<seal::Serializable<seal::Ciphertext>> encrypted_share2;
    vector<seal::Serializable<seal::Ciphertext>> encrypted_share3;

    protocols.encryptOneValue(input_vector1, meta, encrypted_share1);
    protocols.encryptOneValue(input_vector2, meta, encrypted_share2);
    protocols.encryptOneValue(input_vector3, meta, encrypted_share3);
    

    // 反序列化 encrypted_share1 和 encrypted_share2
    Ciphertext enc_value1;
    stringstream ss1;
    encrypted_share1[0].save(ss1);
    ss1.seekg(0);
    enc_value1.load(context, ss1);

    Ciphertext enc_value2;
    stringstream ss2;
    encrypted_share2[0].save(ss2);
    ss2.seekg(0);
    enc_value2.load(context, ss2);

    Ciphertext enc_value3;
    stringstream ss3;
    encrypted_share3[0].save(ss3);
    ss3.seekg(0);
    enc_value3.load(context, ss3);

    //int result = DataComparFunc(enc_value1, enc_value2, evaluator, context, protocols, decryptor, meta, plain_modulus);
    //cout << "Comparison result: " << result << endl; // 如果 value1 < value2，结果应为 1；否则为 0

    vector<Ciphertext> ciphertexts = {enc_value1, enc_value2, enc_value3};
    Ciphertext max_ciphertext = PMVFunc(ciphertexts, evaluator, context, protocols, decryptor, meta);
    
    // 解密并输出最大值
    Plaintext max_plain;
    decryptor.decrypt(max_ciphertext, max_plain);
    cout << "Decrypted max value: ";
    print_plaintext(max_plain);*/

    //用于测试PPCR协议
    // 初始化加密参数
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

    // Call PPCR function
    //encrypted_result是向量和矩阵乘积后的加密多项式
    vector<Ciphertext> final_result = PPCR(encrypted_result, evaluator, encryptor, decryptor, hom_fcss, meta, poly_modulus_degree);

    // Decrypt and print the final results
    for (auto &ciphertext : final_result) {
        //当没分块的时候，ciphertext[0]=91+217x+343x^2+469x^3（目前是这个）
        //当分两块的时候，ciphertext[0]=91+217x的密文, ciphertext[1]=343+469x的密文.
        Plaintext final_plain;
        decryptor.decrypt(ciphertext, final_plain);
        cout << "Decrypted result: ";
        print_plaintext(final_plain);  // Implement this function if not already defined
    }

    return 0;
}
