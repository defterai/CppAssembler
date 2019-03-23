#pragma once


#include "x86_base.h"

namespace CppAsm::X86
{
	static constexpr struct TopRegFPU {} ST;

	enum RegFPU : uint8_t {
		ST0 = 0b000,
		ST1 = 0b001,
		ST2 = 0b010,
		ST3 = 0b011,
		ST4 = 0b100,
		ST5 = 0b101,
		ST6 = 0b110,
		ST7 = 0b111
	};

	class i387 {
	protected:
		template<class BLOCK>
		static void write_Ext_Prefix(BLOCK& block) {
			block.pushRaw<uint8_t>(0xD9);
		}

		template<class BLOCK>
		static void write_Unmask_Exception_Prefix(BLOCK& block) {
			block.pushRaw<uint8_t>(0x9B);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void template_1operand_float(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			static_assert((SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "i387: Invalid size modifier");
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, opcode.getOpcode() | ((SIZE == DWORD_PTR) ? 0x00 : 0x04));
			mem.write(block, opcode.getMode());
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void template_1operand_float80(BLOCK& block, const detail::OpcodeLarge& opcode, const detail::OpcodeLarge& opcode80, const Mem32<MODE>& mem) {
			static_assert((SIZE == DWORD_PTR) || (SIZE == QWORD_PTR) || (SIZE == TBYTE_PTR), "i387: Invalid size modifier");
			mem.writeSegmPrefix(block);
			/*constexpr*/ if (SIZE == TBYTE_PTR) {
				common::write_Opcode(block, opcode80.getOpcode());
				mem.write(block, opcode80.getMode());
			} else {
				common::write_Opcode(block, opcode.getOpcode() | ((SIZE == DWORD_PTR) ? 0x00 : 0x04));
				mem.write(block, opcode.getMode());
			}
		}

		template<class BLOCK>
		static void template_1operand_float(BLOCK& block, const detail::OpcodeLarge& opcode, RegFPU reg) {
			common::write_Opcode(block, opcode.getOpcode());
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
		}

		template<AddressMode MODE, class BLOCK>
		static void template_1operand_mem_uncheck(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, opcode.getOpcode());
			mem.write(block, opcode.getMode());
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void template_1operand_int(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR), "i387: Invalid size modifier");
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, opcode.getOpcode() | ((SIZE == DWORD_PTR) ? 0x00 : 0x04));
			mem.write(block, opcode.getMode());
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void template_1operand_int64(BLOCK& block, const detail::OpcodeLarge& opcode, const detail::OpcodeLarge& opcode64, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "i387: Invalid size modifier");
			mem.writeSegmPrefix(block);
			/*constexpr*/ if (SIZE == QWORD_PTR) {
				common::write_Opcode(block, opcode64.getOpcode());
				mem.write(block, opcode64.getMode());
			} else {
				common::write_Opcode(block, opcode.getOpcode() | ((SIZE == DWORD_PTR) ? 0x00 : 0x04));
				mem.write(block, opcode.getMode());
			}
		}

		template<class BLOCK>
		static void template_Extended(BLOCK& block, common::Opcode opcode) {
			write_Ext_Prefix(block);
			common::write_Opcode(block, opcode);
		}

		template<class BLOCK>
		static void template_2Byte_Opcode(BLOCK& block, common::Opcode opcodeFirst, common::Opcode opcodeSecond) {
			common::write_Opcode(block, opcodeFirst);
			common::write_Opcode(block, opcodeSecond);
		}
	public:
#pragma region FPU operations
		template<class BLOCK>
		static void F2xm1(BLOCK& block) {
			template_Extended(block, 0xF0);
		}

