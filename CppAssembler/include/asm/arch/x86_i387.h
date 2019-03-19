#pragma once


#include "x86_base.h"

namespace CppAsm::X86
{
	struct TopRegFPU {} ST;

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
		static void write_Ext_Prefix(Os::CodeBlock& block) {
			block.pushRaw<uint8_t>(0xD9);
		}

		static void write_Unmask_Exception_Prefix(Os::CodeBlock& block) {
			block.pushRaw<uint8_t>(0x9B);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void template_1operand_float(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			static_assert((SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "Invalid operand size");
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, opcode.getOpcode() | ((SIZE == DWORD_PTR) ? 0x00 : 0x04));
			mem.write(block, opcode.getMode());
		}

		template<MemSize SIZE, AddressMode MODE>
		static void template_1operand_float80(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, const detail::OpcodeLarge& opcode80, const Mem32<MODE>& mem) {
			static_assert((SIZE == DWORD_PTR) || (SIZE == QWORD_PTR) || (SIZE == TBYTE_PTR), "Invalid operand size");
			mem.writeSegmPrefix(block);
			if (SIZE == TBYTE_PTR) {
				common::write_Opcode(block, opcode80.getOpcode());
				mem.write(block, opcode80.getMode());
			} else {
				common::write_Opcode(block, opcode.getOpcode() | ((SIZE == DWORD_PTR) ? 0x00 : 0x04));
				mem.write(block, opcode.getMode());
			}
		}

		static void template_1operand_float(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, RegFPU reg) {
			common::write_Opcode(block, opcode.getOpcode());
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
		}

		template<AddressMode MODE>
		static void template_1operand_mem_uncheck(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, opcode.getOpcode());
			mem.write(block, opcode.getMode());
		}

		template<MemSize SIZE, AddressMode MODE>
		static void template_1operand_int(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR), "Invalid operand size");
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, opcode.getOpcode() | ((SIZE == DWORD_PTR) ? 0x00 : 0x04));
			mem.write(block, opcode.getMode());
		}

		template<MemSize SIZE, AddressMode MODE>
		static void template_1operand_int64(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, const detail::OpcodeLarge& opcode64, const Mem32<MODE>& mem) {
			static_assert((SIZE == WORD_PTR) || (SIZE == DWORD_PTR) || (SIZE == QWORD_PTR), "Invalid operand size");
			mem.writeSegmPrefix(block);
			if (SIZE == QWORD_PTR) {
				common::write_Opcode(block, opcode64.getOpcode());
				mem.write(block, opcode64.getMode());
			} else {
				common::write_Opcode(block, opcode.getOpcode() | ((SIZE == DWORD_PTR) ? 0x00 : 0x04));
				mem.write(block, opcode.getMode());
			}
		}

		static void template_Extended(Os::CodeBlock& block, common::Opcode opcode) {
			write_Ext_Prefix(block);
			common::write_Opcode(block, opcode);
		}

		static void template_2Byte_Opcode(Os::CodeBlock& block, common::Opcode opcodeFirst, common::Opcode opcodeSecond) {
			common::write_Opcode(block, opcodeFirst);
			common::write_Opcode(block, opcodeSecond);
		}
	public:
