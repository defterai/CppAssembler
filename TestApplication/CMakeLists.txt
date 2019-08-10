cmake_minimum_required (VERSION 3.0)
project (TestApplication VERSION 0.2.0 DESCRIPTION "CppAssembler test application")

add_executable(TestApplication ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
target_compile_options(TestApplication PRIVATE -std=c++14 -O3 -Wall -Wno-unknown-pragmas)

target_link_libraries(TestApplication CppAssembler)