		template<class BLOCK>
		static void Fabs(BLOCK& block) {
			template_Extended(block, 0xE1);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fadd(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "Fadd: Invalid size modifier");
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b000), mem);
		}

		template<class BLOCK>
		static void Fadd(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b000), reg);
		}

		template<class BLOCK>
		static void Fadd(BLOCK& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDC, 0b000), reg);
		}

		template<class BLOCK>
		static void Faddp(BLOCK& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDE, 0b000), reg);
		}

		template<class BLOCK>
		static void Faddp(BLOCK& block) {
			Faddp(block, ST1, ST);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fiadd(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR), "Fiadd: Invalid size modifier");
			template_1operand_int<SIZE>(block, detail::OpcodeLarge(0xDA, 0b000), mem);
		}

		template<MemSize SIZE = TBYTE_PTR, AddressMode MODE, class BLOCK>
		static void Fbld(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == TBYTE_PTR, "Fbld: Invalid size modifier");
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, 0xDF);
			mem.write(block, 0b100);
		}

		template<MemSize SIZE = TBYTE_PTR, AddressMode MODE, class BLOCK>
		static void Fbstp(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == TBYTE_PTR, "Fbstp: Invalid size modifier");
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, 0xDF);
			mem.write(block, 0b110);
		}

		template<class BLOCK>
		static void Fchs(BLOCK& block) {
			template_Extended(block, 0xE0);
		}

		template<class BLOCK>
		static void Fnclex(BLOCK& block) {
			template_2Byte_Opcode(block, 0xDB, 0xE2);
		}

		template<class BLOCK>
		static void Fclex(BLOCK& block) {
			write_Unmask_Exception_Prefix(block);
			Fnclex(block);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fcom(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "Fcom: Invalid size modifier");
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b010), mem);
		}

		template<class BLOCK>
		static void Fcom(BLOCK& block, RegFPU reg = ST1) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b010), reg);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fcomp(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "Fcomp: Invalid size modifier");
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b011), mem);
		}

		template<class BLOCK>
		static void Fcomp(BLOCK& block, RegFPU reg = ST1) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b011), reg);
		}

		template<class BLOCK>
		static void Fcompp(BLOCK& block) {
			template_2Byte_Opcode(block, 0xDE, 0xD9);
		}

		template<class BLOCK>
		static void Fcos(BLOCK& block) {
			template_Extended(block, 0xFF);
		}
		
		template<class BLOCK>
		static void Fdecstp(BLOCK& block) {
			template_Extended(block, 0xF6);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fdiv(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "Fdiv: Invalid size modifier");
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b110), mem);
		}

		template<class BLOCK>
		static void Fdiv(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b110), reg);
		}

		template<class BLOCK>
		static void Fdiv(BLOCK& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDC, 0b111), reg);
		}

		template<class BLOCK>
		static void Fdivp(BLOCK& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDE, 0b111), reg);
		}

		template<class BLOCK>
		static void Fdivp(BLOCK& block) {
			Fdivp(block, ST1, ST);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fidiv(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR), "Fidiv: Invalid size modifier");
			template_1operand_int<SIZE>(block, detail::OpcodeLarge(0xDA, 0b110), mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fdivr(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "Fdivr: Invalid size modifier");
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b111), mem);
		}

		template<class BLOCK>
		static void Fdivr(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b111), reg);
		}

		template<class BLOCK>
		static void Fdivr(BLOCK& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDC, 0b110), reg);
		}

		template<class BLOCK>
		static void Fdivrp(BLOCK& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDE, 0b110), reg);
		}

		template<class BLOCK>
		static void Fdivrp(BLOCK& block) {
			Fdivrp(block, ST1, ST);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fidivr(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR), "Fidivr: Invalid size modifier");
			template_1operand_int<SIZE>(block, detail::OpcodeLarge(0xDA, 0b111), mem);
		}

		template<class BLOCK>
		static void Ffree(BLOCK& block, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDD, 0b000), reg);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Ficom(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR), "Ficom: Invalid size modifier");
			template_1operand_int<SIZE>(block, detail::OpcodeLarge(0xDA, 0b010), mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Ficomp(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR), "Ficomp: Invalid size modifier");
			template_1operand_int<SIZE>(block, detail::OpcodeLarge(0xDA, 0b011), mem);
		}


		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fild(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "Fild: Invalid size modifier");
			template_1operand_int64<SIZE>(block, detail::OpcodeLarge(0xDB, 0b000),
				detail::OpcodeLarge(0xDF, 0b101), mem);
		}

		template<class BLOCK>
		static void Fincstp(BLOCK& block) {
			template_Extended(block, 0xF7);
		}

		template<class BLOCK>
		static void Fninit(BLOCK& block) {
			template_2Byte_Opcode(block, 0xDB, 0xE3);
		}

		template<class BLOCK>
		static void Finit(BLOCK& block) {
			write_Unmask_Exception_Prefix(block);
			Fninit(block);
		}


		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fist(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR), "Fist: Invalid size modifier");
			template_1operand_int<SIZE>(block, detail::OpcodeLarge(0xDB, 0b010), mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fistp(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "Fistp: Invalid size modifier");
			template_1operand_int64<SIZE>(block, detail::OpcodeLarge(0xDB, 0b011),
				detail::OpcodeLarge(0xDF, 0b111), mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fisttp(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "Fisttp: Invalid size modifier");
			template_1operand_int64<SIZE>(block, detail::OpcodeLarge(0xDB, 0b001),
				detail::OpcodeLarge(0xDD, 0b001), mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fld(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == DWORD_PTR) || (SIZE == QWORD_PTR) || (SIZE == TBYTE_PTR), "Fld: Invalid size modifier");
			template_1operand_float80<SIZE>(block, detail::OpcodeLarge(0xD9, 0b000),
				detail::OpcodeLarge(0xDB, 0b101), mem);
		}

		template<class BLOCK>
		static void Fld(BLOCK& block, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD9, 0b000), reg);
		}

		template<class BLOCK>
		static void Fld1(BLOCK& block) {
			template_Extended(block, 0xE8);
		}

		template<class BLOCK>
		static void FldL2T(BLOCK& block) {
			template_Extended(block, 0xE9);
		}

		template<class BLOCK>
		static void FldL2E(BLOCK& block) {
			template_Extended(block, 0xEA);
		}

		template<class BLOCK>
		static void FldPI(BLOCK& block) {
			template_Extended(block, 0xEB);
		}

		template<class BLOCK>
		static void FldLG2(BLOCK& block) {
			template_Extended(block, 0xEC);
		}

		template<class BLOCK>
		static void FldLN2(BLOCK& block) {
			template_Extended(block, 0xED);
		}

		template<class BLOCK>
		static void FldZ(BLOCK& block) {
			template_Extended(block, 0xEE);
		}

		template<MemSize SIZE = WORD_PTR, AddressMode MODE, class BLOCK>
		static void Fldcw(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == WORD_PTR, "Invalid operand size");
			template_1operand_mem_uncheck(block, detail::OpcodeLarge(0xD9, 0b101), mem);
		}

		template<AddressMode MODE, class BLOCK>
		static void Fldenv(BLOCK& block, const Mem32<MODE>& mem) {
			template_1operand_mem_uncheck(block, detail::OpcodeLarge(0xD9, 0b100), mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fmul(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "Fmul: Invalid size modifier");
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b001), mem);
		}

		template<class BLOCK>
		static void Fmul(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b001), reg);
		}

		template<class BLOCK>
		static void Fmul(BLOCK& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDC, 0b001), reg);
		}

		template<class BLOCK>
		static void Fmulp(BLOCK& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDE, 0b001), reg);
		}

		template<class BLOCK>
		static void Fmulp(BLOCK& block) {
			Fmulp(block, ST1, ST);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fimul(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR), "Fimul: Invalid size modifier");
			template_1operand_int<SIZE>(block, detail::OpcodeLarge(0xDA, 0b001), mem);
		}

		template<class BLOCK>
		static void Fnop(BLOCK& block) {
			template_Extended(block, 0xD0);
		}

		template<class BLOCK>
		static void Fpatan(BLOCK& block) {
			template_Extended(block, 0xF3);
		}

		template<class BLOCK>
		static void Fprem(BLOCK& block) {
			template_Extended(block, 0xF8);
		}

		template<class BLOCK>
		static void Fprem1(BLOCK& block) {
			template_Extended(block, 0xF5);
		}

		template<class BLOCK>
		static void Fptan(BLOCK& block) {
			template_Extended(block, 0xF2);
		}

		template<class BLOCK>
		static void Frndint(BLOCK& block) {
			template_Extended(block, 0xFC);
		}

		template<AddressMode MODE, class BLOCK>
		static void Frstor(BLOCK& block, const Mem32<MODE>& mem) {
			template_1operand_mem_uncheck(block, detail::OpcodeLarge(0xDD, 0b100), mem);
		}

		template<AddressMode MODE, class BLOCK>
		static void Fsave(BLOCK& block, const Mem32<MODE>& mem) {
			write_Unmask_Exception_Prefix(block);
			Fnsave(block, mem);
		}

		template<AddressMode MODE, class BLOCK>
		static void Fnsave(BLOCK& block, const Mem32<MODE>& mem) {
			template_1operand_mem_uncheck(block, detail::OpcodeLarge(0xDD, 0b110), mem);
		}

		template<class BLOCK>
		static void Fscale(BLOCK& block) {
			template_Extended(block, 0xFD);
		}

		template<class BLOCK>
		static void Fsin(BLOCK& block) {
			template_Extended(block, 0xFE);
		}

		template<class BLOCK>
		static void Fsincos(BLOCK& block) {
			template_Extended(block, 0xFB);
		}

		template<class BLOCK>
		static void Fsqrt(BLOCK& block) {
			template_Extended(block, 0xFA);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fst(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "Fst: Invalid size modifier");
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD9, 0b010), mem);
		}

		template<class BLOCK>
		static void Fst(BLOCK& block, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDD, 0b010), reg);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fstp(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == DWORD_PTR) || (SIZE == QWORD_PTR) || (SIZE == TBYTE_PTR), "Fstp: Invalid size modifier");
			template_1operand_float80<SIZE>(block, detail::OpcodeLarge(0xD9, 0b011),
				detail::OpcodeLarge(0xDB, 0b111), mem);
		}

		template<class BLOCK>
		static void Fstp(BLOCK& block, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDD, 0b011), reg);
		}

		template<MemSize SIZE = WORD_PTR, AddressMode MODE, class BLOCK>
		static void Fstcw(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == WORD_PTR, "Fstcw: Invalid size modifier");
			write_Unmask_Exception_Prefix(block);
			Fnstcw<SIZE>(block, mem);
		}

		template<MemSize SIZE = WORD_PTR, AddressMode MODE, class BLOCK>
		static void Fnstcw(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == WORD_PTR, "Fnstcw: Invalid size modifier");
			template_1operand_mem_uncheck(block, detail::OpcodeLarge(0xD9, 0b111), mem);
		}

		template<AddressMode MODE, class BLOCK>
		static void Fstenv(BLOCK& block, const Mem32<MODE>& mem) {
			write_Unmask_Exception_Prefix(block);
			Fnstenv(block, mem);
		}

		template<AddressMode MODE, class BLOCK>
		static void Fnstenv(BLOCK& block, const Mem32<MODE>& mem) {
			template_1operand_mem_uncheck(block, detail::OpcodeLarge(0xD9, 0b110), mem);
		}

		template<MemSize SIZE = WORD_PTR, AddressMode MODE, class BLOCK>
		static void Fstsw(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == WORD_PTR, "Fstsw: Invalid size modifier");
			write_Unmask_Exception_Prefix(block);
			Fnstsw<SIZE>(block, mem);
		}

		template<MemSize SIZE = WORD_PTR, AddressMode MODE, class BLOCK>
		static void Fnstsw(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == WORD_PTR, "Fnstsw: Invalid size modifier");
			template_1operand_mem_uncheck(block, detail::OpcodeLarge(0xDD, 0b111), mem);
		}

		template<Reg16 REG = AX, class BLOCK>
		static void Fstsw(BLOCK& block) {
			static_assert(REG == AX, "Fstsw: Invalid register operand");
			write_Unmask_Exception_Prefix(block);
			Fnstsw<REG>(block);
		}

		template<Reg16 REG = AX, class BLOCK>
		static void Fnstsw(BLOCK& block) {
			static_assert(REG == AX, "Fnstsw: Invalid register operand");
			template_2Byte_Opcode(block, 0xDF, 0xE0);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fsub(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "Fsub: Invalid size modifier");
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b100), mem);
		}

		template<class BLOCK>
		static void Fsub(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b100), reg);
		}

		template<class BLOCK>
		static void Fsub(BLOCK& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDC, 0b101), reg);
		}

		template<class BLOCK>
		static void Fsubp(BLOCK& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDE, 0b101), reg);
		}

		template<class BLOCK>
		static void Fsubp(BLOCK& block) {
			Fsubp(block, ST1, ST);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fisub(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR), "Fisub: Invalid size modifier");
			template_1operand_int<SIZE>(block, detail::OpcodeLarge(0xDA, 0b100), mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fsubr(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "Fsubr: Invalid size modifier");
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b101), mem);
		}

		template<class BLOCK>
		static void Fsubr(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b101), reg);
		}

		template<class BLOCK>
		static void Fsubr(BLOCK& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDC, 0b100), reg);
		}

		template<class BLOCK>
		static void Fsubrp(BLOCK& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDE, 0b100), reg);
		}

		template<class BLOCK>
		static void Fsubrp(BLOCK& block) {
			Fsubrp(block, ST1, ST);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static void Fisubr(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR), "Fisubr: Invalid size modifier");
			template_1operand_int<SIZE>(block, detail::OpcodeLarge(0xDA, 0b101), mem);
		}

		template<class BLOCK>
		static void Ftst(BLOCK& block) {
			template_Extended(block, 0xE4);
		}

		template<class BLOCK>
		static void Fucom(BLOCK& block, RegFPU reg = ST1) {
			template_1operand_float(block, detail::OpcodeLarge(0xDD, 0b100), reg);
		}

		template<class BLOCK>
		static void Fucomp(BLOCK& block, RegFPU reg = ST1) {
			template_1operand_float(block, detail::OpcodeLarge(0xDD, 0b101), reg);
		}

		template<class BLOCK>
		static void Fucompp(BLOCK& block) {
			template_2Byte_Opcode(block, 0xDA, 0xE9);
		}

		template<class BLOCK>
		static void Fxam(BLOCK& block) {
			template_Extended(block, 0xE5);
		}

		template<class BLOCK>
		static void Fxch(BLOCK& block, RegFPU reg = ST1) {
			template_1operand_float(block, detail::OpcodeLarge(0xD9, 0b001), reg);
		}

		template<class BLOCK>
		static void Fxtract(BLOCK& block) {
			template_Extended(block, 0xE4);
		}

		template<class BLOCK>
		static void FyL2X(BLOCK& block) {
			template_Extended(block, 0xF1);
		}

		template<class BLOCK>
		static void FyL2XP1(BLOCK& block) {
			template_Extended(block, 0xF9);
		}

		/* FPU wait */
		template<class BLOCK>
		static void Fwait(BLOCK& block) {
			Wait(block);
		}

		/* FPU wait */
		template<class BLOCK>
		static void Wait(BLOCK& block) {
			common::write_Opcode(block, 0x9B);
		}
#pragma endregion
	};
}