//
// Created by Shae Bolt on 8/11/2017.
//

#include "virtual_machine.h"
#include <iostream>


namespace tvm{
    std::tuple<Tryte, Tryte, Tryte> enumToCode(TVM_ISA instruction, std::uint16_t arg1, std::uint32_t arg2){
        Tryte tryte_instruction(static_cast<std::uint16_t>(instruction));
        Tryte tryte_arg1(arg1);
        Tryte tryte_arg2(arg2);
        return std::make_tuple(tryte_instruction, tryte_arg1, tryte_arg2);
    };
}


tvm::VirtualMachine::VirtualMachine(const std::uint64_t code_memory_size,
                                    const std::uint64_t data_memory_size,
                                    const std::uint64_t number_of_registers) {

    m_code_memory = std::vector<std::tuple<Tryte, Tryte, Tryte>>(code_memory_size);
    m_data_memory = std::vector<Tryte>(data_memory_size);
    m_registers = std::vector<Tryte>(number_of_registers);
}



void tvm::VirtualMachine::fetch() {
    // TODO should each line always be 3 trytes so that the program counter can properly go up?
    m_current_program_line = m_code_memory[m_program_counter];
    m_program_counter += 1;
}

void tvm::VirtualMachine::decode() {
    m_current_instruction = std::get<0>(m_current_program_line);
    m_current_first_argument = std::get<1>(m_current_program_line);
    m_current_second_argument = std::get<2>(m_current_program_line);
    std::cout << "Current instruction: " << std::endl;
    printTryte(m_current_instruction);
    std::cout << "First argument: " << std::endl;
    printTryte(m_current_first_argument);
    std::cout << "Second argument: " << std::endl;
    printTryte(m_current_second_argument);
    std::cout << std::endl;
}

void tvm::VirtualMachine::execute() {
    switch (TVM_ISA(m_current_instruction.m_data.uint16_val)){
        case TVM_ISA::NOP   :
            no_operation();
            break;
        case TVM_ISA::EXIT  :
            exit_operation();
            break;
        case TVM_ISA::MOVRI :
            move_to_register_from_immediate();
            break;
        case TVM_ISA::MOVMI :
            move_to_memory_from_immediate();
            break;
        case TVM_ISA::MOVMM :
            move_to_memory_from_memory();
            break;
        case TVM_ISA::MOVRR :
            move_to_register_from_register();
            break;
        case TVM_ISA::MOVRM :
            move_to_register_from_memory();
            break;
        case TVM_ISA::MOVMR :
            move_to_memory_from_register();
            break;
        case TVM_ISA::JMPI  :
            jump_with_immediate();
            break;
        case TVM_ISA::JMPR  :
            jump_with_register();
            break;
        case TVM_ISA::CMPI  :
            compare_register_with_immediate();
            break;
        case TVM_ISA::CMPR  :
            compare_register_with_register();
            break;
        case TVM_ISA::ANDI  :
            and_register_with_immediate();
            break;
        case TVM_ISA::ANDR  :
            and_register_with_register();
            break;
        case TVM_ISA::ORI   :
            or_register_with_immediate();
            break;
        case TVM_ISA::ORR   :
            or_register_with_register();
            break;
        case TVM_ISA::XORI  :
            xor_register_with_immediate();
            break;
        case TVM_ISA::XORR  :
            xor_register_with_register();
            break;
        case TVM_ISA::FINI  :
            sharkfin_register_with_immediate();
            break;
        case TVM_ISA::FINR  :
            sharkfin_register_with_register();
            break;
        case TVM_ISA::CRYI  :
            findcarries_register_with_immediate();
            break;
        case TVM_ISA::CRYR  :
            findcarries_register_with_register();
            break;
        case TVM_ISA::NOT   :
            invert();
            break;
        case TVM_ISA::NOTF  :
            is_not_false();
            break;
        case TVM_ISA::NOTU  :
            is_not_unknown();
            break;
        case TVM_ISA::NOTT  :
            is_not_true();
            break;
        case TVM_ISA::TRUF  :
            is_false();
            break;
        case TVM_ISA::TRUU  :
            is_unknown();
            break;
        case TVM_ISA::TRUT  :
            is_true();
            break;
        case TVM_ISA::UNKF  :
            unknown_false();
            break;
        case TVM_ISA::UNKU  :
            unknown_unknown();
            break;
        case TVM_ISA::UNKT  :
            unknown_true();
            break;
        case TVM_ISA::SHRI  :
            shift_right_register_with_immediate();
            break;
        case TVM_ISA::SHRR  :
            shift_right_register_with_register();
            break;
        case TVM_ISA::SHLI  :
            shift_left_register_with_immediate();
            break;
        case TVM_ISA::SHLR  :
            shift_left_register_with_register();
            break;
        case TVM_ISA::ROTRI :
            rotate_right_register_with_immediate();
            break;
        case TVM_ISA::ROTRR :
            rotate_right_register_with_register();
            break;
        case TVM_ISA::ROTLI :
            rotate_left_register_with_immediate();
            break;
        case TVM_ISA::ROTLR :
            rotate_left_register_with_register();
            break;
        case TVM_ISA::ADDI  :
            add_register_with_immediate();
            break;
        case TVM_ISA::ADDR  :
            add_register_with_register();
            break;
        case TVM_ISA::SUBI  :
            sub_register_with_immediate();
            break;
        case TVM_ISA::SUBR  :
            sub_register_with_register();
            break;
        case TVM_ISA::AJMPI  :
            add_jump_with_immediate();
            break;
        case TVM_ISA::AJMPR  :
            add_jump_with_register();
            break;
        case TVM_ISA::SJMPI  :
            sub_jump_with_immediate();
            break;
        case TVM_ISA::SJMPR  :
            sub_jump_with_register();
            break;
        default:
            std::cout << "ERROR unknown operation!" << std::endl;
            exit_operation();
            break;
    }
}

