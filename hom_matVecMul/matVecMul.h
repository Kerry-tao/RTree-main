//  Authors: Wen-jie Lu on 2021/9/14.
#ifndef HOM_MATVECMUL_MATVECMUL_H_
#define HOM_MATVECMUL_MATVECMUL_H_

#include <seal/seal.h>
#include <optional>
#include <vector>

#include "tensor.h"
#include "tensor_shape.h"
// #include "ThreadPool.h"

//  Authors: Wen-jie Lu on 2021/9/14.

#include <seal/secretkey.h>
#include <seal/serializable.h>

// Forward
namespace seal
{
  class SEALContext;
  class PublicKey;

  class Plaintext;
  class Ciphertext;
  class Evaluator;
  class UniformRandomGenerator;
} // namespace seal

namespace gemini
{

  class HomFCSS
  {
  public:
    /**#ifdef HOM_CONV2D_SS_MAX_THREADS
      static constexpr size_t kMaxThreads = HOM_CONV2D_SS_MAX_THREADS;
    #else
      static constexpr size_t kMaxThreads = 16;
    #endif*/
    struct Meta
    {
      TensorShape input_shape;
      TensorShape weight_shape;
      bool is_shared_input;
    };

    explicit HomFCSS() = default;

    ~HomFCSS() = default;

    Code setUp(const seal::SEALContext &context,
               std::optional<seal::SecretKey> sk = std::nullopt,
               std::shared_ptr<seal::PublicKey> pk = nullptr);

    [[nodiscard]] seal::scheme_type scheme() const;

    [[nodiscard]] size_t poly_degree() const;

    uint64_t plain_modulus() const;

    Code encryptInputVector(
        const Tensor<uint64_t> &vector, const Meta &meta,
        std::vector<seal::Serializable<seal::Ciphertext>> &encrypted_share) const;

    Code encodeInputVector(const Tensor<uint64_t> &vector, const Meta &meta,
                           std::vector<seal::Plaintext> &encoded_share) const;

    Code encryptWeightMatrix(
        const Tensor<uint64_t> &weight_matrix, const Meta &meta,
        std::vector<std::vector<seal::Serializable<seal::Ciphertext>>> &encrypted_share) const;

    Code matVecMul(const std::vector<std::vector<seal::Ciphertext>> &matrix,
                   const std::vector<seal::Ciphertext> &input_vec,
                   const Meta &meta,
                   std::vector<seal::Ciphertext> &out_vec_share0,
                   Tensor<uint64_t> &out_vec_share1,
                   const seal::RelinKeys &relin_keys) const;

    Code encryptOneValue(
        const Tensor<uint64_t> &input_vector, const Meta &meta,
        std::vector<seal::Serializable<seal::Ciphertext>> &encrypted_share) const;

    Code encodeOneValue(const Tensor<uint64_t> &input_vector,
                        const Meta &meta,
                        std::vector<seal::Plaintext> &encoded_share) const;

    Code encodeInputVectorPositiveSequence(const Tensor<uint64_t> &input_vector,
                                           const Meta &meta,
                                           std::vector<seal::Plaintext> &encoded_share) const;

    Code decryptToVector(const std::vector<seal::Ciphertext> &enc_vector,
                         const Meta &meta, Tensor<uint64_t> &out) const;

    Code idealFunctionality(const Tensor<uint64_t> &weight_matrix,
                            const Tensor<uint64_t> &vector, const Meta &meta,
                            Tensor<uint64_t> &out) const;

  protected:
    enum class Role
    {
      encryptor,
      encoder,
      masking,
      evaluator,
      none,
    };

    Code initPtx(seal::Plaintext &pt,
                 seal::parms_id_type pid = seal::parms_id_zero) const;

    Code vec2PolyBFV(const uint64_t *vec, size_t len, seal::Plaintext &pt,
                     bool is_ntt) const;

    Code vec2Poly(const uint64_t *vec, size_t len, seal::Plaintext &pt,
                  const Role role, bool is_ntt = false) const;

    Code sampleRandomMask(const std::vector<size_t> &targets,
                          uint64_t *coeffs_buff, size_t buff_size,
                          seal::Plaintext &mask, seal::parms_id_type pid,
                          std::shared_ptr<seal::UniformRandomGenerator> prng,
                          bool is_ntt) const;

    Code addRandomMask(std::vector<seal::Ciphertext> &enc_tensor,
                       Tensor<uint64_t> &mask_tensor, const Meta &meta) const;

    Code removeUnusedCoeffs(std::vector<seal::Ciphertext> &ct, const Meta &meta,
                            double *density = nullptr) const;

    void flood_ciphertext(seal::Ciphertext &ct,
                          std::shared_ptr<seal::UniformRandomGenerator> prng,
                          const seal::SEALContext &context,
                          const seal::PublicKey &pk,
                          const seal::Evaluator &evaluator) const;

    // new add
    void truncate_for_decryption(seal::Ciphertext &ct,
                                 const seal::Evaluator &evaluator,
                                 const seal::SEALContext &context) const;

    // new add
    Code sample_random_mask(const std::vector<size_t> &targets,
                            uint64_t *coeffs_buff, size_t buff_size,
                            seal::Plaintext &mask, seal::parms_id_type pid,
                            std::shared_ptr<seal::UniformRandomGenerator> prng,
                            const seal::SEALContext &context) const;

    void set_poly_coeffs_uniform(
        uint64_t *poly, int bitlen,
        std::shared_ptr<seal::UniformRandomGenerator> prng,
        const seal::EncryptionParameters &parms) const;

    void asymmetric_encrypt_zero(
        const seal::SEALContext &context, const seal::PublicKey &public_key,
        const seal::parms_id_type parms_id, bool is_ntt_form,
        std::shared_ptr<seal::UniformRandomGenerator> prng,
        seal::Ciphertext &destination) const;

    inline uint64_t make_bits_mask(int n_low_zeros) const;

    // 函数声明
    // void printPlaintext(const seal::Plaintext& pt);

  private:
    std::shared_ptr<seal::SEALContext> context_;
    std::shared_ptr<seal::Evaluator> evaluator_{nullptr};
    std::shared_ptr<seal::Encryptor> encryptor_{nullptr};
    std::shared_ptr<seal::PublicKey> pk_{nullptr};

    std::optional<seal::SecretKey> sk_{std::nullopt};
  };
} // namespace gemini

namespace gemini
{
  namespace internal
  {
    void sub_poly_inplace(seal::Ciphertext &ct, const seal::Plaintext &pt,
                          const seal::SEALContext &context,
                          const seal::Evaluator &evaluator);
  }
}

#endif // GEMINI_CHEETAH_HOM_FC_SS_H_
