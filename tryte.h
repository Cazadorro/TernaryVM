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
