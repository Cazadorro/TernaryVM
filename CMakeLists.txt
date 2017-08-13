cmake_minimum_required(VERSION 3.9)
project(TernaryVM)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp virtual_machine.cpp virtual_machine.h tryte.cpp tryte.h ternary_operators.cpp ternary_operators.h rotate.h ternaryparser.h)
add_executable(TernaryVM ${SOURCE_FILES})