# CppAssembler
x86 and x64 assembly code generation library for C++11

## Benefits

* Fastest assembly code generation library
1. Compile time instruction opcode generation with inline
2. No heap memory allocations
3. Do not use exceptions

* Some advanced features
1. Compile time check for instructions syntax
2. Support replace some instruction operands after first pass
3. Available instruction set can be configured to find not available instruction in compile time
4. Fast measure of instructions size before write them

* No external dependencies
1. Use only C++ standart libraries for core functionality

## Downsides

* Too low level
1. No optimization of instructions size (should be implemented by more high level code)
2. No relocation code support (should be implemented by more high level code)
3. No check for input variable parameters range etc (high level code must ensure it pass valid constants as arguments to prevert UB)

## Support compilers

* Visual Studio 2015 (Update 3)
