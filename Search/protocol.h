#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <seal/seal.h>
#include <vector>
#include "matVecMul.h"

using namespace std;
using namespace seal;
using namespace gemini;

void print_plaintext(const Plaintext &plain);
void print_ciphertext_metadata(const Ciphertext &cipher);

Ciphertext s1DataComparProcotol(const Ciphertext &ciphertext1, const Ciphertext &ciphertext2,  Decryptor &decryptor, const Evaluator &evaluator,
 const SEALContext &context, HomFCSS &protocols, const HomFCSS::Meta &meta, uint64_t plain_modulus);
int s2DataComparProcotol(const Ciphertext &enc_z, Decryptor &decryptor, uint64_t plain_modulus);
int DataComparFunc(const Ciphertext &ciphertext1, const Ciphertext &ciphertext2, const Evaluator &evaluator, 
const SEALContext &context, HomFCSS &protocols, Decryptor &decryptor, const HomFCSS::Meta &meta, uint64_t plain_modulus);


                                    
std::vector<std::pair<Ciphertext, uint64_t>> s1PMVProtocol(const std::vector<Ciphertext> &ciphertexts,
                                                         const Evaluator &evaluator, const SEALContext &context,
                                                          HomFCSS &protocols, const HomFCSS::Meta &meta);
int s2PMVProtocol(const vector<Ciphertext> &encrypted_results, Decryptor &decryptor);
Ciphertext PMVFunc(const std::vector<Ciphertext> &ciphertexts, const Evaluator &evaluator, const SEALContext &context,
                     HomFCSS &protocols, Decryptor &decryptor, const HomFCSS::Meta &meta);



vector<Ciphertext> s1AddMaskPPCRProtocol(const vector<Ciphertext> &encrypted_result, std::vector<seal::Plaintext> &randomVec_encoded, 
                            const Evaluator &evaluator, const Encryptor &encryptor, const HomFCSS &protocols, 
                            const HomFCSS::Meta &meta, size_t poly_modulus_degree);
vector<Ciphertext> s2PPCRProtocol(const vector<Ciphertext> &encrypted_z, Decryptor &decryptor, Encryptor &encryptor, 
                    const HomFCSS &protocols, const HomFCSS::Meta &meta);
vector<Ciphertext> s1MinusMaskPPCRProtocol(const vector<Ciphertext> &enc_z_overline_prime, const vector<Plaintext>
         &randomVec_encoded, const Evaluator &evaluator, const HomFCSS &protocols, const HomFCSS::Meta &meta);
vector<Ciphertext> PPCR(const vector<Ciphertext> &input, const Evaluator &evaluator, Encryptor &encryptor, 
                Decryptor &decryptor, const HomFCSS &protocols, const HomFCSS::Meta &meta, size_t poly_modulus_degree);
                    


#endif // PROTOCOL_H
