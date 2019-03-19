#pragma once

#include <cstdint>
#include <cassert>
#include <utility>
#include <tuple>

#include "x86_base.h"

namespace CppAsm::X86
{
	class i386 {
	protected:
		enum OpMode : uint8_t {
			MODE_MR = 0b0,
			MODE_RM = 0b1,
			MODE_RR = 0b1
		};
		
		static void write_Operand_Segm_Prefix(Os::CodeBlock& block, RegSeg segm) {
			block.pushRaw<uint8_t>(segm);
		}

		template<MemSize SIZE>
		static void write_Opcode(Os::CodeBlock& block, common::Opcode opcode);

		template<>
		static void write_Opcode<BYTE_PTR>(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode(block, opcode);
		}

		template<>
		static void write_Opcode<WORD_PTR>(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, opcode | 0b1);
		}

		template<>
		static void write_Opcode<DWORD_PTR>(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode(block, opcode | 0b1);
		}

		template<MemSize SIZE>
		static void write_Opcode_Only_Extended_Prefixs(Os::CodeBlock& block, common::Opcode opcode);

		template<>
		static void write_Opcode_Only_Extended_Prefixs<WORD_PTR>(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
		}

		template<>
		static void write_Opcode_Only_Extended_Prefixs<DWORD_PTR>(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
		}

		template<MemSize SIZE>
		static void write_Opcode_Extended(Os::CodeBlock& block, common::Opcode opcode);

		template<>
		static void write_Opcode_Extended<BYTE_PTR>(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
		}

