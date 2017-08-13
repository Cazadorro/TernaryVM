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
// TODO FIX
//    constexpr std::uint8_t operator"" _tr (const char* str){
//        const std::string std_str(str);
//        std::uint64_t ternary_int = 0;
//        for(auto const&str_char : std_str){
//            static_assert((str_char == '0' || str_char == '1' || str_char == '2', "literal must only have numbers 0,"
//                    "1,2"));
//            if(str_char == '1'){
//                ternary_int |= 1;
//                ternary_int <<= 2;
//            }
//            else if(str_char == '2'){
//                ternary_int |= 3;
//                ternary_int <<= 2;
//            }
//            else{
//                ternary_int <<= 2;
//            }
//        }
//    }


    std::uint64_t native_to_10_encoded_ternary(std::uint64_t ternary_bits);

    std::uint64_t ternary_coded_bin_to_bin(std::uint64_t bits);

    std::uint64_t isolate_uknown_op(std::uint64_t bits_110100);


    std::uint64_t not_op(std::uint64_t bits_110100);

//alternate way, but slower
//    std::uint64_t xor_op(std::uint64_t lhs, std::uint64_t rhs){
//        return (lhs & not_op(rhs)) | (not_op(lhs) & rhs);
//    }
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


//alternate calculation, easier to read, but slower
//    std::uint64_t is_unknown_op(std::uint64_t bits){
//        return ~is_true_op(bits) & not_false_op(bits);
//    }


//    constexpr std::uint8_t operator"" _trb (const char* str) {
//        const std::string std_str(str);
//        static_assert((std_str.size() == 4, "only accept 4 characters, no less no more"));
//
//        std::uint8_t rhs_2bits = 0;
//        std::uint8_t lhs_2bits = 0;
//        int i = 0;
//        for(auto const&str_char : std_str){
//            if(i % 2 == 0){
//                lhs_2bits = rhs_2bits;
//                rhs_2bits = 0;
//            }
//            static_assert((str_char == '0' || str_char == '1' || str_char == '2', "literal must only have numbers 0,"
//                    "1,2"));
//            if(str_char == '1'){
//                rhs_2bits |= 1;
//                rhs_2bits <<= 2;
//            }
//            else if(str_char == '2'){
//                rhs_2bits |= 3;
//                rhs_2bits <<= 2;
//            }
//            else{
//                rhs_2bits <<= 2;
//            }
//            i+=1;
//        }
//        std::uint8_t x = lhs_2bits;
//        std::uint8_t y = rhs_2bits;
//        while(y != 0){
//            std::uint8_t carry = x & y;
//
//            x = xor_op(x,y);
//
//            y = shl_op(carry, 1);
//        }
//        return x;
//    }
//
//    const std::array<std::uint8_t, 256> add_array {
//           0000_trb, 0001_trb, 0000_trb, 0002_trb, 0010_trb, 0011_trb, 0000_trb, 0012_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0020_trb, 0021_trb, 0000_trb, 0022_trb,
//           0100_trb, 0101_trb, 0000_trb, 0102_trb, 0110_trb, 0111_trb, 0000_trb, 0112_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0120_trb, 0121_trb, 0000_trb, 0122_trb,
//           0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb,
//           0200_trb, 0201_trb, 0000_trb, 0202_trb, 0210_trb, 0211_trb, 0000_trb, 0212_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0220_trb, 0221_trb, 0000_trb, 0222_trb,
//
//           1000_trb, 1001_trb, 0000_trb, 1002_trb, 1010_trb, 1011_trb, 0000_trb, 1012_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 1020_trb, 1021_trb, 0000_trb, 1022_trb,
//           1100_trb, 1101_trb, 0000_trb, 1102_trb, 1110_trb, 1111_trb, 0000_trb, 1112_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 1120_trb, 1121_trb, 0000_trb, 1122_trb,
//           0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb,
//           1200_trb, 1201_trb, 0000_trb, 1202_trb, 1210_trb, 1211_trb, 0000_trb, 1212_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 1220_trb, 1221_trb, 0000_trb, 1222_trb,
//
//           0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb,
//           0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb,
//           0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb,
//           0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb,
//
//           2000_trb, 2001_trb, 0000_trb, 2002_trb, 2010_trb, 2011_trb, 0000_trb, 2012_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 2020_trb, 2021_trb, 0000_trb, 2022_trb,
//           2100_trb, 2101_trb, 0000_trb, 2102_trb, 2110_trb, 2111_trb, 0000_trb, 2112_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 2120_trb, 2121_trb, 0000_trb, 2122_trb,
//           0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb,
//           2200_trb, 2201_trb, 0000_trb, 2202_trb, 2210_trb, 2211_trb, 0000_trb, 2212_trb, 0000_trb, 0000_trb, 0000_trb, 0000_trb, 2220_trb, 2221_trb, 0000_trb, 2222_trb,
//
//    };




}


#endif //TERNARYVM_TERNARY_OPERATORS_H
