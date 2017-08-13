//
// Created by Shae Bolt on 8/11/2017.
//

#ifndef TERNARYVM_TRYTE_H
#define TERNARYVM_TRYTE_H

#include <cstdint>
#include <string>
#include "ternaryparser.h"

namespace tvm {

    template<char... Chars>
    constexpr std::uint64_t operator"" _tryte (){
        return {litparser<0,0,Chars...>::value};
    }
//    constexpr Tryte operator"" _trt (const char* str){
//        const std::string std_str(str);
//        static_assert(std_str.size() == 6);
//        std::uint64_t ternary_int = 0;
//        //[0,1,2,3,4,5]
//        //ternary maintains msb first order.
//        int i = 0;
//        for(auto const&str_char : std_str){
//            static_assert((str_char == '0' || str_char == '1' || str_char == '2', "literal must only have numbers 0,"
//                    "1,2"));
//            if(str_char == '1'){
//                ternary_int |= (1 << (2 * (5-i)));
//            }
//            else if(str_char == '2'){
//                ternary_int |= (3 << (2 *(5-i)));
//            }
//            i+=1;
//        }
//        return Tryte{ternary_int};
//    }
    union TryteUnion {
        std::uint16_t uint16_val;
        struct {
            std::uint16_t trit_0:2;
            std::uint16_t trit_1:2;
            std::uint16_t trit_2:2;
            std::uint16_t trit_3:2;
            std::uint16_t trit_4:2;
            std::uint16_t trit_5:2;
        };
    };
    class Tryte {
    public:
        TryteUnion m_data;

        explicit operator std::uint16_t() const{
            return m_data.uint16_val;
        }
        explicit operator std::uint64_t() const{
            return m_data.uint16_val;
        }
        Tryte(std::uint64_t data){
            m_data = {static_cast<std::uint16_t>(data)};
        }
        Tryte() = default;
    };

    void printTrit(const std::uint8_t trit);

    void printTryte(const Tryte& tryte);



}


#endif //TERNARYVM_TRYTE_H