		template<>
		static void write_Opcode_Extended<WORD_PTR>(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode | 0b1);
		}

		template<>
		static void write_Opcode_Extended<DWORD_PTR>(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode | 0b1);
		}

		template<MemSize SIZE, MemSize OPT_SIZE>
		static void write_Opcode_Imm_Optimized(Os::CodeBlock& block, common::Opcode opcode);

		template<>
		static void write_Opcode_Imm_Optimized<BYTE_PTR, BYTE_PTR>(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode(block, opcode);
		}

		template<>
		static void write_Opcode_Imm_Optimized<WORD_PTR, WORD_PTR>(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, opcode | 0b1);
		}

		template<>
		static void write_Opcode_Imm_Optimized<WORD_PTR, BYTE_PTR>(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, opcode | 0b11);
		}

		template<>
		static void write_Opcode_Imm_Optimized<DWORD_PTR, DWORD_PTR>(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode(block, opcode | 0b1);
		}

		template<>
		static void write_Opcode_Imm_Optimized<DWORD_PTR, WORD_PTR>(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode(block, opcode | 0b1);
		}

		template<>
		static void write_Opcode_Imm_Optimized<DWORD_PTR, BYTE_PTR>(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode(block, opcode | 0b11);
		}

		template<MemSize SIZE>
		static void write_Opcode(Os::CodeBlock& block, common::Opcode opcode, OpMode rmMode);

		template<>
		static void write_Opcode<BYTE_PTR>(Os::CodeBlock& block, common::Opcode opcode, OpMode rmMode) {
			common::write_Opcode(block, opcode | (rmMode << 1));
		}

		template<>
		static void write_Opcode<WORD_PTR>(Os::CodeBlock& block, common::Opcode opcode, OpMode rmMode) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, opcode | (rmMode << 1) | 0b1);
		}

		template<>
		static void write_Opcode<DWORD_PTR>(Os::CodeBlock& block, common::Opcode opcode, OpMode rmMode) {
			common::write_Opcode(block, opcode | (rmMode << 1) | 0b1);
		}

		template<MemSize SIZE, class T>
		static void write_Imm_Size_Extend(Os::CodeBlock& block, const Imm<T>& imm) {
			block.pushRaw(static_cast<detail::ImmSizeExtend<SIZE, T>::type>(imm));
		}

		template<MemSize SIZE, class T>
		static void write_Imm_Size_Optimize(Os::CodeBlock& block, const Imm<T>& imm) {
			block.pushRaw(static_cast<detail::ImmSizeOptimize<SIZE, T>::type>(imm));
		}

		template<MemSize SIZE, AddressMode MODE>
		static ReplaceableMem32<MODE> template_1mem_operand(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcode.getOpcode());
			return mem.write(block, opcode.getMode());
		}

		template<class REG>
		static ReplaceableReg<REG> template_1reg_operand(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, REG reg);

		template<>
		static ReplaceableReg<Reg8> template_1reg_operand<Reg8>(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, Reg8 reg) {
			write_Opcode<BYTE_PTR>(block, opcode.getOpcode());
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			return ReplaceableReg<Reg8>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
		}

		template<>
		static ReplaceableReg<Reg16> template_1reg_operand<Reg16>(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, Reg16 reg) {
			write_Opcode<WORD_PTR>(block, opcode.getOpcode());
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			return ReplaceableReg<Reg16>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
		}

		template<>
		static ReplaceableReg<Reg32> template_1reg_operand<Reg32>(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, Reg32 reg) {
			write_Opcode<DWORD_PTR>(block, opcode.getOpcode());
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			return ReplaceableReg<Reg32>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
		}

		template<MemSize SIZE, AddressMode MODE, class T>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableExtendedImm<SIZE, T>> template_mem_imm_operands(Os::CodeBlock& block, const detail::OpcodeSet& opcodeSet, const Mem32<MODE>& mem, const Imm<T>& imm) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcodeSet.getSecond().getOpcode());
			auto replaceMem = mem.write(block, opcodeSet.getSecond().getMode());
			Offset offset = block.getOffset();
			write_Imm_Size_Extend<SIZE>(block, imm);
			return std::make_pair(replaceMem, ReplaceableExtendedImm<SIZE, T>(offset));
		}

		template<MemSize SIZE, AddressMode MODE, class T>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableOptimizedImm<SIZE, T>> template_mem_imm_opt_operands(Os::CodeBlock& block, const detail::OpcodeSet& opcodeSet, const Mem32<MODE>& mem, const Imm<T>& imm) {
			mem.writeSegmPrefix(block);
			write_Opcode_Imm_Optimized<SIZE, TypeMemSize<Imm<T>>::value>(block, opcodeSet.getSecond().getOpcode());
			auto replaceMem = mem.write(block, opcodeSet.getSecond().getMode());
			Offset offset = block.getOffset();
			write_Imm_Size_Optimize<SIZE>(block, imm);
			return std::make_pair(replaceMem, ReplaceableOptimizedImm<SIZE, T>(offset));
		}

		template<class REG, class T>
		static std::pair<ReplaceableReg<REG>, ReplaceableOptimizedImm<TypeMemSize<REG>::value, T>> template_reg_imm_opt_operands(Os::CodeBlock& block, const detail::OpcodeSet& opcodeSet, const REG& reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "First param must be register");
			write_Opcode_Imm_Optimized<TypeMemSize<REG>::value, TypeMemSize<Imm<T>>::value>(block, opcodeSet.getSecond().getOpcode());
			Offset modOffset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcodeSet.getSecond().getMode(), reg);
			Offset offset = block.getOffset();
			write_Imm_Size_Optimize<TypeMemSize<REG>::value>(block, imm);
			return std::make_pair(ReplaceableReg<REG>(modOffset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableOptimizedImm<TypeMemSize<REG>::value, T>(offset));
		}

		template<class REG, class T>
		static std::pair<ReplaceableReg<REG>, ReplaceableExtendedImm<TypeMemSize<REG>::value, T>> template_reg_imm_operands(Os::CodeBlock& block, const detail::OpcodeSet& opcodeSet, const REG& reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "First param must be register");
			write_Opcode<TypeMemSize<REG>::value>(block, opcodeSet.opcodeSet.getSecond().getOpcode());
			Offset regOffset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcodeSet.getSecond().getMode(), reg);
			Offset offset = block.getOffset();
			write_Imm_Size_Extend<TypeMemSize<REG>::value>(block, imm);
			return std::make_pair(ReplaceableReg<REG>(regOffset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableExtendedImm<TypeMemSize<REG>::value, T>(offset));
		}

		template<MemSize SIZE, AddressMode MODE>
		static ReplaceableMem32<MODE> template_shift_operands(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcode.getOpcode() | 0x10);
			return mem.write(block, opcode.getMode());
		}

		template<class REG>
		static ReplaceableReg<REG> template_shift_operands(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, const REG& reg) {
			write_Opcode<TypeMemSize<REG>::value>(block, opcode.getOpcode() | 0x10);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			return ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE>
		static ReplaceableMem32<MODE> template_shift_operands(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			static_assert(REG == CL, "Invalid register for shift operation");
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcode.getOpcode() | 0x12);
			return mem.write(block, opcode.getMode());
		}

		template<class REG, Reg8 SH_REG>
		static ReplaceableReg<REG> template_shift_operands(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, const REG& reg) {
			static_assert(SH_REG == CL, "Invalid register for shift operation");
			write_Opcode<TypeMemSize<REG>::value>(block, opcode.getOpcode() | 0x12);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			return ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
		}

		template<MemSize SIZE, AddressMode MODE>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableValue<U8::type>> template_shift_operands(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem, const U8& imm) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcode.getOpcode());
			auto replaceMem = mem.write(block, opcode.getMode());
			Offset offset = block.getOffset();
			common::write_Immediate(block, imm);
			return std::make_pair(replaceMem, ReplaceableValue<U8::type>(offset));
		}

		template<class REG>
		static std::pair<ReplaceableReg<REG>, ReplaceableValue<U8::type>> template_shift_operands(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, const REG& reg, const U8& imm) {
			write_Opcode<TypeMemSize<REG>::value>(opcode.getOpcode());
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			common::write_Immediate(block, imm);
			return std::make_pair(ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableValue<U8::type>(offset + 1));
		}

		template<MemSize SIZE, Reg8 COUNT_REG = CL, AddressMode MODE, class REG>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableReg<REG>> template_prshift_operands(Os::CodeBlock& block, common::Opcode opcode, const Mem32<MODE>& mem, REG reg) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			static_assert(COUNT_REG == CL, "Invalid register for precision shift operation");
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Invalid precision shift size prefix");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<SIZE>(opcode | 0x01);
			auto replaceMem = mem.write(block, reg);
			return std::make_pair(replaceMem, replaceMem.getOtherReg<REG>());
		}

		template<Reg8 COUNT_REG = CL, class REG>
		static std::pair<ReplaceableReg<REG>, ReplaceableReg<REG>> template_prshift_operands(Os::CodeBlock& block, common::Opcode opcode, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			static_assert(COUNT_REG == CL, "Invalid register for precision shift operation");
			static_assert(TypeMemSize<REG>::value == WORD_PTR || TypeMemSize<REG>::value == DWORD_PTR, "Invalid precision shift size prefix");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(opcode | 0x01);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg2, reg1);
			return std::make_pair(ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableReg<REG>(offset, common::MOD_REG_RM::REG_BIT_OFFSET));
		}

		template<MemSize SIZE, AddressMode MODE, class REG>
		static std::tuple<ReplaceableMem32<MODE>, ReplaceableReg<REG>, ReplaceableValue<U8::type>> template_prshift_operands(Os::CodeBlock& block, common::Opcode opcode, const Mem32<MODE>& mem, REG reg, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Invalid precision shift size prefix");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<SIZE>(opcode);
			auto replaceMem = mem.write(block, reg);
			Offset offset = block.getOffset();
			common::write_Immediate(block, imm);
			return std::make_tuple(replaceMem, replaceMem.getOtherReg<REG>(), ReplaceableValue<U8::type>(offset));
		}

		template<class REG>
		static std::tuple<ReplaceableReg<REG>, ReplaceableReg<REG>, ReplaceableValue<U8::type>> template_prshift_operands(Os::CodeBlock& block, common::Opcode opcode, REG reg1, REG reg2, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			static_assert(TypeMemSize<REG>::value == WORD_PTR || TypeMemSize<REG>::value == DWORD_PTR, "Invalid precision shift size prefix");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg2, reg1);
			common::write_Immediate(block, imm);
			return std::make_tuple(ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableReg<REG>(offset, common::MOD_REG_RM::REG_BIT_OFFSET),
				ReplaceableValue<U8::type>(offset + 1));
		}

		static std::pair<ReplaceableReg<Reg8>, ReplaceableReg<Reg8>> template_2operands(Os::CodeBlock& block, common::Opcode opcode, const Reg8& dst, const Reg8& src) {
			write_Opcode<BYTE_PTR>(block, opcode, MODE_RR);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
			return std::make_pair(ReplaceableReg<Reg8>(offset, common::MOD_REG_RM::REG_BIT_OFFSET),
				ReplaceableReg<Reg8>(offset, common::MOD_REG_RM::RM_BIT_OFFSET));
		}

		template<AddressMode MODE>
		static std::pair<ReplaceableReg<Reg8>, ReplaceableMem32<MODE>> template_2operands(Os::CodeBlock& block, common::Opcode opcode, const Reg8& dst, const Mem32<MODE>& src) {
			src.writeSegmPrefix(block);
			write_Opcode<BYTE_PTR>(block, opcode, MODE_RM);
			auto replaceMem = src.write(block, dst);
			return std::make_pair(replaceMem.getOtherReg<Reg8>(), replaceMem);
		}

		template<AddressMode MODE>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableReg<Reg8>> template_2operands(Os::CodeBlock& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg8& src) {
			dst.writeSegmPrefix(block);
			write_Opcode<BYTE_PTR>(block, opcode, MODE_MR);
			auto replaceMem = dst.write(block, src);
			return std::make_pair(replaceMem, replaceMem.getOtherReg<Reg8>());
		}

		static std::pair<ReplaceableReg<Reg16>, ReplaceableReg<Reg16>> template_2operands(Os::CodeBlock& block, common::Opcode opcode, const Reg16& dst, const Reg16& src) {
			write_Opcode<WORD_PTR>(block, opcode, MODE_RR);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
			return std::make_pair(ReplaceableReg<Reg16>(offset, common::MOD_REG_RM::REG_BIT_OFFSET),
				ReplaceableReg<Reg16>(offset, common::MOD_REG_RM::RM_BIT_OFFSET));
		}

		template<AddressMode MODE>
		static std::pair<ReplaceableReg<Reg16>, ReplaceableMem32<MODE>> template_2operands(Os::CodeBlock& block, common::Opcode opcode, const Reg16& dst, const Mem32<MODE>& src) {
			src.writeSegmPrefix(block);
			write_Opcode<WORD_PTR>(block, opcode, MODE_RM);
			auto replaceMem = src.write(block, dst);
			return std::make_pair(replaceMem.getOtherReg<Reg16>(), replaceMem);
		}

		template<AddressMode MODE>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableReg<Reg16>> template_2operands(Os::CodeBlock& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg16& src) {
			dst.writeSegmPrefix(block);
			write_Opcode<WORD_PTR>(block, opcode, MODE_MR);
			auto replaceMem = dst.write(block, src);
			return std::make_pair(replaceMem, replaceMem.getOtherReg<Reg16>());
		}

		static std::pair<ReplaceableReg<Reg32>, ReplaceableReg<Reg32>> template_2operands(Os::CodeBlock& block, common::Opcode opcode, const Reg32& dst, const Reg32& src) {
			write_Opcode<DWORD_PTR>(block, opcode, MODE_RR);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
			return std::make_pair(ReplaceableReg<Reg32>(offset, common::MOD_REG_RM::REG_BIT_OFFSET),
				ReplaceableReg<Reg32>(offset, common::MOD_REG_RM::RM_BIT_OFFSET));
		}

		template<AddressMode MODE>
		static std::pair<ReplaceableReg<Reg32>, ReplaceableMem32<MODE>> template_2operands(Os::CodeBlock& block, common::Opcode opcode, const Reg32& dst, const Mem32<MODE>& src) {
			src.writeSegmPrefix(block);
			write_Opcode<DWORD_PTR>(block, opcode, MODE_RM);
			auto replaceMem = src.write(block, dst);
			return std::make_pair(replaceMem.getOtherReg<Reg32>(), replaceMem);
		}

		template<AddressMode MODE>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableReg<Reg32>> template_2operands(Os::CodeBlock& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg32& src) {
			dst.writeSegmPrefix(block);
			write_Opcode<DWORD_PTR>(block, opcode, MODE_MR);
			auto replaceMem = dst.write(block, src);
			return std::make_pair(replaceMem, replaceMem.getOtherReg<Reg32>());
		}

		template<class DST, class SRC>
		static auto template_2operands_opt_imm(Os::CodeBlock& block, const detail::OpcodeSet& opcodeSet, const DST& dst, const SRC& src) {
			return template_2operands(block, opcodeSet.getMain(), dst, src);
		}

		template<class T>
		static auto template_2operands_opt_imm(Os::CodeBlock& block, const detail::OpcodeSet& opcodeSet, const Reg32& dst, const Imm<T>& src) {
			return template_reg_imm_opt_operands(block, opcodeSet, dst, src);
		}

		template<class T>
		static auto template_2operands_opt_imm(Os::CodeBlock& block, const detail::OpcodeSet& opcodeSet, const Reg16& dst, const Imm<T>& src) {
			return template_reg_imm_opt_operands(block, opcodeSet, dst, src);
		}

		template<class T>
		static auto template_2operands_opt_imm(Os::CodeBlock& block, const detail::OpcodeSet& opcodeSet, const Reg8& dst, const Imm<T>& src) {
			return template_reg_imm_opt_operands(block, opcodeSet, dst, src);
		}

