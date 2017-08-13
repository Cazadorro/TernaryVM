//
// Created by Shae Bolt on 8/11/2017.
//

#include "ternary_operators.h"

namespace tvm {


    std::uint64_t native_to_10_encoded_ternary(std::uint64_t ternary_bits) {
        return ((ternary_bits & ALT_101010) >> 1) ^ ternary_bits;
    }

    std::uint64_t ternary_coded_bin_to_bin(std::uint64_t bits) {
        std::uint64_t acc = native_to_10_encoded_ternary(bits);
        acc = acc - (((acc >> 2) & 0x3333333333333333) * (4 - 3));
        acc = acc - (((acc >> 4) & 0x0F0F0F0F0F0F0F0F) * (16 - 9));
        acc = acc - (((acc >> 8) & 0x00FF00FF00FF00FF) * (256 - 81));
        acc = acc - (((acc >> 16) & 0x0000FFFF0000FFFF) * (65536 - 6561));
        acc = acc - (((acc >> 32) & 0x00000000FFFFFFFF) * (4294967296ULL - 43046721));
        return acc;
    }

    std::uint64_t isolate_uknown_op(std::uint64_t bits_110100) {
        std::uint64_t conv_bits = native_to_10_encoded_ternary(bits_110100);
        return conv_bits & ALT_010101;
    }

    std::uint64_t not_op(std::uint64_t bits_110100) {
        std::uint64_t inv_bits_001011 = ~bits_110100;
        std::uint64_t isolated_01 = isolate_uknown_op(bits_110100);
        std::uint64_t isolated_shr_10 = isolated_01 << 1;
        std::uint64_t removed_u = inv_bits_001011 ^isolated_shr_10;
        return removed_u ^ isolated_01;
    }

    std::uint64_t xor_op(std::uint64_t lhs_110100, std::uint64_t rhs_001101) {
        std::uint64_t lhs_010100 = lhs_110100 & ALT_010101;
        std::uint64_t rhs_000101 = rhs_001101 & ALT_010101;
        std::uint64_t lrs_000100 = lhs_010100 & rhs_000101;
        std::uint64_t lrs_001100 = lrs_000100 | (lrs_000100 << 1);
        std::uint64_t lrs_110011 = ~lrs_001100;
        std::uint64_t xor_111001 = lhs_110100 ^rhs_001101;
        std::uint64_t xor_110001 = xor_111001 & lrs_110011;
        return xor_110001 | lrs_000100;
    }

    std::uint64_t shl_op(std::uint64_t bits, std::uint64_t shl_amount) {

        std::uint64_t bin_shl_amount = ternary_coded_bin_to_bin(shl_amount);
        return bits << (bin_shl_amount * 2);
    }

    std::uint64_t shr_op(std::uint64_t bits, std::uint64_t shr_amount, std::uint64_t mask) {
        std::uint64_t masked_bits = bits & mask;
        std::uint64_t bin_shr_amount = ternary_coded_bin_to_bin(shr_amount);
        return masked_bits >> (bin_shr_amount * 2);
    }

    std::uint64_t not_false_op(std::uint64_t bits_110100) {
        std::uint64_t bits_010100 = bits_110100 & ALT_010101;
        return bits_010100 | (bits_010100 << 1);
    }

    std::uint64_t not_true_op(std::uint64_t bits_110100) {
        std::uint64_t bits_001010 = ~bits_110100 & ALT_101010;
        return bits_001010 | (bits_001010 >> 1);
    }

    std::uint64_t is_unknown_op(std::uint64_t bits) {
        std::uint64_t isolated_unknown = isolate_uknown_op(bits);
        return isolated_unknown | (isolated_unknown << 1);
    }

    std::uint64_t not_unknown_op(std::uint64_t bits_110100) {
        return ~is_unknown_op(bits_110100);
    }

    std::uint64_t is_true_op(std::uint64_t bits_110100) {
        std::uint64_t bits_100000 = bits_110100 & ALT_101010;
        return bits_100000 | (bits_100000 >> 1);
    }

    std::uint64_t unknown_only_true_op(std::uint64_t bits_110100) {
        std::uint64_t bits_100000 = bits_110100 & ALT_101010;
        return (bits_100000 >> 1);
    }

    std::uint64_t unknown_only_false_op(std::uint64_t bits_110100) {
        std::uint64_t bits_000001 = ~bits_110100 & ALT_010101;
        return bits_000001;
    }

    std::uint64_t is_false_op(std::uint64_t bits_110100) {
        std::uint64_t bits_000001 = ~bits_110100 & ALT_010101;
        return bits_000001 | (bits_000001 << 1);
    }

    std::uint64_t sharkfin_op(std::uint64_t lhs, std::uint64_t rhs) {
        std::uint64_t isolate_TT = is_true_op(lhs & rhs);
        std::uint64_t isolate_TT_U = unknown_only_true_op(isolate_TT);
        std::uint64_t isolate_UOR = isolate_uknown_op(lhs | rhs);


        std::uint64_t alt_true_false_lhs = (lhs ^ ALT_010101);
        std::uint64_t alt_true_false_rhs = (rhs ^ ALT_010101);
        std::uint64_t is_unk_lhs = is_unknown_op(lhs);
        std::uint64_t is_unk_rhs = is_unknown_op(rhs);
        std::uint64_t is_unk = is_unk_lhs | is_unk_rhs;

        std::uint64_t tf_11 = (alt_true_false_lhs ^ alt_true_false_rhs) & ~(is_unk);
        std::uint64_t uu_11 = is_unk_lhs & is_unk_rhs;
        std::uint64_t all_11 = tf_11 | uu_11;
        return isolate_TT_U | isolate_UOR | all_11;
    }

    std::uint64_t find_carry_op(std::uint64_t lhs, std::uint64_t rhs) {
        std::uint64_t notf_and = not_false_op(lhs & rhs);
        std::uint64_t true_or = is_true_op(lhs | rhs);
        return (notf_and & true_or) & ALT_010101;
    }

    std::uint64_t add_op(std::uint64_t x, std::uint64_t y){
        while(y != 0){
            std::uint64_t carry = find_carry_op(x, y);

            x = sharkfin_op(x,y);

            y = shl_op(carry, 1);
        }
        return x;
    }

    std::uint64_t sub_op(std::uint64_t x, std::uint64_t y){
        std::uint64_t y_comp = add_op(not_op(y), 1);
        return add_op(x, y_comp);
    }

    std::uint64_t comp_op(std::uint64_t lhs, std::uint64_t rhs){
        std::uint64_t ret = 1;
        if(lhs < rhs){
            ret = 0;
        }
        else if(lhs > rhs){
            ret = 3;
        }
        return ret;
    }
    std::uint64_t rotl_op(std::uint64_t bits, std::uint64_t rotate_amount, std::uint64_t mask, std::uint64_t max_bits){
        std::uint64_t lower_bits = shr_op(bits, max_bits - rotate_amount, mask);
        std::uint64_t upper_bits = shl_op(bits, rotate_amount);
        return lower_bits | upper_bits;
    }

    std::uint64_t rotr_op(std::uint64_t bits, std::uint64_t rotate_amount, std::uint64_t mask, std::uint64_t max_bits){
        std::uint64_t lower_bits = shr_op(bits, rotate_amount, mask);
        std::uint64_t upper_bits = shl_op(bits,  max_bits - rotate_amount);
        return lower_bits | upper_bits;
    }
}