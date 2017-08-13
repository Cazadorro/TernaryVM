//
// Created by Shae Bolt on 8/11/2017.
//

#ifndef TERNARYVM_TERNARY_OPERATORS_H
#define TERNARYVM_TERNARY_OPERATORS_H

#include <cstdint>
#include <string>
#include <array>
#include "rotate.h"


namespace tvm {
    // bits binary, T= 11, U = 01, F = 00, TUF, = 110100.
    constexpr std::uint64_t ALT_010101 = 0x5555555555555555ULL;
    constexpr std::uint64_t ALT_101010 = 0xAAAAAAAAAAAAAAAAULL;

    std::uint64_t native_to_10_encoded_ternary(std::uint64_t ternary_bits);

    std::uint64_t ternary_coded_bin_to_bin(std::uint64_t bits);

    std::uint64_t isolate_uknown_op(std::uint64_t bits_110100);

    std::uint64_t not_op(std::uint64_t bits_110100);

    std::uint64_t xor_op(std::uint64_t lhs_110100, std::uint64_t rhs_001101);

    std::uint64_t shl_op(std::uint64_t bits, std::uint64_t shl_amount);

    std::uint64_t shr_op(std::uint64_t bits, std::uint64_t shr_amount, std::uint64_t mask);

    std::uint64_t not_false_op(std::uint64_t bits_110100);

    std::uint64_t not_true_op(std::uint64_t bits_110100);

    std::uint64_t is_unknown_op(std::uint64_t bits);

    std::uint64_t not_unknown_op(std::uint64_t bits_110100);

    std::uint64_t is_true_op(std::uint64_t bits_110100);

    std::uint64_t unknown_only_true_op(std::uint64_t bits_110100);

    std::uint64_t unknown_only_false_op(std::uint64_t bits_110100);

    std::uint64_t is_false_op(std::uint64_t bits_110100);

    std::uint64_t sharkfin_op(std::uint64_t lhs, std::uint64_t rhs);

    std::uint64_t find_carry_op(std::uint64_t lhs, std::uint64_t rhs);

    std::uint64_t add_op(std::uint64_t x, std::uint64_t y);

    std::uint64_t sub_op(std::uint64_t x, std::uint64_t y);

    std::uint64_t comp_op(std::uint64_t lhs, std::uint64_t rhs);

    std::uint64_t rotl_op(std::uint64_t bits, std::uint64_t rotate_amount, std::uint64_t mask, std::uint64_t max_bits);

    std::uint64_t rotr_op(std::uint64_t bits, std::uint64_t rotate_amount, std::uint64_t mask, std::uint64_t max_bits);
}


#endif //TERNARYVM_TERNARY_OPERATORS_H