#ifdef X86_SIZE_OPTIMIZED
		template<class REG, class T>
		static bool template_2operands_first_reg_imm(Os::CodeBlock& block, common::Opcode opcode, const REG& reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			if ((reg == 0) && (TypeMemSize<REG>::value == TypeMemSize<Imm<T>>::value)) {
				write_Opcode<TypeMemSize<REG>::value>(block, opcode);
				write_Imm_Size_Extend<TypeMemSize<REG>::value>(block, imm);
				return true;
			}
			return false;
		}
#endif
		template<AddressMode MODE>
		static auto template_2operands_symetric(Os::CodeBlock& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg8& src) {
			return template_2operands(block, opcode, dst, src);
		}

		template<AddressMode MODE>
		static auto template_2operands_symetric(Os::CodeBlock& block, common::Opcode opcode, const Reg8& dst, const Mem32<MODE>& src) {
			return template_2operands(block, opcode, src, dst);
		}

		template<AddressMode MODE>
		static auto template_2operands_symetric(Os::CodeBlock& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg16& src) {
			return template_2operands(block, opcode, dst, src);
		}

		template<AddressMode MODE>
		static auto template_2operands_symetric(Os::CodeBlock& block, common::Opcode opcode, const Reg16& dst, const Mem32<MODE>& src) {
			return template_2operands(block, opcode, src, dst);
		}

		template<AddressMode MODE>
		static auto template_2operands_symetric(Os::CodeBlock& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg32& src) {
			return template_2operands(block, opcode, dst, src);
		}

		template<AddressMode MODE>
		static auto template_2operands_symetric(Os::CodeBlock& block, common::Opcode opcode, const Reg32& dst, const Mem32<MODE>& src) {
			return template_2operands(block, opcode, src, dst);
		}

		template<class DST, class SRC>
		static void template_2operands_xchange(Os::CodeBlock& block, common::Opcode opcode, const DST& dst, const SRC& src) {
			static_assert(TypeMemSize<DST>::value == TypeMemSize<SRC>::value, "Xchange operand size mismatch");
			write_Opcode_Extended<TypeMemSize<REG>::value>(opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, src, dst);
		}

		template<AddressMode MODE, class REG>
		static void template_2operands_xchange(Os::CodeBlock& block, common::Opcode opcode, const Mem32<MODE>& mem, const REG& reg) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			mem.writeSegmPrefix(block);
			write_Opcode_Extended<TypeMemSize<REG>::value>(opcode);
			mem.write(block, reg);
		}

		static std::pair<ReplaceableReg<Reg32>, ReplaceableReg<Reg16>> template_2operands_extend_val(Os::CodeBlock& block, common::Opcode opcode, const Reg32& dst, const Reg16& src) {
			write_Opcode_Extended<TypeMemSize<Reg32>::value>(block, opcode);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
			return std::make_pair(ReplaceableReg<Reg32>(offset, common::MOD_REG_RM::REG_BIT_OFFSET),
				ReplaceableReg<Reg16>(offset, common::MOD_REG_RM::RM_BIT_OFFSET));
		}

		static std::pair<ReplaceableReg<Reg16>, ReplaceableReg<Reg8>> template_2operands_extend_val(Os::CodeBlock& block, common::Opcode opcode, const Reg16& dst, const Reg8& src) {
			write_Opcode_Extended<TypeMemSize<Reg16>::value>(block, opcode);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
			return std::make_pair(ReplaceableReg<Reg16>(offset, common::MOD_REG_RM::REG_BIT_OFFSET),
				ReplaceableReg<Reg8>(offset, common::MOD_REG_RM::RM_BIT_OFFSET));
		}

		template<class REG, AddressMode MODE>
		static std::pair<ReplaceableReg<REG>, ReplaceableMem32<MODE>> template_2operands_extend_val(Os::CodeBlock& block, common::Opcode opcode, const REG& reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			mem.writeSegmPrefix(block);
			write_Opcode_Extended<TypeMemSize<REG>::value>(block, opcode);
			auto replaceMem = mem.write(block, reg);
			return std::make_pair(replaceMem.getOtherReg<REG>(), replaceMem);
		}

		template<class REG>
		static std::pair<ReplaceableReg<REG>, ReplaceableReg<REG>> template_bit_scan(Os::CodeBlock& block, common::Opcode opcode, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "Invalid size of operand");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg1, reg2);
			return std::make_pair(ReplaceableReg<REG>(offset, common::MOD_REG_RM::REG_BIT_OFFSET),
				ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET));
		}

		template<class REG, AddressMode MODE>
		static std::pair<ReplaceableReg<REG>, ReplaceableMem32<MODE>> template_bit_scan(Os::CodeBlock& block, common::Opcode opcode, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "Invalid size of operand");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode);
			auto replaceMem = mem.write(block, reg);
			return std::make_pair(replaceMem.getOtherReg<REG>(), replaceMem);
		}

		template<class REG>
		static std::pair<ReplaceableReg<REG>, ReplaceableReg<REG>> template_bit_operation(Os::CodeBlock& block, const detail::OpcodeSet& opcode, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "Invalid size of operand");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode.getMain());
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg2, reg1);
			return std::make_pair(ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableReg<REG>(offset, common::MOD_REG_RM::REG_BIT_OFFSET));
		}

		template<AddressMode MODE, class REG>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableReg<REG>> template_bit_operation(Os::CodeBlock& block, const detail::OpcodeSet& opcode, const Mem32<MODE>& mem, REG reg) {
			static_assert(IsRegType<REG>::value, "Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "Invalid size of operand");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode.getMain());
			auto replaceMem = mem.write(block, reg);
			return std::make_pair(replaceMem, replaceMem.getOtherReg<REG>());
		}

		template<class REG>
		static std::pair<ReplaceableReg<REG>, ReplaceableValue<U8::type>> template_bit_operation(Os::CodeBlock& block, const detail::OpcodeSet& opcode, REG reg, U8 imm) {
			static_assert(IsRegType<REG>::value, "Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "Invalid size of operand");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode.getSecond().getOpcode());
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getSecond().getMode(), reg);
			common::write_Immediate(block, imm);
			return std::make_pair(ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableValue<U8::type>(offset + 1));
		}

		template<MemSize SIZE, AddressMode MODE>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableValue<U8::type>> template_bit_operation(Os::CodeBlock& block, const detail::OpcodeSet& opcode, const Mem32<MODE>& mem, U8 imm) {
			static_assert(SIZE != BYTE_PTR, "Invalid size of operand");
			write_Opcode_Only_Extended_Prefixs<SIZE>(block, opcode.getSecond().getOpcode());
			auto replaceMem = mem.write(block, opcode.getSecond().getMode());
			Offset offset = block.getOffset();
			common::write_Immediate(block, imm);
			return std::make_pair(replaceMem, ReplaceableValue<U8::type>(offset));
		}

		static ReplaceableReg<Reg8> template_Setcc(Os::CodeBlock& block, common::Opcode opcode, const Reg8& reg) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, 0, reg);
			return ReplaceableReg<Reg8>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
		}

		template<MemSize SIZE = BYTE_PTR, AddressMode MODE>
		static ReplaceableMem32<MODE> template_Setcc(Os::CodeBlock& block, common::Opcode opcode, const Mem32<MODE>& mem) {
			static_assert(SIZE == BYTE_PTR, "Setcc: Invalid size of operand");
			mem.writeSegmPrefix(block);
			write_Operand_Extended_Prefix(block);
			write_Opcode(block, opcode);
			return mem.write(block, 0);
		}

		template<JmpSize SIZE>
		static FwdLabel<SIZE> template_Jxx(Os::CodeBlock& block, common::Opcode opcode);

		template<>
		static FwdLabel<SHORT> template_Jxx(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode(block, opcode);
			Offset offset = block.getOffset();
			block.skipBytes(FwdLabel<SHORT>::offset_size);
			return FwdLabel<SHORT>(offset);
		}

		template<>
		static FwdLabel<LONG> template_Jxx(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode & 0x0F | 0x80);
			Offset offset = block.getOffset();
			block.skipBytes(FwdLabel<LONG>::offset_size);
			return FwdLabel<LONG>(offset);
		}

		template<JmpSize SIZE>
		static FwdLabel<SIZE> template_Jmp(Os::CodeBlock& block);

		template<>
		static FwdLabel<SHORT> template_Jmp(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xEB);
			Offset offset = block.getOffset();
			block.skipBytes(FwdLabel<SHORT>::offset_size);
			return FwdLabel<SHORT>(offset);
		}

		template<>
		static FwdLabel<LONG> template_Jmp(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xE9);
			Offset offset = block.getOffset();
			block.skipBytes(FwdLabel<LONG>::offset_size);
			return FwdLabel<LONG>(offset);
		}

		template<AddressMode MODE>
		static ReplaceableMem32<MODE> template_Jmp(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<DWORD_PTR>(block, detail::opcode_JMP, mem);
		}

		static ReplaceableReg<Reg32> template_Jmp(Os::CodeBlock& block, const Reg32& reg) {
			return template_1reg_operand(block, detail::opcode_JMP, reg);
		}

		static void template_Call(Os::CodeBlock& block, const Addr& jumpAddress) {
			common::write_Opcode(block, 0xE8);
			block.pushRaw(common::calc_Jump_Offset(block.getCurrentPtr(), jumpAddress, sizeof(Addr)));
		}

		template<AddressMode MODE>
		static ReplaceableMem32<MODE> template_Call(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<DWORD_PTR>(block, detail::opcode_CALL, mem);
		}

		static ReplaceableReg<Reg32> template_Call(Os::CodeBlock& block, const Reg32& reg) {
			return template_1reg_operand(block, detail::opcode_CALL, reg);
		}

		template<JmpSize SIZE>
		static FwdLabel<SIZE> template_Loop(Os::CodeBlock& block, common::Opcode opcode);

		template<>
		static FwdLabel<SHORT> template_Loop(Os::CodeBlock& block, common::Opcode opcode) {
			common::write_Opcode(block, opcode);
			Offset offset = block.getOffset();
			block.skipBytes(FwdLabel<SHORT>::offset_size);
			return FwdLabel<SHORT>(offset);
		}
	public:
