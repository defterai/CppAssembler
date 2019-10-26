cmake_minimum_required (VERSION 3.0)
project (CppAssembler VERSION 0.2.0 DESCRIPTION "CppAssembler library")

add_library(CppAssembler STATIC ${CMAKE_CURRENT_LIST_DIR}/source/os.cpp)

target_compile_options(CppAssembler PRIVATE -std=c++17 -O3 -Wall -Wno-unknown-pragmas)
target_include_directories(CppAssembler PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(CppAssembler PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

if(DEFINED ENV{BUILD_32BIT})
    message(STATUS "Build 32 bit CppAssembler")
    target_compile_options(CppAssembler PRIVATE -m32)
    SET_TARGET_PROPERTIES(CppAssembler PROPERTIES LINK_FLAGS "-Wl,--build-id=none,-mi386linux")
endif()