void tvm::VirtualMachine::loadProgram(const  std::vector<std::tuple<Tryte, Tryte, Tryte>> program) {
    assert(program.size() <= m_code_memory.size());
    std::copy(program.begin(), program.end(), m_code_memory.begin());
    std::cout << "Copied program!" << std::endl;
    std::cout << std::endl;
}

void tvm::VirtualMachine::run() {
    m_running = true;
    m_program_counter = 0;
    while (m_running){
        fetch();
        decode();
        execute();
        std::cout << "Compare Flag: " << std::endl;
        printTryte(m_registers[VM_REG_NAME::FLAGS]);
        std::cout << "Arithmetic ouput: " << std::endl;
        printTryte(m_registers[VM_REG_NAME::ARITH]);
        std::cout << std::endl;
    }
    std::cout << "Program Finished" << std::endl;
}

void tvm::VirtualMachine::no_operation() {

}

void tvm::VirtualMachine::exit_operation() {
    m_running = false;
}

void tvm::VirtualMachine::move_to_register_from_immediate() {
    std::uint64_t destination_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    assert(destination_location < m_registers.size());
    m_registers[destination_location] = m_current_second_argument;
}

void tvm::VirtualMachine::move_to_memory_from_immediate() {
    std::uint64_t destination_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    assert(destination_location < m_data_memory.size());
    m_data_memory[destination_location] = m_current_second_argument;
}

void tvm::VirtualMachine::move_to_memory_from_memory() {
    std::uint64_t destination_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    assert(destination_location < m_data_memory.size());
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    assert(source_location < m_data_memory.size());
    m_data_memory[destination_location] = m_data_memory[source_location];
}

void tvm::VirtualMachine::move_to_register_from_register() {
    std::uint64_t destination_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    assert(destination_location < m_registers.size());
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    assert(source_location < m_registers.size());
    m_registers[destination_location] = m_registers[source_location];

}

void tvm::VirtualMachine::move_to_register_from_memory() {
    std::uint64_t destination_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    assert(destination_location < m_registers.size());
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    assert(source_location < m_data_memory.size());
    m_registers[destination_location] = m_data_memory[source_location];

}

void tvm::VirtualMachine::move_to_memory_from_register() {
    std::uint64_t destination_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    assert(destination_location < m_data_memory.size());
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    assert(source_location < m_registers.size());
    m_data_memory[destination_location] = m_registers[source_location];

}

void tvm::VirtualMachine::jump_with_immediate() {
    std::uint64_t jump_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    m_program_counter = jump_location;
}

void tvm::VirtualMachine::jump_with_register() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t jump_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_registers[register_location]));
    m_program_counter = jump_location;
}

void tvm::VirtualMachine::compare_register_with_immediate() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_current_second_argument);
    m_registers[VM_REG_NAME::FLAGS] = Tryte{comp_op(first, second)};
}

void tvm::VirtualMachine::compare_register_with_register() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_registers[source_location]);
    m_registers[VM_REG_NAME::FLAGS] = Tryte{comp_op(first, second)};

}

void tvm::VirtualMachine::and_register_with_immediate() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_current_second_argument);
    m_registers[register_location] = Tryte{first & second};
}

void tvm::VirtualMachine::and_register_with_register() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_registers[source_location]);
    m_registers[register_location] = Tryte{first & second};

}

void tvm::VirtualMachine::or_register_with_immediate() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_current_second_argument);
    m_registers[register_location] = Tryte{first | second};

}

void tvm::VirtualMachine::or_register_with_register() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_registers[source_location]);
    m_registers[register_location] = Tryte{first | second};

}

void tvm::VirtualMachine::xor_register_with_immediate() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_current_second_argument);
    m_registers[register_location] = Tryte{xor_op(first, second)};

}

void tvm::VirtualMachine::xor_register_with_register() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_registers[source_location]);
    m_registers[register_location] = Tryte{xor_op(first, second)};

}