#pragma region Data transfer [DONE]
		/* Convert byte to word */
		static void Cbw(Os::CodeBlock& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x98);
		}

		/* Convert byte to word */
		static void Cdq(Os::CodeBlock& block) {
			common::write_Opcode(block, 0x99);
		}

		/* Convert word to double word */
		static void Cwd(Os::CodeBlock& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x99);
		}

		/* Convert word to double word */
		static void Cwde(Os::CodeBlock& block) {
			common::write_Opcode(block, 0x98);
		}

		/* Calculate effective address
		 - LEA reg16, [mem]
		*/
		template<AddressMode MODE>
		static auto Lea(Os::CodeBlock& block, Reg16 reg, const Mem32<MODE>& mem) {
			return template_2operands(block, 0x8D, mem, reg);
		}

		/* Calculate effective address
		 - LEA reg32, [mem]
		*/
		template<AddressMode MODE>
		static auto Lea(Os::CodeBlock& block, Reg32 reg, const Mem32<MODE>& mem) {
			return template_2operands(block, 0x8D, mem, reg);
		}

		/* Moving data
		 - MOV reg,reg
		 - MOV reg,[mem]
		 - MOV [mem],reg
		*/
		template<class DST, class SRC>
		static auto Mov(Os::CodeBlock& block, const DST& dst, const SRC& src) {
			return template_2operands(block, detail::opcode_MOV.getMain(), dst, src);
		}

		/* Moving data
		 - MOV [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T>
		static auto Mov(Os::CodeBlock& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_operands<SIZE>(block, detail::opcode_MOV, mem, imm);
		}

		/* Moving data
		 - MOV reg,imm
		*/
		template<class REG, class T>
		static auto Mov(Os::CodeBlock& block, const REG& reg, const Imm<T>& imm) {
			return template_reg_imm_operands(block, detail::opcode_MOV, reg, imm);
		}
		
		/* Mov with sign extend
		 - MOVSX reg,reg
		 - MOVSX reg,[mem]
		*/
		template<class DST, class SRC>
		static auto Movsx(Os::CodeBlock& block, const DST& dst, const SRC& src) {
			return template_2operands_extend_val(block, 0xBE, dst, src);
		}

		/* Mov with zero extend
		 - MOVZX reg,reg
		 - MOVZX reg,[mem]
		*/
		template<class DST, class SRC>
		static auto Movzx(Os::CodeBlock& block, const DST& dst, const SRC& src) {
			return template_2operands_extend_val(block, 0xB6, dst, src);
		}

		/* Push value to the stack
		 - PUSH reg16
		*/
		static ReplaceableReg<Reg16> Push(Os::CodeBlock& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x50 | reg);
			return ReplaceableReg<Reg16>(offset, 0);
		}

		/* Push value to the stack
		 - PUSH reg32
		*/
		static ReplaceableReg<Reg32> Push(Os::CodeBlock& block, Reg32 reg) {
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x50 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Push value to the stack
		- PUSH imm8
		*/
		static ReplaceableValue<S8> Push(Os::CodeBlock& block, S8 number) {
			common::write_Opcode(block, 0x6A);
			Offset offset = block.getOffset();
			common::write_Immediate(block, number);
			return ReplaceableValue<S8>(offset);
		}

		/* Push value to the stack
		- PUSH imm32
		*/
		static ReplaceableValue<S32> Push(Os::CodeBlock& block, S32 number) {
			common::write_Opcode(block, 0x68);
			Offset offset = block.getOffset();
			common::write_Immediate(block, number);
			return ReplaceableValue<S32>(offset);
		}

		/* Push value to the stack
		- PUSH [mem]
		*/
		template<MemSize SIZE, AddressMode MODE>
		static ReplaceableMem32<MODE> Push(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_PUSH, mem);
		}

		/* Store all 16 bit general registers to stack */
		static void Pusha(Os::CodeBlock& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x60);
		}

		/* Store all 32 bit general registers to stack */
		static void Pushad(Os::CodeBlock& block) {
			common::write_Opcode(block, 0x60);
		}

		/* Pop value from the stack
		- POP reg16
		*/
		static ReplaceableReg<Reg16> Pop(Os::CodeBlock& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x58 | reg);
			return ReplaceableReg<Reg16>(offset, 0);
		}

		/* Pop value from the stack
		- POP reg32
		*/
		static ReplaceableReg<Reg32> Pop(Os::CodeBlock& block, Reg32 reg) {
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x58 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Pop value from the stack
		- POP [mem]
		*/
		template<MemSize SIZE, AddressMode MODE>
		static ReplaceableMem32<MODE> Pop(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Invalid Pop size prefix");
			return template_1mem_operand<SIZE>(block, detail::opcode_POP, mem);
		}
		
		/* Load all 16 bit general registers from stack */
		static void Popa(Os::CodeBlock& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x61);
		}

		/* Load all 32 bit general registers from stack */
		static void Popad(Os::CodeBlock& block) {
			common::write_Opcode(block, 0x61);
		}

		/* Xchange
		 - XCHG reg,reg
		 - XCHG reg,[mem]
		 - XCHG [mem],reg
		*/
		template<class DST, class SRC>
		static auto Xchg(Os::CodeBlock& block, const DST& dst, const SRC& src) {
			return template_2operands_symetric(block, 0x86, dst, src);
		}

		/* Translate by table */
		static void Xlat(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xD7);
		}

		/* Read data from port
		 - IN reg,imm
		*/
		template<MemSize SIZE>
		static ReplaceableValue<U8> In(Os::CodeBlock& block, const U8& imm) {
			write_Opcode<SIZE>(block, 0xE4);
			Offset offset = block.getOffset();
			common::write_Immediate(block, imm);
			return ReplaceableValue<U8>(offset);
		}

		/* Read data from port
		 - IN eax/ax,dx
		*/
		template<MemSize SIZE, Reg16 PORT_REG>
		static void In(Os::CodeBlock& block) {
			write_Opcode<SIZE>(block, 0xEC);
		}

		/* Write data to port
		 - OUT imm, reg
		*/
		template<MemSize SIZE>
		static ReplaceableValue<U8> Out(Os::CodeBlock& block, const U8& imm) {
			write_Opcode<SIZE>(block, 0xE6);
			Offset offset = block.getOffset();
			common::write_Immediate(block, imm);
			return ReplaceableValue<U8>(offset);
		}

		/* Write data to port
		 - OUT dx,eax/ax
		*/
		template<MemSize SIZE, Reg16 PORT_REG>
		static void Out(Os::CodeBlock& block) {
			write_Opcode<SIZE>(block, 0xEE);
		}
#pragma endregion

#pragma region Binary arithmetics [DONE]
		/* Addition with carry flag (CF)
		 - ADC reg,reg
		 - ADC reg,[mem]
		 - ADC [mem],reg
		 - ADC reg,imm
		*/
		template<class DST, class SRC>
		static auto Adc(Os::CodeBlock& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_ADC, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Addition with carry flag (CF)
		 - ADC reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T>
		static void Adc(Os::CodeBlock& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x14, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_ADC, reg, imm);
			}
		}
#endif
		/* Addition with carry flag (CF)
		 - ADC [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T>
		static auto Adc(Os::CodeBlock& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_ADC, mem, imm);
		}

		/* Addition
		 - ADD reg,reg
		 - ADD reg,[mem]
		 - ADD [mem],reg
		 - ADD reg,imm
		*/
		template<class DST, class SRC>
		static auto Add(Os::CodeBlock& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_ADD, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Addition
		 - ADD reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T>
		static void Add(Os::CodeBlock& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x04, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_ADD, reg, imm);
			}
		}
