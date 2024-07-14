#ifndef TOKEN_H
#define TOKEN_H

#include <vector>
#include <iostream>
#include <utility>
#include "Node.h"

#include <string>
#include <seal/seal.h>
#include "matVecMul.h"

#include <string>
#include <fstream>
#include <sstream>

using namespace std;
using namespace seal;
using namespace gemini;

class encToken {
public:
    seal::Ciphertext value;
    std::pair<seal::Ciphertext, seal::Ciphertext> coordinate;
    std::vector<seal::Ciphertext> vector1;
    std::vector<seal::Ciphertext> vector2;
};

class Token {
public:
    int value;
    std::pair<int, int> coordinate;
    std::vector<int> vector1;
    std::vector<int> vector2;
    Token(int value, int x, int y, size_t vector1Size = maxDimension, size_t vector2Size = maxDimension);
    static Token generateRandomToken(int value, int x, int y, int n);
    static int encryptToken(Token& token,encToken& encToken);
    static void print_plaintext(const Plaintext &plain);

    // static bool encryptTokenValue(HomFCSS& hom_fcss, const HomFCSS::Meta& meta, std::shared_ptr<seal::SEALContext> context, Tensor<uint64_t> input_value, std::vector<seal::Ciphertext>& encrypted_input_vector);    
    static int encryptTokenValue(int value, HomFCSS& hom_fcss, 
                        HomFCSS::Meta& meta, 
                        std::shared_ptr<seal::SEALContext> context, 
                        std::vector<seal::Ciphertext>& encrypted_input_vector);
    static std::pair<int, int> getCoordinateFromLine(const std::string& filename, int lineNumber);
    void print() const;
};




#endif // TOKEN_H