void tvm::VirtualMachine::sharkfin_register_with_immediate() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_current_second_argument);
    m_registers[register_location] = Tryte{sharkfin_op(first, second)};

}

void tvm::VirtualMachine::sharkfin_register_with_register() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_registers[source_location]);
    m_registers[register_location] = Tryte{sharkfin_op(first, second)};

}

void tvm::VirtualMachine::findcarries_register_with_immediate() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_current_second_argument);
    m_registers[register_location] = Tryte{find_carry_op(first, second)};

}

void tvm::VirtualMachine::findcarries_register_with_register() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_registers[source_location]);
    m_registers[register_location] = Tryte{find_carry_op(first, second)};
}

void tvm::VirtualMachine::invert() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    m_registers[register_location] = Tryte{not_op(first)};
}

void tvm::VirtualMachine::is_not_false() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    m_registers[register_location] = Tryte{not_false_op(first)};
}

void tvm::VirtualMachine::is_not_unknown() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    m_registers[register_location] = Tryte{not_unknown_op(first)};
}

void tvm::VirtualMachine::is_not_true() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    m_registers[register_location] = Tryte{not_true_op(first)};
}

void tvm::VirtualMachine::is_false() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    m_registers[register_location] = Tryte{is_false_op(first)};

}

void tvm::VirtualMachine::is_unknown() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    m_registers[register_location] = Tryte{is_unknown_op(first)};

}

void tvm::VirtualMachine::is_true() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    m_registers[register_location] = Tryte{is_true_op(first)};
}

void tvm::VirtualMachine::unknown_false() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    m_registers[register_location] = Tryte{unknown_only_false_op(first)};
}

void tvm::VirtualMachine::unknown_unknown() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    m_registers[register_location] = Tryte{isolate_uknown_op(first)};

}

void tvm::VirtualMachine::unknown_true() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    m_registers[register_location] = Tryte{unknown_only_true_op(first)};
}

void tvm::VirtualMachine::shift_right_register_with_immediate() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_current_second_argument);
    m_registers[register_location] = Tryte{shr_op(first, second, 0x0FFF)};
}

void tvm::VirtualMachine::shift_right_register_with_register() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_registers[source_location]);
    m_registers[register_location] = Tryte{shr_op(first, second, 0x0FFF)};
}

void tvm::VirtualMachine::shift_left_register_with_immediate() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_current_second_argument);
    m_registers[register_location] = Tryte{shl_op(first, second)};

}

void tvm::VirtualMachine::shift_left_register_with_register() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_registers[source_location]);
    m_registers[register_location] = Tryte{shl_op(first, second)};

}

void tvm::VirtualMachine::rotate_right_register_with_immediate() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_current_second_argument);
    m_registers[register_location] = Tryte{rotr_op(first, second, 0x0FFF, 12)};
}

void tvm::VirtualMachine::rotate_right_register_with_register() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_registers[source_location]);
    m_registers[register_location] = Tryte{rotr_op(first, second, 0x0FFF, 12)};
}

void tvm::VirtualMachine::rotate_left_register_with_immediate() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_current_second_argument);
    m_registers[register_location] = Tryte{rotl_op(first, second, 0x0FFF, 12)};
}

void tvm::VirtualMachine::rotate_left_register_with_register() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_registers[source_location]);
    m_registers[register_location] = Tryte{rotl_op(first, second, 0x0FFF, 12)};
}

void tvm::VirtualMachine::add_register_with_immediate() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_current_second_argument);
    m_registers[VM_REG_NAME::ARITH] = Tryte{add_op(first, second)};
}

void tvm::VirtualMachine::add_register_with_register() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_registers[source_location]);
    m_registers[VM_REG_NAME::ARITH] = Tryte{add_op(first, second)};
}

void tvm::VirtualMachine::sub_register_with_immediate() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_current_second_argument);
    m_registers[VM_REG_NAME::ARITH] = Tryte{sub_op(first, second)};
}

void tvm::VirtualMachine::sub_register_with_register() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t source_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_second_argument));
    std::uint64_t first = static_cast<std::uint16_t>(m_registers[register_location]);
    std::uint64_t second = static_cast<std::uint16_t>(m_registers[source_location]);
    m_registers[VM_REG_NAME::ARITH] = Tryte{sub_op(first, second)};
}

void tvm::VirtualMachine::add_jump_with_immediate() {
    std::uint64_t offset = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    m_program_counter += offset;
}

void tvm::VirtualMachine::add_jump_with_register() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t offset = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_registers[register_location]));
    m_program_counter += offset;
}

void tvm::VirtualMachine::sub_jump_with_immediate() {
    std::uint64_t offset = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    m_program_counter -= offset;
}

void tvm::VirtualMachine::sub_jump_with_register() {
    std::uint64_t register_location = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_current_first_argument));
    std::uint64_t offset = ternary_coded_bin_to_bin(static_cast<std::uint16_t>(m_registers[register_location]));
    m_program_counter -= offset;
}