#endif
		/* Addition
		 - ADD [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T>
		static auto Add(Os::CodeBlock& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_ADD, mem, imm);
		}

		/* Compare Two Operands
		 - CMP reg,reg
		 - CMP reg,[mem]
		 - CMP [mem],reg
		 - CMP reg,imm
		*/
		template<class DST, class SRC>
		static auto Cmp(Os::CodeBlock& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_CMP, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Compare Two Operands
		 - CMP reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T>
		static void Cmp(Os::CodeBlock& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x3C, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_CMP, reg, imm);
			}
		}
#endif
		/* Compare Two Operands
		 - CMP [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T>
		static auto Cmp(Os::CodeBlock& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_CMP, mem, imm);
		}

		/* Compare and set flags
		 - CMP imm,[mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class T>
		static auto Cmp(Os::CodeBlock& block, const Imm<T>& imm, const Mem32<MODE>& mem) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_CMP, mem, imm);
		}

		/* Increment
		 - INC reg32
		*/
		static ReplaceableReg<Reg32> Inc(Os::CodeBlock& block, Reg32 reg) {
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x40 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Increment
		 - INC reg16
		*/
		static ReplaceableReg<Reg16> Inc(Os::CodeBlock& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x40 | reg);
			return ReplaceableReg<Reg16>(offset, 0);
		}

		/* Increment
		 - INC reg8
		*/
		static ReplaceableReg<Reg8> Inc(Os::CodeBlock& block, Reg8 reg) {
			return template_1reg_operand(block, detail::opcode_INC, reg);
		}

		/* Increment
		 - INC [mem]
		*/
		template<MemSize SIZE, AddressMode MODE>
		static ReplaceableMem32<MODE> Inc(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_INC, mem);
		}

		/* Decrement
		 - DEC reg32
		*/
		static ReplaceableReg<Reg32> Dec(Os::CodeBlock& block, Reg32 reg) {
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x48 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Decrement
		 - DEC reg16
		*/
		static ReplaceableReg<Reg16> Dec(Os::CodeBlock& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x48 | reg);
			return ReplaceableReg<Reg16>(offset, 0);
		}

		/* Decrement
		 - DEC reg8
		*/
		static ReplaceableReg<Reg8> Dec(Os::CodeBlock& block, Reg8 reg) {
			return template_1reg_operand(block, detail::opcode_DEC, reg);
		}

		/* Decrement
		- DEC [mem]
		*/
		template<MemSize SIZE, AddressMode MODE>
		static ReplaceableMem32<MODE> Dec(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_DEC, mem);
		}

		/* Change sign
		 - NEG reg
		*/
		template<class REG>
		static ReplaceableReg<REG> Neg(Os::CodeBlock& block, REG reg) {
			return template_1reg_operand(block, detail::opcode_NEG, reg);
		}

		/* Change sign
		 - NEG [mem]
		*/
		template<MemSize SIZE, AddressMode MODE>
		static ReplaceableMem32<MODE> Neg(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_NEG, mem);
		}

		/* Multiply unsigned numbers
		 - MUL reg
		*/
		template<class REG>
		static ReplaceableReg<REG> Mul(Os::CodeBlock& block, REG reg) {
			return template_1reg_operand(block, detail::opcode_MUL, reg);
		}

		/* Multiply unsigned numbers
		 - MUL [mem]
		*/
		template<MemSize SIZE, AddressMode MODE>
		static ReplaceableMem32<MODE> Mul(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_MUL, mem);
		}

		/* Signed Multiply
		 - IMUL reg
		*/
		template<class REG>
		static ReplaceableReg<REG> Imul(Os::CodeBlock& block, REG reg) {
			return template_1reg_operand(block, detail::opcode_IMUL, reg);
		}

		/* Signed Multiply
		 - IMUL [mem]
		*/
		template<MemSize SIZE, AddressMode MODE>
		static ReplaceableMem32<MODE> Imul(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_IMUL, mem);
		}

		/* TODO: IMUL reg,imm
		 * TODO: IMUL reg,reg
		 * TODO: IMUL reg,[mem]
		 * TODO: IMUL reg,reg,imm
		 * TODO: IMUL reg,[mem],imm */

		/* Divide unsigned numbers
		 - DIV reg
		*/
		template<class REG>
		static ReplaceableReg<REG> Div(Os::CodeBlock& block, REG reg) {
			return template_1reg_operand(block, detail::opcode_DIV, reg);
		}

		/* Divide unsigned numbers
		 - DIV [mem]
		*/
		template<MemSize SIZE, AddressMode MODE>
		static ReplaceableMem32<MODE> Div(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_DIV, mem);
		}

		/* Divide signed numbers
		 - IDIV reg
		*/
		template<class REG>
		static ReplaceableReg<REG> Idiv(REG reg) {
			return template_1reg_operand(block, detail::opcode_IDIV, reg);
		}

		/* Divide signed numbers
		 - IDIV [mem]
		*/
		template<MemSize SIZE, AddressMode MODE>
		static ReplaceableMem32<MODE> Idiv(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_IDIV, mem);
		}

		/* Integer Subtraction with Borrow
		 - SBB reg,reg
		 - SBB reg,[mem]
		 - SBB [mem],reg
		 - SBB reg,imm
		*/
		template<class DST, class SRC>
		static auto Sbb(Os::CodeBlock& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_SBB, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Integer Subtraction with Borrow
		 - SBB reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T>
		static void Sbb(Os::CodeBlock& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x1C, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_SBB, reg, imm);
			}
		}
#endif
		/* Integer Subtraction with Borrow
		 - SBB [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T>
		static auto Sbb(Os::CodeBlock& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_SBB, mem, imm);
		}

		/* Subtract
		 - SUB reg,reg
		 - SUB reg,[mem]
		 - SUB [mem],reg
		 - SUB reg,imm
		*/
		template<class DST, class SRC>
		static auto Sub(Os::CodeBlock& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_SUB, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Subtract
		 - SUB reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T>
		static void Sub(Os::CodeBlock& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x2C, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_SBB, reg, imm);
			}
		}
#endif
		/* Subtract
		 - SUB [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T>
		static auto Sub(Os::CodeBlock& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_SUB, mem, imm);
		}
#pragma endregion

#pragma region Decimal arithmetics [DONE]
		/* ACII correction after addition */
		static void Aaa(Os::CodeBlock& block) {
			common::write_Opcode(block, 0x37);
		}

		/* ASCII correction before divide 
		 - AAD {imm} 
		 */
		static ReplaceableValue<S8::type> Aad(Os::CodeBlock& block, S8 base = S8(10)) {
			common::write_Opcode(block, 0xD5);
			Offset offset = block.getOffset();
			common::write_Immediate(block, base);
			return ReplaceableValue<S8::type>(offset);
		}

		/* ASCII correction after multiply
		 - AAM {imm}
		 */
		static ReplaceableValue<S8::type> Aam(Os::CodeBlock& block, S8 base = S8(10)) {
			common::write_Opcode(block, 0xD4);
			Offset offset = block.getOffset();
			common::write_Immediate(block, base);
			return ReplaceableValue<S8::type>(offset);
		}

		/* ASCII correction after subtraction */
		static void Aas(Os::CodeBlock& block) {
			common::write_Opcode(block, 0x3F);
		}

		/* BCD-correction after addition */
		static void Daa(Os::CodeBlock& block) {
			common::write_Opcode(block, 0x27);
		}

		/* BCD-correction after subtraction */
		static void Das(Os::CodeBlock& block) {
			common::write_Opcode(block, 0x2F);
		}
#pragma endregion

#pragma region Logical operations [DONE]
		/* Logical AND
		 - AND reg,reg
		 - AND reg,[mem]
		 - AND [mem],reg
		 - AND reg,imm
		*/
		template<class DST, class SRC>
		static auto And(Os::CodeBlock& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_AND, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Logical AND
		 - AND reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T>
		static void And(Os::CodeBlock& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x24, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_AND, reg, imm);
			}
		}
#endif
		/* Logical AND
		 - AND [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T>
		static auto And(Os::CodeBlock& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_AND, mem, imm);
		}

		/* Logical NOT operation
		 - NOT reg
		*/
		template<class REG>
		static ReplaceableReg<REG> Not(Os::CodeBlock& block, REG reg) {
			return template_1reg_operand(block, detail::opcode_NOT, reg);
		}

		/* Logical NOT operation
		 - NOT [mem]
		*/
		template<MemSize SIZE, AddressMode MODE>
		static ReplaceableMem32<MODE> Not(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_NOT, mem);
		}

		/* Logical OR operation
		 - OR reg,reg
		 - OR reg,[mem]
		 - OR [mem],reg
		 - OR reg,imm
		*/
		template<class DST, class SRC>
		static auto Or(Os::CodeBlock& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_OR, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Logical OR
		 - OR reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T>
		static void Or(Os::CodeBlock& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x0C, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_OR, reg, imm);
			}
		}
