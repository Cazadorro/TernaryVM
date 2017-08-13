#include <iostream>

#include <cstdint>
#include <bitset>
#include "virtual_machine.h"
#include <vector>
#include <tuple>

int main() {
    std::cout << "hello world" << std::endl;
    tvm::VirtualMachine vm(48, 48, 12);
    std::vector<std::tuple<tvm::Tryte, tvm::Tryte, tvm::Tryte>> program;
    {
        using namespace tvm;
        program.push_back(enumToCode(TVM_ISA::MOVRI, 000010_tryte, 000010_tryte));
        program.push_back(enumToCode(TVM_ISA::MOVRI, 000011_tryte, 000210_tryte));
        program.push_back(enumToCode(TVM_ISA::SUBR, 000011_tryte, 000010_tryte));
        program.push_back(enumToCode(TVM_ISA::EXIT, 000000_tryte, 000000_tryte));
    }
    vm.loadProgram(program);
    vm.run();


    return 0;
}