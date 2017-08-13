//
// Created by Shae Bolt on 8/11/2017.
//

#ifndef TERNARYVM_TERNARY_VM_H
#define TERNARYVM_TERNARY_VM_H

#include <cstdint>
#include <array>
#include <vector>
#include <tuple>
#include "ternary_operators.h"
#include "tryte.h"


// TODO allow static array allocation

namespace tvm {


    enum class TVM_ISA : std::uint16_t {
        NOP   = 000000_tryte,
        EXIT  = 000001_tryte,
        MOVRI = 000002_tryte,
        MOVMI = 000010_tryte,
        MOVMM = 000011_tryte,
        MOVRR = 000012_tryte,
        MOVRM = 000020_tryte,
        MOVMR = 000021_tryte,
        JMPI  = 000022_tryte,
        JMPR  = 000100_tryte,
        CMPI  = 000101_tryte,
        CMPR  = 000102_tryte,
        ANDI  = 000110_tryte,
        ANDR  = 000111_tryte,
        ORI   = 000112_tryte,
        ORR   = 000120_tryte,
        XORI  = 000121_tryte,
        XORR  = 000122_tryte,
        FINI  = 000200_tryte,
        FINR  = 000201_tryte,
        CRYI  = 000202_tryte,
        CRYR  = 000210_tryte,
        NOT   = 000211_tryte,
        NOTF  = 000212_tryte,
        NOTU  = 000220_tryte,
        NOTT  = 000221_tryte,
        TRUF  = 000222_tryte,
        TRUU  = 001000_tryte,
        TRUT  = 001001_tryte,
        UNKF  = 001002_tryte,
        UNKU  = 001010_tryte,
        UNKT  = 001011_tryte,
        SHRI  = 001012_tryte,
        SHRR  = 001020_tryte,
        SHLI  = 001021_tryte,
        SHLR  = 001022_tryte,
        ROTRI = 001100_tryte,
        ROTRR = 001101_tryte,
        ROTLI = 001102_tryte,
        ROTLR = 001110_tryte,
        ADDI  = 001111_tryte,
        ADDR  = 001112_tryte,
        SUBI  = 001120_tryte,
        SUBR  = 001121_tryte,
        AJMPI = 001122_tryte,
        AJMPR = 001200_tryte,
        SJMPI = 001201_tryte,
        SJMPR = 001202_tryte,
    };

    std::tuple<Tryte, Tryte, Tryte> enumToCode(TVM_ISA instruction, std::uint16_t arg1, std::uint32_t arg2);

    enum VM_REG_NAME : std::uint64_t {
        FLAGS = 0,
        ARITH = 1,
        LOGIC = 2,
    };


    class VirtualMachine {
    public:
        std::uint64_t m_program_counter = 0;
        bool m_running = false;
        std::vector<std::tuple<Tryte, Tryte, Tryte>> m_code_memory;
        std::vector<Tryte> m_data_memory;
        std::vector<Tryte> m_registers;
        std::tuple<Tryte, Tryte, Tryte> m_current_program_line;
        Tryte m_current_instruction;
        Tryte m_current_first_argument;
        Tryte m_current_second_argument;
    public:
        VirtualMachine(std::uint64_t code_memory_size,
                       std::uint64_t data_memory_size,
                       std::uint64_t number_of_registers);

        void fetch();
        void decode();
        void execute();
        void loadProgram( std::vector<std::tuple<Tryte, Tryte, Tryte>> program);
        void run();
    private:
        void no_operation();
        void exit_operation();
        void move_to_register_from_immediate();
        void move_to_memory_from_immediate();
        void move_to_memory_from_memory();
        void move_to_register_from_register();
        void move_to_register_from_memory();
        void move_to_memory_from_register();
        void jump_with_immediate();
        void jump_with_register();
        void compare_register_with_immediate();
        void compare_register_with_register();
        void and_register_with_immediate();
        void and_register_with_register();
        void or_register_with_immediate();
        void or_register_with_register();
        void xor_register_with_immediate();
        void xor_register_with_register();
        void sharkfin_register_with_immediate();
        void sharkfin_register_with_register();
        void findcarries_register_with_immediate();
        void findcarries_register_with_register();
        void invert();
        void is_not_false();
        void is_not_unknown();
        void is_not_true();
        void is_false();
        void is_unknown();
        void is_true();
        void unknown_false();
        void unknown_unknown();
        void unknown_true();
        void shift_right_register_with_immediate();
        void shift_right_register_with_register();
        void shift_left_register_with_immediate();
        void shift_left_register_with_register();
        void rotate_right_register_with_immediate();
        void rotate_right_register_with_register();
        void rotate_left_register_with_immediate();
        void rotate_left_register_with_register();
        void add_register_with_immediate();
        void add_register_with_register();
        void sub_register_with_immediate();
        void sub_register_with_register();
        void add_jump_with_immediate();
        void add_jump_with_register();
        void sub_jump_with_immediate();
        void sub_jump_with_register();
    };


}



#endif //TERNARYVM_TERNARY_VM_H