#endif
		/* Logical OR operation
		 - OR [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T>
		static auto Or(Os::CodeBlock& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_OR, mem, imm);
		}

		/* Logical Compare
		 - TEST reg,reg
		 - TEST reg,[mem]
		 - TEST [mem],reg
		*/
		template<class DST, class SRC>
		static auto Test(Os::CodeBlock& block, const DST& dst, const SRC& src) {
			return template_2operands_symetric(block, detail::opcode_TEST.getMain(), dst, src);
		}

		/* Logical Compare
		 - TEST [mem],imm
		 */
		template<MemSize SIZE, AddressMode MODE, class T>
		static auto Test(Os::CodeBlock& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_operands<SIZE>(block, detail::opcode_TEST, mem, imm);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Logical Compare
		 - TEST reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T>
		static void Test(Os::CodeBlock& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0xA8, reg, imm)) {
				template_2operands_symetric(block, detail::opcode_TEST.getMain(), reg, imm);
			}
		}
#endif
		/* Logical Exclusive OR
		 - XOR reg,reg
		 - XOR reg,[mem]
		 - XOR [mem],reg
		 - XOR reg,imm
		*/
		template<class DST, class SRC>
		static auto Xor(Os::CodeBlock& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_XOR, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Logical Exclusive OR
		 - XOR reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T>
		static void Xor(Os::CodeBlock& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x34, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_XOR, reg, imm);
			}
		}
#endif
		/* Logical Exclusive OR
		 - XOR [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T>
		static auto Xor(Os::CodeBlock& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_XOR, mem, imm);
		}
#pragma endregion
		
#pragma region Bit shift operations [DONE]
		template<MemSize SIZE, Reg8 REG, AddressMode MODE>
		static auto Rol(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_ROL, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Rol(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_ROL, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Rol(Os::CodeBlock& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_ROL, mem, imm);
		}

		template<class REG>
		static auto Rol(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_ROL, reg);
		}

		template<class REG, Reg8 SH_REG>
		static auto Rol(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_ROL, reg);
		}

		template<class REG>
		static auto Rol(Os::CodeBlock& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_ROL, reg, imm);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE>
		static auto Ror(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_ROR, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Ror(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_ROR, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Ror(Os::CodeBlock& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_ROR, mem, imm);
		}

		template<class REG>
		static auto Ror(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_ROR, reg);
		}

		template<class REG, Reg8 SH_REG>
		static auto Ror(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_ROR, reg);
		}

		template<class REG>
		static auto Ror(Os::CodeBlock& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_ROR, reg, imm);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE>
		static auto Rcl(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_RCL, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Rcl(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_RCL, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Rcl(Os::CodeBlock& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_RCL, mem, imm);
		}

		template<class REG>
		static auto Rcl(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_RCL, reg);
		}

		template<class REG, Reg8 SH_REG>
		static auto Rcl(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_RCL, reg);
		}

		template<class REG>
		auto Rcl(Os::CodeBlock& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_RCL, reg, imm);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE>
		static auto Rcr(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_RCR, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Rcr(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_RCR, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Rcr(Os::CodeBlock& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_RCR, mem, imm);
		}

		template<class REG>
		static auto Rcr(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_RCR, reg);
		}

		template<class REG, Reg8 SH_REG>
		static auto Rcr(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_RCR, reg);
		}

		template<class REG>
		static auto Rcr(Os::CodeBlock& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_RCR, reg, imm);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE>
		static auto Sal(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_SAL, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Sal(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_SAL, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Sal(Os::CodeBlock& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_SAL, mem, imm);
		}

		template<class REG>
		static auto Sal(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_SAL, reg);
		}

		template<class REG, Reg8 SH_REG>
		static auto Sal(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_SAL, reg);
		}

		template<class REG>
		static auto Sal(Os::CodeBlock& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_SAL, reg, imm);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE>
		static auto Shl(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_SHL, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Shl(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_SHL, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Shl(Os::CodeBlock& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_SHL, mem, imm);
		}

		template<class REG>
		static auto Shl(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_SHL, reg);
		}

		template<class REG, Reg8 SH_REG>
		static auto Shl(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_SHL, reg);
		}

		template<class REG>
		static auto Shl(Os::CodeBlock& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_SHL, reg, imm);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE>
		static auto Shr(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_SHR, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Shr(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_SHR, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Shr(Os::CodeBlock& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_SHR, mem, imm);
		}

		template<class REG>
		static auto Shr(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_SHR, reg);
		}

		template<class REG, Reg8 SH_REG>
		static auto Shr(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_SHR, reg);
		}

		template<class REG>
		static auto Shr(Os::CodeBlock& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_SHR, reg, imm);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE>
		static auto Sar(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_SAR, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Sar(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_SAR, mem);
		}

		template<MemSize SIZE, AddressMode MODE>
		static auto Sar(Os::CodeBlock& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_SAR, mem, imm);
		}

		template<class REG>
		static auto Sar(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_SAR, reg);
		}

		template<class REG, Reg8 SH_REG>
		static auto Sar(Os::CodeBlock& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_SAR, reg);
		}

		template<class REG>
		static auto Sar(Os::CodeBlock& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_SAR, reg, imm);
		}

		template<MemSize SIZE, Reg8 COUNT_REG, class REG, AddressMode MODE>
		static auto Shrd(Os::CodeBlock& block, const Mem32<MODE>& mem, REG reg) {
			return template_prshift_operands<SIZE, COUNT_REG>(block, 0xAC, mem, reg);
		}

		template<Reg8 COUNT_REG, class REG>
		static auto Shrd(Os::CodeBlock& block, REG reg1, REG reg2) {
			return template_prshift_operands<COUNT_REG>(block, 0xAC, reg1, reg2);
		}

		template<MemSize SIZE, class REG, AddressMode MODE>
		static auto Shrd(Os::CodeBlock& block, const Mem32<MODE>& mem, REG reg, const U8& imm) {
			return template_prshift_operands<SIZE>(block, 0xAC, mem, reg, imm);
		}

		template<class REG>
		static auto Shrd(Os::CodeBlock& block, REG reg1, REG reg2, const U8& imm) {
			return template_prshift_operands(block, 0xAC, reg1, reg2, imm);
		}

		template<MemSize SIZE, Reg8 COUNT_REG, class REG, AddressMode MODE>
		static auto Shld(Os::CodeBlock& block, const Mem32<MODE>& mem, REG reg) {
			return template_prshift_operands<SIZE, COUNT_REG>(block, 0xA4, mem, reg);
		}

		template<Reg8 COUNT_REG, class REG>
		static auto Shld(Os::CodeBlock& block, REG reg1, REG reg2) {
			return template_prshift_operands<COUNT_REG>(block, 0xA4, reg1, reg2);
		}

		template<MemSize SIZE, class REG, AddressMode MODE>
		static auto Shld(Os::CodeBlock& block, const Mem32<MODE>& mem, REG reg, const U8& imm) {
			return template_prshift_operands<SIZE>(block, 0xA4, mem, reg, imm);
		}

		template<class REG>
		static auto Shld(Os::CodeBlock& block, REG reg1, REG reg2, const U8& imm) {
			return template_prshift_operands(block, 0xA4, reg1, reg2, imm);
		}
#pragma endregion

#pragma region Bit manipulations [DONE]
		template<class REG>
		auto Bsf(Os::CodeBlock& block, REG reg1, REG reg2) {
			return template_bit_scan(block, 0xBC, reg1, reg2);
		}

		template<class REG, AddressMode MODE>
		auto Bsf(Os::CodeBlock& block, REG reg, const Mem32<MODE>& mem) {
			return template_bit_scan(block, 0xBC, reg, mem);
		}

		template<class REG>
		auto Bsr(Os::CodeBlock& block, REG reg1, REG reg2) {
			return template_bit_scan(block, 0xBD, reg1, reg2);
		}

		template<class REG, AddressMode MODE>
		auto Bsr(Os::CodeBlock& block, REG reg, const Mem32<MODE>& mem) {
			return template_bit_scan(block, 0xBD, reg, mem);
		}

		template<class T1, class T2>
		auto Bt(Os::CodeBlock& block, T1 val1, T2 val2) {
			return template_bit_operation(block, detail::opcode_BT, val1, val2);
		}

		template<MemSize SIZE, AddressMode MODE>
		auto Bt(Os::CodeBlock& block, const Mem32<MODE>& mem, U8 imm) {
			return template_bit_operation<SIZE>(block, detail::opcode_BT, mem, imm);
		}

		template<class T1, class T2>
		auto Btc(Os::CodeBlock& block, T1 val1, T2 val2) {
			return template_bit_operation(block, detail::opcode_BTC, val1, val2);
		}

		template<MemSize SIZE, AddressMode MODE>
		auto Btc(Os::CodeBlock& block, const Mem32<MODE>& mem, U8 imm) {
			return template_bit_operation<SIZE>(block, detail::opcode_BTC, mem, imm);
		}

		template<class T1, class T2>
		auto Btr(Os::CodeBlock& block, T1 val1, T2 val2) {
			return template_bit_operation(block, detail::opcode_BTR, val1, val2);
		}

		template<MemSize SIZE, AddressMode MODE>
		auto Btr(Os::CodeBlock& block, const Mem32<MODE>& mem, U8 imm) {
			return template_bit_operation<SIZE>(block, detail::opcode_BTR, mem, imm);
		}

		template<class T1, class T2>
		auto Bts(Os::CodeBlock& block, T1 val1, T2 val2) {
			return template_bit_operation(block, detail::opcode_BTS, val1, val2);
		}

		template<MemSize SIZE, AddressMode MODE>
		auto Bts(Os::CodeBlock& block, const Mem32<MODE>& mem, U8 imm) {
			return template_bit_operation<SIZE>(block, detail::opcode_BTS, mem, imm);
		}

		/* Set byte if overflow (OF == 1) */
		template<class T>
		auto Seto(Os::CodeBlock& block, const T& val) {
			return template_Setcc(block, 0x90, val);
		}

		/* Set byte if no overflow (OF == 0) */
		template<class T>
		auto Setno(Os::CodeBlock& block, const T& val) {
			return template_Setcc(block, 0x91, val);
		}

		/* Set byte if unsigned lower (CF == 1) */
		template<class T>
		auto Setb(Os::CodeBlock& block, const T& val) {
			return template_Setcc(block, 0x92, val);
		}

		/* Set byte if carry (CF == 1) */
		template<class T>
		auto Setc(Os::CodeBlock& block, const T& val) {
			return Setb(block, val);
		}

		/* Set byte if unsigned not greater-equal (CF == 1) */
		template<class T>
		auto Setnae(Os::CodeBlock& block, const T& val) {
			return Setb(block, val);
		}

		/* Set byte if unsigned greater-equal (CF == 0) */
		template<class T>
		auto Setae(Os::CodeBlock& block, const T& val) {
			return template_Setcc(block, 0x93, val);
		}

		/* Set byte if no carry (CF == 0) */
		template<class T>
		auto Setnc(Os::CodeBlock& block, const T& val) {
			return Setae(block, val);
		}

		/* Set byte if unsigned not lower (CF == 0) */
		template<class T>
		auto Setnb(Os::CodeBlock& block, const T& val) {
			return Setae(block, val);
		}

		/* Set byte if equal (ZF == 1) */
		template<class T>
		auto Sete(Os::CodeBlock& block, const T& val) {
			return template_Setcc(block, 0x94, val);
		}

		/* Set byte if zero (ZF == 1) */
		template<class T>
		auto Setz(Os::CodeBlock& block, const T& val) {
			return Sete(block, val);
		}

		/* Set byte if not equal (ZF == 0) */
		template<class T>
		auto Setne(Os::CodeBlock& block, const T& val) {
			return template_Setcc(block, 0x95, val);
		}

		/* Set byte if not zero (ZF == 0) */
		template<class T>
		auto Setnz(Os::CodeBlock& block, const T& val) {
			return Setne(block, val);
		}

		/* Set byte if unsigned lower-equal (CF == 1 || ZF == 1) */
		template<class T>
		auto Setbe(Os::CodeBlock& block, const T& val) {
			return template_Setcc(block, 0x96, val);
		}

		/* Set byte if unsigned not greater (CF == 1 || ZF == 1) */
		template<class T>
		auto Setna(Os::CodeBlock& block, const T& val) {
			return Setbe(block, val);
		}

		/* Set byte if unsigned greater (CF == 0 && ZF == 0) */
		template<class T>
		auto Seta(Os::CodeBlock& block, const T& val) {
			return template_Setcc(block, 0x97, val);
		}

		/* Set byte if unsigned not lower-equal (CF == 0 && ZF == 0) */
		template<class T>
		auto Setnbe(Os::CodeBlock& block, const T& val) {
			return Seta(block, val);
		}

		/* Set byte if sign (SF == 1) */
		template<class T>
		auto Sets(Os::CodeBlock& block, const T& val) {
			return template_Setcc(block, 0x98, val);
		}

		/* Set byte if no sign (SF == 0) */
		template<class T>
		auto Setns(Os::CodeBlock& block, const T& val) {
			return template_Setcc(block, 0x99, val);
		}

		/* Set byte if parity (PF == 1) */
		template<class T>
		auto Setp(Os::CodeBlock& block, const T& val) {
			return template_Setcc(block, 0x9A, val);
		}

		/* Set byte if parity (PF == 1) */
		template<class T>
		auto Setpe(Os::CodeBlock& block, const T& val) {
			return Setp(block, val);
		}

		/* Set byte if no parity (PF == 0) */
		template<class T>
		auto Setnp(Os::CodeBlock& block, const T& val) {
			return template_Setcc(block, 0x9B, val);
		}

		/* Set byte if no parity (PF == 0) */
		template<class T>
		auto Setpo(Os::CodeBlock& block, const T& val) {
			return Setnp(block, val);
		}

		/* Set byte if signed lower (SF != OF) */
		template<class T>
		auto Setl(Os::CodeBlock& block, const T& val) {
			return template_Setcc(block, 0x9C, val);
		}

		/* Set byte if signed not greater-equal (SF != OF) */
		template<class T>
		auto Setnge(Os::CodeBlock& block, const T& val) {
			return Setl(block, val);
		}

		/* Set byte if signed greater-equal (ZF == 0 && SF == OF) */
		template<class T>
		auto Setge(Os::CodeBlock& block, const T& val) {
			return template_Setcc(block, 0x9D, val);
		}

		/* Set byte if signed not lower (ZF == 0 && SF == OF) */
		template<class T>
		auto Setnl(Os::CodeBlock& block, const T& val) {
			return Setge(block, val);
		}

		/* Set byte if signed lower-equal (ZF == 1 && SF != OF) */
		template<class T>
		auto Setle(Os::CodeBlock& block, const T& val) {
			return template_Setcc(block, 0x9E, val);
		}

		/* Set byte if signed not greater (ZF == 1 && SF != OF) */
		template<class T>
		auto Setng(Os::CodeBlock& block, const T& val) {
			return Setle(block, val);
		}

		/* Set byte if signed greater (ZF == 0 && SF == OF) */
		template<class T>
		auto Setg(Os::CodeBlock& block) {
			return template_Setcc(block, 0x9F, val);
		}

		/* Set byte if signed not lower-equal (ZF == 0 && SF == OF) */
		template<class T>
		auto Setnle(Os::CodeBlock& block, const T& val) {
			return Setg(block, val);
		}
