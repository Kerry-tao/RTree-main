// matVecMul.cpp

#include "matVecMul.h"
#include <seal/seal.h>
#include <seal/secretkey.h>
#include <seal/util/polyarithsmallmod.h>
#include <seal/util/rlwe.h>
// #include <seal.h>
#include <vector>
#include <functional>
#include "logging.h"
#include "ThreadPool.h"  // 调整路径以匹配项目结构

using namespace seal;


namespace gemini {

//这个函数的核心作用是根据权重矩阵和输入向量的形状，计算出输出向量的形状.
static TensorShape GetOutShape(const HomFCSS::Meta &meta) {
  //比如meta.weight是3*6的矩阵，meta.input是6*1的向量。需要判断两个6是不是相等
  if (meta.weight_shape.dims() != 2 || meta.input_shape.dims() != 1 ||
      meta.input_shape.length() != meta.weight_shape.cols()) {
    LOG(WARNING) << "GetConv2DOutShape invalid meta";
  }
  //这意味着输出张量的形状只有一个维度，该维度的大小由权重的行数确定。也即输出3
  TensorShape outshape({meta.weight_shape.rows()});
  return outshape;
}

//用于计算如何最优地将给定的权重矩阵和输入向量分割成更小的块，以便处理（如在同态加密环境中）。这个函数的目的是找到在给定限制下的最小计算成本的分割方式。
static TensorShape getSplit(const HomFCSS::Meta &meta, size_t N) {
  if (meta.weight_shape.dims() != 2 || meta.input_shape.dims() != 1 ||
      meta.input_shape.length() != meta.weight_shape.cols()) {
    LOG(FATAL) << "getSplit invalid meta";
  }

  //size_t是一个无符号的整型数据类型，能保证值始终非负
  size_t nrows = meta.weight_shape.rows();
  size_t ncols = meta.weight_shape.cols();

  //ret{0} 是一个列表初始化表达式，用于初始化 std::array，数组中的元素被设置为 0。
  std::array<int64_t, 2> ret{0};
  //当-1被赋给一个 size_t 类型的变量时，会发生一个隐式的类型转换。-1对应的二进制在 64 位系统上则是 18446744073709551615
  //所以其实是给min_cost负了一个系统中的最大值，然后逐步寻求更小的cost
  size_t min_cost = -1;
  for (size_t d0 = 1; d0 <= std::min(N, nrows); ++d0) {
    for (size_t d1 = 1; d1 <= std::min(N, ncols); ++d1) {
      if (d0 * d1 > N) continue;
      size_t ct_in = CeilDiv(ncols, d1);
      size_t ct_out = CeilDiv(nrows, d0);
      size_t cost = ct_in + ct_out;
      if (cost < min_cost) {
        min_cost = cost;
        ret[0] = int64_t(d0);
        ret[1] = int64_t(d1);
      }
    }
  }
  return TensorShape({ret[0], ret[1]});
}

//主要功能是分配和管理在一个线程池中执行的工作任务。
/**static Code LaunchWorks(
    ThreadPool &tpool, size_t num_works,
    std::function<Code(long wid, size_t start, size_t end)> program) {
  if (num_works == 0) return Code::OK;
  const long pool_sze = tpool.pool_size();
  if (pool_sze <= 1L) {
    return program(0, 0, num_works);
  } else {
    Code code;
    std::vector<std::future<Code>> futures;
    size_t work_load = (num_works + pool_sze - 1) / pool_sze;
    for (long wid = 0; wid < pool_sze; ++wid) {
      size_t start = wid * work_load;
      size_t end = std::min(start + work_load, num_works);
      futures.push_back(tpool.enqueue(program, wid, start, end));
    }

    code = Code::OK;
    for (auto &&work : futures) {
      Code c = work.get();
      if (code == Code::OK && c != Code::OK) {
        code = c;
      }
    }
    return code;
  }
}*/

// defined in hom_conv2d_ss.cc
//&ct表示对 seal::Ciphertext 类型对象的一个引用。允许函数直接操作调用者提供的对象，而不是操作其副本。
void flood_ciphertext(seal::Ciphertext &ct,
                      std::shared_ptr<seal::UniformRandomGenerator> prng,
                      const seal::SEALContext &context,
                      const seal::PublicKey &pk,
                      const seal::Evaluator &evaluator);

// defined in hom_conv2d_ss.cc
void truncate_for_decryption(seal::Ciphertext &ct,
                             const seal::Evaluator &evaluator,
                             const seal::SEALContext &context);

// defined in hom_conv2d_ss.cc
Code sample_random_mask(const std::vector<size_t> &targets,
                        uint64_t *coeffs_buff, size_t buff_size,
                        seal::Plaintext &mask, seal::parms_id_type pid,
                        std::shared_ptr<seal::UniformRandomGenerator> prng,
                        const seal::SEALContext &context);

namespace internal {
void sub_poly_inplace(seal::Ciphertext &ct, const seal::Plaintext &pt,
                      const seal::SEALContext &context,
                      const seal::Evaluator &evaluator);
}

//用于获取当前同态加密方案的类型。
seal::scheme_type HomFCSS::scheme() const {
  if (context_) {
    return context_->first_context_data()->parms().scheme();
  } else {
    return seal::scheme_type::none;
  }
}
//用于获取多项式模数的度数
size_t HomFCSS::poly_degree() const {
  if (context_) {
    return context_->first_context_data()->parms().poly_modulus_degree();
  } else {
    return 0;
  }
}
//用于获取明文模数的值
uint64_t HomFCSS::plain_modulus() const {
  if (context_) {
    return context_->first_context_data()->parms().plain_modulus().value();
  } else {
    //作为错误或未初始化的标志
    return -1;
  }
}

//初始化公钥和私钥等参数
Code HomFCSS::setUp(const seal::SEALContext &context,
                    std::optional<seal::SecretKey> sk,
                    std::shared_ptr<seal::PublicKey> pk) {
  context_ = std::make_shared<seal::SEALContext>(context);
  ENSURE_OR_RETURN(context_, Code::ERR_NULL_POINTER);

  if (sk) {
    if (!seal::is_metadata_valid_for(*sk, *context_)) {
      LOG(WARNING) << "HomFCSS: invalid secret key for this SEALContext";
      return Code::ERR_INVALID_ARG;
    }

    //"*"是解引用操作符，被用来从指针或类似指针（如智能指针或 std::optional）的对象中获取它所指向的数据。
    sk_ = seal::SecretKey(*sk);
    encryptor_ = std::make_shared<seal::Encryptor>(*context_, *sk);
  }

  if (pk) {
    if (!seal::is_metadata_valid_for(*pk, *context_)) {
      LOG(WARNING) << "HomFCSS: invalid public key for this SEALContext";
      return Code::ERR_INVALID_ARG;
    }

    pk_ = std::make_shared<seal::PublicKey>(*pk);
  }
  //seal::Evaluator 类型的智能指针 evaluator_，用提供的加密上下文 context_ 进行初始化。Evaluator 用于执行各种同态加密算术操作，如加、乘等
  evaluator_ = std::make_shared<seal::Evaluator>(*context_);
  return Code::OK;
}

//用于初始化一个 seal::Plaintext 对象，这个对象在同态加密库 Microsoft SEAL 中用于存储和操作未加密的数据。
Code HomFCSS::initPtx(seal::Plaintext &pt, seal::parms_id_type pid) const {
  ENSURE_OR_RETURN(context_, Code::ERR_CONFIG);

  if (scheme() != seal::scheme_type::ckks) {
    // BFV or BGV
    pt.parms_id() = seal::parms_id_zero;  // foo SEAL when using BFV
    pt.resize(poly_degree());
    ENSURE_OR_RETURN(pt.data() != nullptr, Code::ERR_SEAL_MEMORY);
    return Code::OK;
  }

  if (pid == seal::parms_id_zero) {
    pid = context_->first_parms_id();
  }

  auto cntxt_data = context_->get_context_data(pid);
  ENSURE_OR_RETURN(cntxt_data != nullptr, Code::ERR_INVALID_ARG);
  const size_t num_moduli = cntxt_data->parms().coeff_modulus().size();
  const size_t num_elt = seal::util::mul_safe(num_moduli, poly_degree());
  pt.parms_id() = seal::parms_id_zero;  // foo SEAL when using BFV
  pt.resize(num_elt);
  pt.parms_id() = pid;
  ENSURE_OR_RETURN(pt.data() != nullptr, Code::ERR_SEAL_MEMORY);
  return Code::OK;
}

Code HomFCSS::vec2PolyBFV(const uint64_t *vec, size_t len, seal::Plaintext &pt,
                          bool is_to_ntt) const {
  if (scheme() != seal::scheme_type::bfv) {
    LOG(FATAL) << "A2HBFV: invalid scheme";
  }

  if (is_to_ntt) {
    LOG(WARNING) << "A2H: demand is_to_ntt = false for scheme bfv";
  }

  CHECK_ERR(initPtx(pt), "A2H: InitPtx");
  ENSURE_OR_RETURN(vec != nullptr, Code::ERR_NULL_POINTER);
  ENSURE_OR_RETURN(len > 0 && len <= poly_degree(), Code::ERR_OUT_BOUND);

  //将数组 vec 中的数据转换成多项式系数，并自动对每个系数进行模 plain_modulus() 的运算。
  seal::util::modulo_poly_coeffs(vec, len, plain_modulus(), pt.data());
  //将多项式剩余的系数（从 len 到 pt.coeff_count()）填充为 0
  std::fill_n(pt.data() + len, pt.coeff_count() - len, 0);

  return Code::OK;
}

//只在BFV机制下将输入向量转换为多项式
Code HomFCSS::vec2Poly(const uint64_t *vec, size_t len, seal::Plaintext &pt,
                       const Role role, bool is_to_ntt) const {
  switch (scheme()) {
    case seal::scheme_type::bfv:
      return vec2PolyBFV(vec, len, pt, is_to_ntt);
    default:
      LOG(WARNING) << "A2H: shceme is not supported yet\n";
  }
  return Code::ERR_INTERNAL;
}

//输出是密文（从<seal::Ciphertext>> &encrypted_share中看出来的）
Code HomFCSS::encryptInputVector(
    const Tensor<uint64_t> &input_vector, const Meta &meta,
    std::vector<seal::Serializable<seal::Ciphertext>> &encrypted_share) const {
  ENSURE_OR_RETURN(context_ && encryptor_, Code::ERR_CONFIG);
  ENSURE_OR_RETURN(input_vector.shape().IsSameSize(meta.input_shape),
                   Code::ERR_DIM_MISMATCH);
  ENSURE_OR_RETURN(meta.input_shape.num_elements() > 0, Code::ERR_INVALID_ARG);

  const bool is_ckks = scheme() == seal::scheme_type::ckks;
  ENSURE_OR_RETURN(!is_ckks, Code::ERR_INTERNAL);

  auto split_shape = getSplit(meta, poly_degree());
  //如果split_shape={2,3}，那么split_shape.num_elements()会返回2*3=6，其中split_shape={2,3} 表示每个块有2行3列
  if (split_shape.num_elements() > poly_degree()) {
    LOG(FATAL) << "BUG";
  }
  //nout=6/3=2，也就是要生成2个密文多项式
  auto nout = CeilDiv(input_vector.length(), split_shape.cols());

  Role encode_role = Role::none;  // BFV/BGV not use this role
  //初始化密文向量，预填充使用加密器生成的零值密文
  encrypted_share.resize(nout, encryptor_->encrypt_zero());

  seal::Plaintext pt;
  //tmp 数组的大小是度N
  std::vector<uint64_t> tmp(poly_degree());
  const uint64_t plain = plain_modulus();
  bool is_failed = false;
  for (size_t i = 0; i < nout && !is_failed; ++i) {
    auto start = i * split_shape.cols();
    auto end =
        std::min<size_t>(input_vector.length(), start + split_shape.cols());
    auto len = end - start;
    // reversed ordering for the vector（这里是为了实现映射函数\tau^v'=b0-0x-0x^2-b2x^3-b1x^4）

    //例如输入向量 input_vector=[3, 5, 7, 2, 9]，若start=0，end=3，那么要处理的元素就是[b0,b1,b2]=[3,5,7]，此时tmp[0]=3
    tmp[0] = input_vector(start);

    //若start=0，end=3，那么输入元素5，计算-5：10 - 5 = 5。输入元素7，计算-7：10 - 7 = 3。
    //也就是tmp.size()=N=5，第一个元素保持b0=3不变，然后将其余元素逆序排列再放回tmp中，不够的补0，最后tmp=[3,0,0,3,5]。
    //input_vector.data() + start + 1 = 1, input_vector.data() + end = 3(不包括3)
    std::transform(input_vector.data() + start + 1, input_vector.data() + end,
                   tmp.rbegin(),
                   [plain](uint64_t u) { return u > 0 ? plain - u : 0; });
    if (len < tmp.size()) {
      //不够的补0
      std::fill_n(tmp.rbegin() + len, tmp.size() - len - 1, 0);
    }

    //经过vec2Poly函数后得到多项式f=3+0X+...+0X+3X^6+5X^7
    if (Code::OK != vec2Poly(tmp.data(), tmp.size(), pt, encode_role, false)) {
      is_failed = true;
    } else {
      try {
        //使用 encryptor_ 对象对明文 pt 进行对称加密，并将结果存储在 encrypted_share 的相应位置。
        encrypted_share.at(i) = encryptor_->encrypt_symmetric(pt);
      } catch (const std::logic_error &e) {
        is_failed = true;
      }
    }
  }

  // erase the sensitive data
  seal::util::seal_memzero(tmp.data(), sizeof(uint64_t) * tmp.size());
  seal::util::seal_memzero(pt.data(), sizeof(uint64_t) * pt.coeff_count());

  if (is_failed) {
    return Code::ERR_INTERNAL;
  } else {
    return Code::OK;
  }
}

//输出是明文对象（从<seal::Plaintext> &encoded_share中看出来的）
/**Code HomFCSS::encodeInputVector(const Tensor<uint64_t> &input_vector,
                                const Meta &meta,
                                std::vector<seal::Plaintext> &encoded_share) const {
  ENSURE_OR_RETURN(context_, Code::ERR_CONFIG);
  ENSURE_OR_RETURN(input_vector.shape().IsSameSize(meta.input_shape),
                   Code::ERR_DIM_MISMATCH);
  ENSURE_OR_RETURN(meta.input_shape.num_elements() > 0, Code::ERR_INVALID_ARG);

  const bool is_ckks = scheme() == seal::scheme_type::ckks;
  ENSURE_OR_RETURN(!is_ckks, Code::ERR_INTERNAL);

  auto split_shape = getSplit(meta, poly_degree());
  if (split_shape.num_elements() > poly_degree()) {
    LOG(FATAL) << "BUG";
  }
  auto nout = CeilDiv(input_vector.length(), split_shape.cols());

  Role encode_role = Role::none;  // BFV/BGV not use this role
  encoded_share.resize(nout);

  auto encode_prg = [&](long wid, size_t start, size_t end) {
    std::vector<uint64_t> tmp(poly_degree());
    const uint64_t plain = plain_modulus();
    bool is_failed = false;
    for (size_t i = start; i < end && !is_failed; ++i) {
      auto start = i * split_shape.cols();
      auto end =
          std::min<size_t>(input_vector.length(), start + split_shape.cols());
      auto len = end - start;

      // reversed ordering for the vector
      tmp[0] = input_vector(start);
      std::transform(input_vector.data() + start + 1, input_vector.data() + end,
                     tmp.rbegin(),
                     [plain](uint64_t u) { return u > 0 ? plain - u : 0; });
      if (len < tmp.size()) {
        std::fill_n(tmp.rbegin() + len, tmp.size() - len - 1, 0);
      }
      if (Code::OK != vec2Poly(tmp.data(), tmp.size(), encoded_share.at(i),
                               encode_role, false)) {
        is_failed = true;
      }
    }
    seal::util::seal_memzero(tmp.data(), sizeof(uint64_t) * tmp.size());
    return is_failed ? Code::ERR_INTERNAL : Code::OK;
  };

  gemini::ThreadPool tpool(nthreads);
  return LaunchWorks(tpool, nout, encode_prg);
}*/

//const Meta &meta: 包含权重矩阵形状等元数据的结构。输出是密文（因为seal::Serializable<seal::Ciphertext> &encrypted_share)
Code HomFCSS::encryptWeightMatrix(
    const Tensor<uint64_t> &weight_matrix, const Meta &meta,
    std::vector<std::vector<seal::Serializable<seal::Ciphertext>>> &encrypted_share//二维向量
    ) const {
  ENSURE_OR_RETURN(context_, Code::ERR_CONFIG);
  ENSURE_OR_RETURN(weight_matrix.shape().IsSameSize(meta.weight_shape),
                   Code::ERR_DIM_MISMATCH);
  const size_t nrows = meta.weight_shape.rows();
  const size_t ncols = meta.weight_shape.cols();

  auto split_shape = getSplit(meta, poly_degree());
  if (split_shape.num_elements() > poly_degree()) {
    LOG(FATAL) << "BUG";
  }

  const auto n_row_blks = CeilDiv<size_t>(nrows, split_shape.rows());
  const auto n_col_blks = CeilDiv<size_t>(ncols, split_shape.cols());
  
  Role encode_role = Role::none;  // BFV/BGV not use this role


  //encoded_share.resize(n_row_blks);

  //size_t start, size_t end定义了当前线程负责处理的行块的开始和结束索引
  //auto encode_prg = [&](long wid, size_t start, size_t end) {
  bool is_failed = false;
  seal::Plaintext pt;
  std::vector<uint64_t> tmp(poly_degree());
    //双层循环把大矩阵分成多个小矩阵
    //例如大矩阵是[[123456][789 10 11 12][13 14 15 16 17 18][19 20 21 22 23 24]]
    //小矩阵分别是[[123][789]]、[[456][10 11 12]]、[[13 14 15][19 20 21]]、[[16 17 18][22 23 24]]
  for (size_t r_blk = 0; r_blk < n_row_blks && !is_failed; ++r_blk) {
    //初始化密文向量，预填充使用加密器生成的零值密文
    encrypted_share[r_blk].resize(n_col_blks, encryptor_->encrypt_zero());

      //当r_blk=0,top_left_row=0,row_extent=2
      //当r_blk=1,top_left_row=2,row_extent=2
    auto top_left_row = r_blk * split_shape.rows();
    auto top_right_row =
          std::min<size_t>(top_left_row + split_shape.rows(), nrows);
    auto row_extent = top_right_row - top_left_row;

    for (size_t c_blk = 0; c_blk < n_col_blks; ++c_blk) {
        //当c_blk=0,top_left_col=0,col_extent=3
        //当c_blk=1,top_left_col=3,col_extent=3
      auto top_left_col = c_blk * split_shape.cols();
      auto col_extent =
            std::min<size_t>(top_left_col + split_shape.cols(), ncols) -
            top_left_col;
        // Encode the sub-matrix start ad (top_left_row, top_left_col) with
        // sizes (row_extent, col_extent) Matrix is stored in row-major
        //指向子块1的起始元素（1）。
      auto src_ptr =
            weight_matrix.data() + top_left_row * ncols + top_left_col;
        //指向 tmp 数组的起始位置。
      auto dst_ptr = tmp.begin();
        //对于第一个子块[[123][789]]：
        //第一行复制：1, 2, 3 被复制到 tmp。行内零填充：无需填充，因为列数等于块宽度。更新 src_ptr 到下一行起始。
        //第二行复制：7, 8, 9 被复制到 tmp 的下一个位置。
      for (size_t r = 0; r < row_extent; ++r) {
        std::copy_n(src_ptr, col_extent, dst_ptr);
          // For the right-most submatrtix, we might need zero-padding.
        size_t nzero_pad = split_shape.cols() - col_extent;
          //如果当前列块是最右边的列块（即列的数量少于块宽度），则在复制后进行零填充以保持块的固定宽度。
        if (nzero_pad > 0) {
          std::fill_n(dst_ptr + col_extent, nzero_pad, 0);
        }
          //指向 tmp 数组中下一组要复制过来的数据的起始存储位置
        dst_ptr += split_shape.cols();
          //更新 src_ptr 到下一行起始。
        src_ptr += ncols;
      }
        // zero-out the other coefficients
      std::fill(dst_ptr, tmp.end(), 0);
        //综上之后，得到tmp = [1, 2, 3, 7, 8, 9, 0, 0]
      if (Code::OK != vec2Poly(tmp.data(), tmp.size(),
                                 pt, encode_role,
                                 false)) {
        is_failed = true;
        break;
      } else {
        try{
          //使用 encryptor_ 对象对明文 pt 进行对称加密，并将结果存储在 encrypted_share 的相应位置。
          encrypted_share.at(r_blk).at(c_blk) = encryptor_->encrypt_symmetric(pt);
        } catch (const std::logic_error &e) {
        is_failed = true;

      }

        //经过函数vec2Poly后得到的多项式f=1+2X+3X^2+7X^3+8X^4+9X^5+0X^6+0X^7
        //再加密
    }
  }
  seal::util::seal_memzero(tmp.data(), sizeof(uint64_t) * tmp.size());
  seal::util::seal_memzero(pt.data(), sizeof(uint64_t) * pt.coeff_count());

  return is_failed ? Code::ERR_INTERNAL : Code::OK;
  //};

  //ThreadPool tpool(nthreads);
  //return LaunchWorks(tpool, n_row_blks, encode_prg);
  }
}

//向量是input_vec=[3, 5, 7, 2, 9 ,5]
//matrix是用encodeWeightMatrix函数编码后的矩阵，也就是[1 2 3]是以一个多项式的形式存在的，因为matrix的类型和encodeWeightMatrix函数的输出一模一样
//改了：改成encryptWeightMatrix输出密文的矩阵了
Code HomFCSS::matVecMul(const std::vector<std::vector<seal::Ciphertext>> &matrix,
                        const std::vector<seal::Ciphertext> &input_vec,
                        const Meta &meta,
                        std::vector<seal::Ciphertext> &out_share0,
                        Tensor<uint64_t> &out_share1) const {
  ENSURE_OR_RETURN(context_ && evaluator_, Code::ERR_CONFIG);

  auto split_shape = getSplit(meta, poly_degree());
  //n_ct_in 表示输入向量分块的数量
  const size_t n_ct_in =
      CeilDiv<size_t>(meta.input_shape.length(), split_shape.cols());
  //n_ct_out 表示输出向量的长度，即计算结果的分块数。
  const size_t n_ct_out =
      CeilDiv<size_t>(meta.weight_shape.rows(), split_shape.rows());

  ENSURE_OR_RETURN(input_vec.size() == n_ct_in, Code::ERR_INVALID_ARG);
  ENSURE_OR_RETURN(matrix.size() == n_ct_out, Code::ERR_INVALID_ARG);
  for (const auto &c : matrix) {
    ENSURE_OR_RETURN(c.size() == n_ct_in, Code::ERR_INVALID_ARG);
  }

  /**for (const auto &rows : matrix) {
    for (const auto &submat : rows) {
      if (submat.is_zero()) {
        LOG(WARNING) << "matVecMul: sub-matrix with all zero is not supported.\
	                     Maybe to use a larger fixed-point scaling factor\n";
		return Code::ERR_INVALID_ARG;
      }
    }
  }*/

  //如果是共享输入（多方同时提供输入的情况），检查明文部分的向量大小是否正确。
  //if (meta.is_shared_input && vec_share1.size() != n_ct_in) {
    //return Code::ERR_DIM_MISMATCH;
  //}

  //ThreadPool tpool(nthreads);

  //std::vector<seal::Ciphertext> input;
  //如果是共享输入，组合明文和密文向量以形成完整的输入向量。
  /**if (meta.is_shared_input) {
    input.resize(n_ct_in);
    auto add_prg = [&](long wid, size_t start, size_t end) {
      for (size_t i = start; i < end; ++i) {
        evaluator_->add_plain(vec_share0[i], vec_share1[i], input[i]);
      }
      return Code::OK;
    };
    //(void)LaunchWorks(tpool, n_ct_in, add_prg);
  }*/

  out_share0.resize(n_ct_out);
  //auto fma_prg = [&](long wid, size_t start, size_t end) {
    //end=2
  //for (size_t j = start; j < end; ++j) {
  for (size_t j = 0; j < n_ct_out; ++j) {
      //假设N=8
      //j=0时，matrix[j][0]=[1 2 3 7 8 9 0 0]对应的多项式，input_vec[0]为[3 0 0 0 0 0 3 5]对应的多项式(编码前的向量为[3, 5, 7]）
    evaluator_->multiply(input_vec[0], matrix[j][0], out_share0[j]);

      //vec_share0[0], matrix[j][0],out_share0[j]);
      // TODO(wen-jie): to implement FMA
    for (size_t i = 1; i < n_ct_in; ++i) {
      seal::Ciphertext tmp;
        //i=1时，matrix[j][1]=[4 5 6 10 11 12 0 0]对应的多项式，input_vec[1]=[2 0 0 0 0 0 5 1]对应的多项式(编码前的向量为[2, 9, 5]）
      evaluator_->multiply(input_vec[i], matrix[j][i], tmp);
      evaluator_->add_inplace(out_share0[j], tmp);
    }
  }
  return Code::OK;
  //};
  //(void)LaunchWorks(tpool, n_ct_out, fma_prg);
  
  //out_share0是(□+X+X^2+□X^3+...，△+X+X^2+△X^3+...)，其中第一个□是[1 2 3 4 5 6]和向量乘之后的内积，第二个□是[7 8 9 10 11 12]和向量的内积。△是类似的
  //out_share1用来存储随机掩码向量
  addRandomMask(out_share0, out_share1, meta);

  if (scheme() == seal::scheme_type::bfv) {
    for (auto &c : out_share0) {
      truncate_for_decryption(c, *evaluator_, *context_);
    }
  }

  // Post-processing for compressing out_ct volume.
  removeUnusedCoeffs(out_share0, meta);
  return Code::OK;
}


Code HomFCSS::addRandomMask(std::vector<seal::Ciphertext> &cts,
                            Tensor<uint64_t> &mask_vector, const Meta &meta) const {
  ENSURE_OR_RETURN(pk_, Code::ERR_CONFIG);
  TensorShape split_shape = getSplit(meta, poly_degree());
  const size_t n_ct_out =
      CeilDiv<size_t>(meta.weight_shape.rows(), split_shape.rows());
  ENSURE_OR_RETURN(cts.size() == n_ct_out, Code::ERR_INVALID_ARG);

  std::vector<size_t> targets(split_shape.rows());
  for (size_t i = 0; i < targets.size(); ++i) {
    targets[i] = i * split_shape.cols();
  }

  //auto mask_prg = [&](long wid, size_t start, size_t end) {
  RLWECt zero;
  RLWEPt mask;
  std::vector<U64> coeffs(targets.size());
  mask_vector.Reshape(GetOutShape(meta));
    //初始化一个新的随机数生成器，用于在同态加密操作中生成随机数
  auto prng =
        context_->first_context_data()->parms().random_generator()->create();
  for (size_t r_blk = 0; r_blk < n_ct_out; ++r_blk) {
      //获取 cts 向量中第 r_blk 个元素的引用
    auto &this_ct = cts.at(r_blk);

      //flood_ciphertext 函数的作用是对密文进行"淹没"处理，通过添加随机噪声和加密的零值来增加密文的安全性。
    flood_ciphertext(this_ct, prng, *context_, *pk_, *evaluator_);
      //sampleRandomMask生成随机掩码 mask，并将目标位置的系数存储在 coeffs 中。也就是原论文图2中的随机数r
    CHECK_ERR(
          sampleRandomMask(targets, coeffs.data(), coeffs.size(), mask,
                           this_ct.parms_id(), prng, this_ct.is_ntt_form()),
          "RandomMaskPoly");
    internal::sub_poly_inplace(this_ct, mask, *context_, *evaluator_);

    auto row_bgn = r_blk * split_shape.rows();
    auto row_end = std::min<size_t>(row_bgn + split_shape.rows(),
                                      meta.weight_shape.rows());
    auto coeffs_ptr = coeffs.data();
      //计算当前块的起始和结束行索引，并将系数 coeffs 中的值复制到 mask_vector 中。(应该就是记录一下r向量)
    for (size_t r = row_bgn; r < row_end; ++r) {
      mask_vector(r) = *coeffs_ptr++;
    }
  }

    seal::util::seal_memzero(coeffs.data(), sizeof(uint64_t) * coeffs.size());
    seal::util::seal_memzero(mask.data(),
                             sizeof(uint64_t) * mask.coeff_count());
    return Code::OK;
  //};

  //return LaunchWorks(tpool, n_ct_out, mask_prg);
}

// In our Cheetah paper, we export the needed coefficients using the Extract
// function. Indeed this semantic can be implemented by zero-out the un-used
// coefficients.
Code HomFCSS::removeUnusedCoeffs(std::vector<seal::Ciphertext> &cts,
                                 const Meta &meta, double *density) const {
  TensorShape out_shape = GetOutShape(meta);
  ENSURE_OR_RETURN(out_shape.num_elements() > 0, Code::ERR_INVALID_ARG);
  ENSURE_OR_RETURN(context_ && evaluator_, Code::ERR_CONFIG);

  TensorShape split_shape = getSplit(meta, poly_degree());
  const size_t n_ct_out =
      CeilDiv<size_t>(meta.weight_shape.rows(), split_shape.rows());
  ENSURE_OR_RETURN(cts.size() == n_ct_out, Code::ERR_INVALID_ARG);

  if (density) *density = 0.;

  for (size_t r_blk = 0; r_blk < n_ct_out; ++r_blk) {
    auto &this_ct = cts[r_blk];
    auto row_bgn = r_blk * split_shape.rows();
    auto row_end = std::min<size_t>(row_bgn + split_shape.rows(),
                                    meta.weight_shape.rows());
    //只是为了计算一个upper，不会用r_blk去遍历。upper的意思是说，这个块里一共有upper个元素，由于我们保证了内积一定在前N项的
    //系数里，所以小于upper的系数就已经都包含内积了。
    auto upper = (row_end - row_bgn) * split_shape.cols();

    //对于每个多项式的索引
    for (size_t index = 0; index < poly_degree(); ++index) {
      //index % split_shape.cols() == 0的意思是：我们要提取的系数，一定都在split_shape.cols()的整数倍的位置。
      //比如当split_shape.cols()=3时，□+X+X^2+□X^3+...中，我们需要的系数正好在第0*3,1*3的位置（不包含2*3，因为2*3不小于upper）
      //在我们的例子中，upper=6个元素。
      if (index < upper && index % split_shape.cols() == 0) {
        if (density) *density += 1;
        //不需要清零的系数就直接跳下一次循环了
        continue;
      }

      auto this_ct_ptr = this_ct.data();
      //coeff_modulus_size()返回密文中系数模数的数量，假设是3（他是假设有三个模数，但在BFV方案里，应该只有一个模数q）。
      for (size_t L = 0; L < this_ct.coeff_modulus_size(); ++L) {
        this_ct_ptr[index] = 0;
        //移动指针到下一个模数链的位置
        /**比如this_ct当前是[
        [a0, a1, a2, a3, a4, a5, a6, a7],    // 在模数 q_0 下的系数
        [b0, b1, b2, b3, b4, b5, b6, b7],    // 在模数 q_1 下的系数
        [c0, c1, c2, c3, c4, c5, c6, c7]     // 在模数 q_2 下的系数
        ]
        例如当index=1时，指针移动多项式系数个，正好是从a1移动到b1
        */
        this_ct_ptr += poly_degree();
      }
    }
  }

  if (density) *density /= cts.size();
  return Code::OK;
}

Code HomFCSS::sampleRandomMask(
    const std::vector<size_t> &targets, uint64_t *coeffs_buff, size_t buff_size,
    seal::Plaintext &mask, seal::parms_id_type pid,
    std::shared_ptr<seal::UniformRandomGenerator> prng, bool /*is_ntt*/) const {
  ENSURE_OR_RETURN(context_, Code::ERR_CONFIG);
  return sample_random_mask(targets, coeffs_buff, buff_size, mask, pid, prng,
                            *context_);
}


//将密文向量解密为明文向量,并存储在 out 中
/**假设我们有以下参数：

多项式度数 (poly_degree) = 8
密文向量 (enc_vector) 的大小为 2
每个密文块对应一个 2x3 的子矩阵 (split_shape.rows() = 2, split_shape.cols() = 3)
权重矩阵的行数 (meta.weight_shape.rows()) = 4
假设输入
假设我们的密文向量 enc_vector 中包含了以下密文块：

enc_vector[0]=□+X+X^2+□X^3+...: 对应前两行（第 0 行和第 1 行，也即[1 2 3 4 5 6]和[7 8 9 10 11 12]）
enc_vector[1]=△+X+X^2+△X^3+...: 对应最后两行（第 2 行和第 3 行，也即[13 14 15 16 17 18]和[19 20 21 22 23 24]） */
Code HomFCSS::decryptToVector(const std::vector<seal::Ciphertext> &enc_vector,
                              const Meta &meta, Tensor<uint64_t> &out) const {
  ENSURE_OR_RETURN(context_ && evaluator_ && sk_, Code::ERR_CONFIG);

  auto split_shape = getSplit(meta, poly_degree());
  auto n_ct_out = CeilDiv<size_t>(meta.weight_shape.rows(), split_shape.rows());

  if (n_ct_out != enc_vector.size()) {
    printf("expect %zd but got %zd\n", n_ct_out, enc_vector.size());
  }

  ENSURE_OR_RETURN(enc_vector.size() == n_ct_out, Code::ERR_INVALID_ARG);
  TensorShape out_shape = GetOutShape(meta);
  out.Reshape(out_shape);
  seal::Decryptor decryptor(*context_, *sk_);
  seal::Plaintext pt;
  /**遍历每个输出块进行解密：
  第 0 块：
  对应的行范围是 row_bgn = 0, row_end = 2（第 0 行和第 1 行）
  假设解密后的明文 pt 包含 [a0, a1, a2, a3, a4, a5, a6, a7](也即多项式的系数)，那么：
  第 0 行：out(0) = a0
  第 1 行：out(1) = a4*/
  for (size_t r_blk = 0; r_blk < n_ct_out; ++r_blk) {
    decryptor.decrypt(enc_vector.at(r_blk), pt);
    auto row_bgn = r_blk * split_shape.rows();
    auto row_end = std::min<size_t>(row_bgn + split_shape.rows(),
                                    meta.weight_shape.rows());

    for (size_t r = row_bgn; r < row_end; ++r) {
      size_t coeff_idx = (r - row_bgn) * split_shape.cols();
      out(r) = coeff_idx >= pt.coeff_count() ? 0 : pt[coeff_idx];
    }
  }
  //输出的out = [a0, a4, b0, b4]
  return Code::OK;
}

Code HomFCSS::idealFunctionality(const Tensor<uint64_t> &input_matrix,
                                 const Tensor<uint64_t> &weight_matrix,
                                 const Meta &meta,
                                 Tensor<uint64_t> &out) const {
  return Code::OK;
}
}  // namespace gemini
