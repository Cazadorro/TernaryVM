//
// Created by Shae Bolt on 8/11/2017.
//

#include "tryte.h"
#include <iostream>
#include <bitset>

namespace tvm {
    void printTrit(const std::uint8_t trit) {
        const std::uint8_t trit_masked_value = trit & static_cast<std::uint8_t>(3);
        if (trit_masked_value == 2) {
            std::cout << "X";
        } else if(trit_masked_value == 3){
            std::cout << "2";
        } else {
            std::cout << static_cast<uint64_t>(trit);
        }
    }

    void printTryte(const Tryte &tryte) {
        std::bitset<12> bitset_tryte = tryte.m_data.uint16_val;
        std::cout << "Bitset value:" << bitset_tryte << std::endl;
        std::cout << "Tritset value: ";
        printTrit(tryte.m_data.trit_5);
        printTrit(tryte.m_data.trit_4);
        printTrit(tryte.m_data.trit_3);
        printTrit(tryte.m_data.trit_2);
        printTrit(tryte.m_data.trit_1);
        printTrit(tryte.m_data.trit_0);
        std::cout << std::endl;
    }
}