#pragma endregion

#pragma region Flow control
		/* Call procedure
		 - CALL reg
		 - CALL [mem]
		*/
		template<class T>
		auto Call(Os::CodeBlock& block, const T& addr) {
			return template_Call(block, addr);
		}

		/* Call procedure
		 - CALL imm_addr
		 - CALL label
		*/
		FwdLabel<LONG> Call(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xE8);
			Offset offset = block.getOffset();
			block.skipBytes(FwdLabel<LONG>::offset_size);
			return FwdLabel<LONG>(offset);
		}

		/* Jump always
		 - JMP reg
		 - JMP [mem]
		*/
		template<class T>
		auto Jmp(Os::CodeBlock& block, const T& addr) {
			return template_Jmp(block, addr);
		}

		/* Jump always
		 - JMP imm_addr
		 - JMP label
		*/
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jmp(Os::CodeBlock& block) {
			return template_Jmp<SIZE>(block);
		}

		/* Jump in overflow (OF == 1) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jo(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x70);
		}

		/* Jump in no overflow (OF == 0) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jno(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x71);
		}

		/* Jump if unsigned lower (CF == 1) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jb(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x72);
		}

		/* Jump if carry (CF == 1) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jc(Os::CodeBlock& block) {
			return Jb<SIZE>(block);
		}

		/* Jump if unsigned not greater-equal (CF == 1) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jnae(Os::CodeBlock& block) {
			return Jb<SIZE>(block);
		}

		/* Jump if unsigned greater-equal (CF == 0) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jae(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x73);
		}

		/* Jump if no carry (CF == 0) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jnc(Os::CodeBlock& block) {
			return Jae<SIZE>(block);
		}

		/* Jump if unsigned not lower (CF == 0) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jnb(Os::CodeBlock& block) {
			return Jae<SIZE>(block);
		}

		/* Jump if equal (ZF == 1) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Je(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x74);
		}

		/* Jump if zero (ZF == 1) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jz(Os::CodeBlock& block) {
			return Je<SIZE>(block);
		}

		/* Jump if not equal (ZF == 0) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jne(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x75);
		}

		/* Jump if not zero (ZF == 0) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jnz(Os::CodeBlock& block) {
			return Jne<SIZE>(block);
		}

		/* Jump if unsigned lower-equal (CF == 1 || ZF == 1) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jbe(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x76);
		}

		/* Jump if unsigned not greater (CF == 1 || ZF == 1) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jna(Os::CodeBlock& block) {
			return Jbe<SIZE>(block);
		}

		/* Jump if unsigned greater (CF == 0 && ZF == 0) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Ja(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x77);
		}

		/* Jump if unsigned not lower-equal (CF == 0 && ZF == 0) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jnbe(Os::CodeBlock& block) {
			return Ja<SIZE>(block);
		}

		/* Jump if sign (SF == 1) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Js(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x78);
		}

		/* Jump if no sign (SF == 0) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jns(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x79);
		}

		/* Jump if parity (PF == 1) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jp(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x7A);
		}

		/* Jump if parity (PF == 1) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jpe(Os::CodeBlock& block) {
			return Jp<SIZE>(block);
		}

		/* Jump if no parity (PF == 0) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jnp(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x7B);
		}

		/* Jump if no parity (PF == 0) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jpo(Os::CodeBlock& block) {
			return Jnp<SIZE>(block);
		}

		/* Jump if signed lower (SF != OF) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jl(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x7C);
		}

		/* Jump if signed not greater-equal (SF != OF) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jnge(Os::CodeBlock& block) {
			return Jl<SIZE>(block);
		}

		/* Jump if signed greater-equal (ZF == 0 && SF == OF) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jge(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x7D);
		}

		/* Jump if signed not lower (ZF == 0 && SF == OF) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jnl(Os::CodeBlock& block) {
			return Jge<SIZE>(block);
		}

		/* Jump if signed lower-equal (ZF == 1 && SF != OF) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jle(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x7E);
		}

		/* Jump if signed not greater (ZF == 1 && SF != OF) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jng(Os::CodeBlock& block) {
			return Jle<SIZE>(block);
		}

		/* Jump if signed greater (ZF == 0 && SF == OF) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jg(Os::CodeBlock& block) {
			return template_Jxx<SIZE>(block, 0x7F);
		}

		/* Jump if signed not lower-equal (ZF == 0 && SF == OF) */
		template<JmpSize SIZE = SHORT>
		static FwdLabel<SIZE> Jnle(Os::CodeBlock& block) {
			return Jg<SIZE>(block);
		}

		/* Launch interrupt
		 - INT imm
		*/
		static ReplaceableValue<U8::type> Int(Os::CodeBlock& block, U8 number) {
			common::write_Opcode(block, 0xCD);
			Offset offset = block.getOffset();
			common::write_Immediate(block, number);
			return ReplaceableValue<U8::type>(offset);
		}

		/* Launch interrupt number 3 */
		static void Int3(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xCC);
		}

		/* Launch interrupt number 4 if overflow flag (OF) is set */
		static void IntO(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xCE);
		}

		/* Return from interrupt */
		static void Iret(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xCF);
		}

		/* Create enter procedure stack frame */
		static auto Enter(Os::CodeBlock& block, U16 size, U8 level) {
			common::write_Opcode(block, 0xC8);
			Offset offset = block.getOffset();
			common::write_Immediate(block, size);
			common::write_Immediate(block, level);
			return std::make_pair(ReplaceableValue<U16::type>(offset),
				ReplaceableValue<U8::type>(offset + sizeof(U16::type)));
		}

		/* Create leave procedure stack frame */
		static void Leave(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xC9);
		}

		/* Return from near procedure */
		static void Ret(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xC3);
		}

		/* Return from near procedure */
		static ReplaceableValue<U16::type> Ret(Os::CodeBlock& block, U16 number) {
			common::write_Opcode(block, 0xC2);
			Offset offset = block.getOffset();
			common::write_Immediate(block, number);
			return ReplaceableValue<U16::type>(offset);
		}

		/* Return from near procedure */
		static void Retn(Os::CodeBlock& block) {
			Ret(block);
		}

		/* Return from near procedure */
		static auto Retn(Os::CodeBlock& block, U16 number) {
			return Ret(block, number);
		}

		/* Return from far procedure */
		static void Retf(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xCB);
		}

		/* Return from far procedure */
		static ReplaceableValue<U16::type> Retf(Os::CodeBlock& block, U16 number) {
			common::write_Opcode(block, 0xCA);
			Offset offset = block.getOffset();
			common::write_Immediate(block, number);
			return ReplaceableValue<U16::type>(offset);
		}

		/* Jump if ECX == 0 */
		static FwdLabel<SHORT> Jecxz(Os::CodeBlock& block) {
			return template_Loop<SHORT>(block, 0xE3);
		}

		/* Jump if CX == 0 */
		static FwdLabel<SHORT> Jcxz(Os::CodeBlock& block) {
			block.pushRaw<uint8_t>(0x67);
			return Jecxz(block);
		}

		/* Decrement ECX and jump if ECX != 0 */
		static FwdLabel<SHORT> Loop(Os::CodeBlock& block) {
			return template_Loop<SHORT>(block, 0xE2);
		}

		/* Decrement ECX and jump if ECX != 0 and ZF == 1 */
		static FwdLabel<SHORT> Loope(Os::CodeBlock& block) {
			return template_Loop<SHORT>(block, 0xE1);
		}

		/* Decrement ECX and jump if ECX != 0 and ZF == 1 */
		static FwdLabel<SHORT> Loopz(Os::CodeBlock& block) {
			return Loope(block);
		}

		/* Decrement ECX and jump if ECX != 0 and ZF == 0 */
		static FwdLabel<SHORT> Loopne(Os::CodeBlock& block) {
			return template_Loop<SHORT>(block, 0xE0);
		}

		/* Decrement ECX and jump if ECX != 0 and ZF == 0 */
		static FwdLabel<SHORT> Loopnz(Os::CodeBlock& block) {
			return Loopne(block);
		}
