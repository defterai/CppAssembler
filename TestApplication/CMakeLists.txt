cmake_minimum_required (VERSION 3.0)
project (TestApplication VERSION 0.2.0 DESCRIPTION "CppAssembler test application")

add_executable(TestApplication ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
target_compile_options(TestApplication PRIVATE -std=c++17 -O3 -Wall -Wno-unknown-pragmas)

target_link_libraries(TestApplication CppAssembler)

if(DEFINED ENV{BUILD_32BIT})
    message(STATUS "Build 32 bit TestApplication")
    target_compile_options(TestApplication PRIVATE -m32)
	SET_TARGET_PROPERTIES(TestApplication PROPERTIES LINK_FLAGS "-Wl,--build-id=none,-mi386linux")
endif()
