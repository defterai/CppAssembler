#pragma once

#include "x86_i386.h"

namespace CppAsm::X86
{
	class i386_optimized {
	public:
		class optimizedBySize : private i386 {
		public:
			/* Addition with carry flag (optimized by size)
			 - ADC reg, imm
			*/
			template<class REG, class T, class BLOCK>
			static void Adc(BLOCK& block, REG reg, const Imm<T>& imm) {
				static_assert(IsRegType<REG>::value, "Adc: First parameter is not register");
				if (reg) {
					i386::Adc(block, reg, imm);
				} else {
					i386::Adc<TypeMemSize<REG>::value>(block, imm);
				}
			}

			/* Addition (optimized by size)
			 - ADD reg, imm
			*/
			template<class REG, class T, class BLOCK>
			static void Add(BLOCK& block, REG reg, const Imm<T>& imm) {
				static_assert(IsRegType<REG>::value, "Add: First parameter is not register");
				if (reg) {
					i386::Add(block, reg, imm);
				} else {
					i386::Add<TypeMemSize<REG>::value>(block, imm);
				}
			}

		    /* Compare Two Operands (optimized by size)
			 - CMP reg, imm
			*/
			template<class REG, class T, class BLOCK>
			static void Cmp(BLOCK& block, REG reg, const Imm<T>& imm) {
				static_assert(IsRegType<REG>::value, "Cmp: First parameter is not register");
				if (reg) {
					i386::Cmp(block, reg, imm);
				} else {
					i386::Cmp<TypeMemSize<REG>::value>(block, imm);
				}
			}

			/* Integer Subtraction with Borrow (optimized by size)
			 - SBB reg, imm
			*/
			template<class REG, class T, class BLOCK>
			static void Sbb(BLOCK& block, REG reg, const Imm<T>& imm) {
				static_assert(IsRegType<REG>::value, "Sbb: First parameter is not register");
				if (reg) {
					i386::Sbb(block, reg, imm);
				} else {
					i386::Sbb<TypeMemSize<REG>::value>(block, imm);
				}
			}

			/* Subtract (optimized by size)
			 - SUB reg, imm
			*/
			template<class REG, class T, class BLOCK>
			static void Sub(BLOCK& block, REG reg, const Imm<T>& imm) {
				static_assert(IsRegType<REG>::value, "Sub: First parameter is not register");
				if (reg) {
					i386::Sub(block, reg, imm);
				} else {
					i386::Sub<TypeMemSize<REG>::value>(block, imm);
				}
			}

			/* Logical AND (optimized by size)
			 - AND reg, imm
			*/
			template<class REG, class T, class BLOCK>
			static void And(BLOCK& block, REG reg, const Imm<T>& imm) {
				static_assert(IsRegType<REG>::value, "And: First parameter is not register");
				if (reg) {
					i386::And(block, reg, imm);
				} else {
					i386::And<TypeMemSize<REG>::value>(block, imm);
				}
			}

			/* Logical OR (optimized by size)
			 - OR reg, imm
			*/
			template<class REG, class T, class BLOCK>
			static void Or(BLOCK& block, REG reg, const Imm<T>& imm) {
				static_assert(IsRegType<REG>::value, "Or: First parameter is not register");
				if (reg) {
					i386::Or(block, reg, imm);
				} else {
					i386::Or<TypeMemSize<REG>::value>(block, imm);
				}
			}

			/* Logical Compare (optimized by size)
			 - TEST reg, imm
			*/
			template<class REG, class T, class BLOCK>
			static void Test(BLOCK& block, REG reg, const Imm<T>& imm) {
				static_assert(IsRegType<REG>::value, "Test: First parameter is not register");
				if (reg) {
					i386::Test(block, reg, imm);
				} else {
					i386::Test<TypeMemSize<REG>::value>(block, imm);
				}
			}

			/* Logical Exclusive OR (optimized by size)
			 - XOR reg, imm
			*/
			template<class REG, class T, class BLOCK>
			static void Xor(BLOCK& block, REG reg, const Imm<T>& imm) {
				static_assert(IsRegType<REG>::value, "Xor: First parameter is not register");
				if (reg) {
					i386::Xor(block, reg, imm);
				} else {
					i386::Xor<TypeMemSize<REG>::value>(block, imm);
				}
			}
		};
	};
}