#pragma region FPU operations
		static void F2xm1(Os::CodeBlock& block) {
			template_Extended(block, 0xF0);
		}
		static void Fabs(Os::CodeBlock& block) {
			template_Extended(block, 0xE1);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fadd(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b000), mem);
		}

		static void Fadd(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b000), reg);
		}

		static void Fadd(Os::CodeBlock& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDC, 0b000), reg);
		}

		static void Faddp(Os::CodeBlock& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDE, 0b000), reg);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fiadd(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_int<SIZE>(block, detail::OpcodeLarge(0xDA, 0b000), mem);
		}

		template<MemSize SIZE = TBYTE_PTR, AddressMode MODE>
		static void Fbld(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, 0xDF);
			mem.write(block, 0b100);
		}

		template<MemSize SIZE = TBYTE_PTR, AddressMode MODE>
		static void Fbstp(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, 0xDF);
			mem.write(block, 0b110);
		}

		static void Fchs(Os::CodeBlock& block) {
			template_Extended(block, 0xE0);
		}

		static void Fnclex(Os::CodeBlock& block) {
			template_2Byte_Opcode(block, 0xDB, 0xE2);
		}

		static void Fclex(Os::CodeBlock& block) {
			write_Unmask_Exception_Prefix(block);
			Fnclex(block);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fcom(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b010), mem);
		}

		static void Fcom(Os::CodeBlock& block, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b010), reg);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fcomp(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b011), mem);
		}

		static void Fcomp(Os::CodeBlock& block, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b011), reg);
		}

		static void Fcompp(Os::CodeBlock& block) {
			template_2Byte_Opcode(block, 0xDE, 0xD9);
		}

		static void Fcos(Os::CodeBlock& block) {
			template_Extended(block, 0xFF);
		}
		
		static void Fdecstp(Os::CodeBlock& block) {
			template_Extended(block, 0xF6);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fdiv(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b110), mem);
		}

		static void Fdiv(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b110), reg);
		}

		static void Fdiv(Os::CodeBlock& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDC, 0b111), reg);
		}

		static void Fdivp(Os::CodeBlock& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDE, 0b111), reg);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fidiv(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_int(block, detail::OpcodeLarge(0xDA, 0b110), reg);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fdivr(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b111), mem);
		}

		static void Fdivr(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b111), reg);
		}

		static void Fdivr(Os::CodeBlock& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDC, 0b110), reg);
		}

		static void Fdivrp(Os::CodeBlock& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDE, 0b110), reg);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fidivr(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_int(block, detail::OpcodeLarge(0xDA, 0b111), mem);
		}

		static void Ffree(Os::CodeBlock& block, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDD, 0b000), reg);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Ficom(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_int(block, detail::OpcodeLarge(0xDA, 0b010), mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Ficomp(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_int(block, detail::OpcodeLarge(0xDA, 0b011), mem);
		}


		template<MemSize SIZE, AddressMode MODE>
		static void Fild(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_int64(block, detail::OpcodeLarge(0xDB, 0b000),
				detail::OpcodeLarge(0xDF, 0b101), mem);
		}

		static void Fincstp(Os::CodeBlock& block) {
			template_Extended(block, 0xF7);
		}

		static void Fninit(Os::CodeBlock& block) {
			template_2Byte_Opcode(block, 0xDB, 0xE3);
		}

		static void Finit(Os::CodeBlock& block) {
			write_Unmask_Exception_Prefix(block);
			Fninit(block);
		}


		template<MemSize SIZE, AddressMode MODE>
		static void Fist(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_int(block, detail::OpcodeLarge(0xDB, 0b010), mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fistp(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_int64(block, detail::OpcodeLarge(0xDB, 0b011),
				detail::OpcodeLarge(0xDF, 0b111), mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fisttp(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_int64(block, detail::OpcodeLarge(0xDB, 0b001),
				detail::OpcodeLarge(0xDD, 0b001), mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fld(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_float80(block, detail::OpcodeLarge(0xD9, 0b000),
				detail::OpcodeLarge(0xDB, 0b101), mem)
		}

		static void Fld(Os::CodeBlock& block, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD9, 0b000), reg);
		}

		static void Fld1(Os::CodeBlock& block) {
			template_Extended(block, 0xE8);
		}

		static void FldL2T(Os::CodeBlock& block) {
			template_Extended(block, 0xE9);
		}

		static void FldL2E(Os::CodeBlock& block) {
			template_Extended(block, 0xEA);
		}

		static void FldPI(Os::CodeBlock& block) {
			template_Extended(block, 0xEB);
		}

		static void FldLG2(Os::CodeBlock& block) {
			template_Extended(block, 0xEC);
		}

		static void FldLN2(Os::CodeBlock& block) {
			template_Extended(block, 0xED);
		}

		static void FldZ(Os::CodeBlock& block) {
			template_Extended(block, 0xEE);
		}

		template<MemSize SIZE = WORD_PTR, AddressMode MODE>
		static void Fldcw(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == WORD_PTR, "Invalid operand size");
			template_1operand_mem_uncheck(block, detail::OpcodeLarge(0xD9, 0b101), mem);
		}

		template<AddressMode MODE>
		static void Fldenv(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_mem_uncheck(block, detail::OpcodeLarge(0xD9, 0b100), mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fmul(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b001), mem);
		}

		static void Fmul(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b001), reg);
		}

		static void Fmul(Os::CodeBlock& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDC, 0b001), reg);
		}

		static void Fmulp(Os::CodeBlock& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDE, 0b001), reg);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fimul(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_int(block, detail::OpcodeLarge(0xDA, 0b001), reg);
		}

		static void Fnop(Os::CodeBlock& block) {
			template_Extended(block, 0xD0);
		}

		static void Fpatan(Os::CodeBlock& block) {
			template_Extended(block, 0xF3);
		}

		static void Fprem(Os::CodeBlock& block) {
			template_Extended(block, 0xF8);
		}

		static void Fprem1(Os::CodeBlock& block) {
			template_Extended(block, 0xF5);
		}

		static void Fptan(Os::CodeBlock& block) {
			template_Extended(block, 0xF2);
		}

		static void Frndint(Os::CodeBlock& block) {
			template_Extended(block, 0xFC);
		}

		template<AddressMode MODE>
		static void Frstor(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_mem_uncheck(block, detail::OpcodeLarge(0xDD, 0b100), mem);
		}

		template<AddressMode MODE>
		static void Fsave(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			write_Unmask_Exception_Prefix(block);
			Fnsave(block, mem);
		}

		template<AddressMode MODE>
		static void Fnsave(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_mem_uncheck(block, detail::OpcodeLarge(0xDD, 0b110), mem);
		}

		static void Fscale(Os::CodeBlock& block) {
			template_Extended(block, 0xFD);
		}

		static void Fsin(Os::CodeBlock& block) {
			template_Extended(block, 0xFE);
		}

		static void Fsincos(Os::CodeBlock& block) {
			template_Extended(block, 0xFB);
		}

		static void Fsqrt(Os::CodeBlock& block) {
			template_Extended(block, 0xFA);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fst(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD9, 0b010), mem);
		}

		static void Fst(Os::CodeBlock& block, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDD, 0b010), reg);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fstp(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_float80(block, detail::OpcodeLarge(0xD9, 0b011),
				detail::OpcodeLarge(0xDB, 0b111), mem)
		}

		static void Fstp(Os::CodeBlock& block, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDD, 0b011), reg);
		}

		template<MemSize SIZE = WORD_PTR, AddressMode MODE>
		static void Fstcw(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			write_Unmask_Exception_Prefix(block);
			Fnstcw<SIZE>(block, mem);
		}

		template<MemSize SIZE = WORD_PTR, AddressMode MODE>
		static void Fnstcw(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == WORD_PTR, "Invalid operand size");
			template_1operand_mem_uncheck(block, detail::OpcodeLarge(0xD9, 0b111), mem);
		}

		template<AddressMode MODE>
		static void Fstenv(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			write_Unmask_Exception_Prefix(block);
			Fnstenv(block, mem);
		}

		template<AddressMode MODE>
		static void Fnstenv(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_mem_uncheck(block, detail::OpcodeLarge(0xD9, 0b110), mem);
		}

		template<MemSize SIZE = WORD_PTR, AddressMode MODE>
		static void Fstsw(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			write_Unmask_Exception_Prefix(block);
			Fnstsw<SIZE>(block, mem);
		}

		template<MemSize SIZE = WORD_PTR, AddressMode MODE>
		static void Fnstsw(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == WORD_PTR, "Invalid operand size");
			template_1operand_mem_uncheck(block, detail::OpcodeLarge(0xDD, 0b111), mem);
		}

		template<Reg16 REG = AX>
		static void Fstsw(Os::CodeBlock& block) {
			write_Unmask_Exception_Prefix(block);
			Fnstsw<REG>(block);
		}

		template<Reg16 REG = AX>
		static void Fnstsw(Os::CodeBlock& block) {
			static_assert(REG == AX, "Invalid register operand");
			template_2Byte_Opcode(block, 0xDF, 0xE0);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fsub(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b100), mem);
		}

		static void Fsub(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b100), reg);
		}

		static void Fsub(Os::CodeBlock& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDC, 0b101), reg);
		}

		static void Fsubp(Os::CodeBlock& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDE, 0b101), reg);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fisub(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_int(block, detail::OpcodeLarge(0xDA, 0b100), mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fsubr(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_float<SIZE>(block, detail::OpcodeLarge(0xD8, 0b101), mem);
		}

		static void Fsubr(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD8, 0b101), reg);
		}

		static void Fsubr(Os::CodeBlock& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDC, 0b100), reg);
		}

		static void Fsubrp(Os::CodeBlock& block, RegFPU reg, TopRegFPU) {
			template_1operand_float(block, detail::OpcodeLarge(0xDE, 0b100), reg);
		}

		template<MemSize SIZE, AddressMode MODE>
		static void Fisubr(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			template_1operand_int(block, detail::OpcodeLarge(0xDA, 0b101), mem);
		}

		static void Ftst(Os::CodeBlock& block) {
			template_Extended(block, 0xE4);
		}

		static void Fucom(Os::CodeBlock& block, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDD, 0b100), reg);
		}

		static void Fucomp(Os::CodeBlock& block, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDD, 0b101), reg);
		}

		static void Fucompp(Os::CodeBlock& block) {
			template_2Byte_Opcode(block, 0xDA, 0xE9);
		}

		static void Fxam(Os::CodeBlock& block) {
			template_Extended(block, 0xE5);
		}

		static void Fxch(Os::CodeBlock& block, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xD9, 0b001), reg);
		}

		static void Fxtract(Os::CodeBlock& block) {
			template_Extended(block, 0xE4);
		}

		static void FyL2X(Os::CodeBlock& block) {
			template_Extended(block, 0xF1);
		}

		static void FyL2XP1(Os::CodeBlock& block) {
			template_Extended(block, 0xF9);
		}
#pragma endregion
	};
}