#pragma endregion

#pragma region Flags manipulations [DONE]
		/* Reset carry flag (CF) */
		static void Clc(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xF8);
		}

		/* Reset direction flag (DF) */
		static void Cld(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xFC);
		}

		/* Clear interrupt flag (IF) */
		static void Cli(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xFA);
		}

		/* Invert carry flag (CF) */
		static void Cmc(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xF5);
		}

		/* Load flags to register AH */
		static void Lahf(Os::CodeBlock& block) {
			common::write_Opcode(block, 0x9F);
		}

		/* Save FLAGS to stack */
		static void Pushf(Os::CodeBlock& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x9C);
		}

		/* Save EFLAGS to stack */
		static void Pushfd(Os::CodeBlock& block) {
			common::write_Opcode(block, 0x9C);
		}

		/* Load FLAGS from stack */
		static void Popf(Os::CodeBlock& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x9D);
		}

		/* Load EFLAGS from stack */
		static void Popfd(Os::CodeBlock& block) {
			common::write_Opcode(block, 0x9D);
		}

		/* Store register AH to flags */
		static void Sahf(Os::CodeBlock& block) {
			common::write_Opcode(block, 0x9E);
		}

		/* Set all register AL bits according CF flag 
		 - Remark: Not documented Intel instruction
		*/
		static void Salc(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xD6);
		}

		/* Set carry flag (CF) */
		static void Stc(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xF9);
		}

		/* Set direction flag (DF) */
		static void Std(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xFD);
		}

		/* Set interrupt flag (IF) */
		static void Sti(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xFB);
		}
#pragma endregion

#pragma region String operations
		/* TODO: Lodsb */
		void Lodsb(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xAC);
		}

		/* TODO: Lodsw */
		void Lodsw(Os::CodeBlock& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0xAD);
		}

		/* TODO: Movsb */
		void Movsb(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xA4);
		}

		/* TODO: Movsw */
		void Movsw(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xA5);
		}

		void Scasb(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xAE);
		}

		void Scasw(Os::CodeBlock& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0xAF);
		}

		void Stosb(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xAA);
		}

		static void Stosw(Os::CodeBlock& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0xAB);
		}
#pragma endregion

#pragma region Other operations
		/* TODO: Lock processor memory bus */
		static void Lock(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xF0);
		}

		/* No operation */
		static void Nop(Os::CodeBlock& block) {
			common::write_Opcode(block, 0x90);
		}

		/* FPU wait */
		static void Fwait(Os::CodeBlock& block) {
			Wait(block);
		}

		/* FPU wait */
		static void Wait(Os::CodeBlock& block) {
			common::write_Opcode(block, 0x9B);
		}

		/* Halt processor */
		static void Hlt(Os::CodeBlock& block) {
			common::write_Opcode(block, 0xF4);
		}
#pragma endregion
	};
}