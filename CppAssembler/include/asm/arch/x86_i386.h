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
		
		template<class BLOCK>
		static void write_Operand_Segm_Prefix(BLOCK& block, RegSeg segm) {
			block.pushRaw<uint8_t>(segm);
		}

		template<MemSize SIZE, class BLOCK>
		struct OpcodeWriter;

		template<class BLOCK>
		struct OpcodeWriter<BYTE_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode(block, opcode);
			}

			static void write(BLOCK& block, common::Opcode opcode, OpMode rmMode) {
				common::write_Opcode(block, opcode | (rmMode << 1));
			}

			static void writeOnlyExtendedPrefixs(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode);
			}

			static void writeExtended(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode);
			}
		};

		template<class BLOCK>
		struct OpcodeWriter<WORD_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_16bit_Prefix(block);
				common::write_Opcode(block, opcode | 0b1);
			}

			static void write(BLOCK& block, common::Opcode opcode, OpMode rmMode) {
				common::write_Opcode_16bit_Prefix(block);
				common::write_Opcode(block, opcode | (rmMode << 1) | 0b1);
			}

			static void writeOnlyExtendedPrefixs(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_16bit_Prefix(block);
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode);
			}

			static void writeExtended(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_16bit_Prefix(block);
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode | 0b1);
			}
		};

		template<class BLOCK>
		struct OpcodeWriter<DWORD_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode(block, opcode | 0b1);
			}

			static void write(BLOCK& block, common::Opcode opcode, OpMode rmMode) {
				common::write_Opcode(block, opcode | (rmMode << 1) | 0b1);
			}

			static void writeOnlyExtendedPrefixs(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode);
			}

			static void writeExtended(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode | 0b1);
			}
		};

		template<MemSize SIZE, class BLOCK>
		static void write_Opcode(BLOCK& block, common::Opcode opcode) {
			OpcodeWriter<SIZE, BLOCK>::write(block, opcode);
		}

		template<MemSize SIZE, class BLOCK>
		static void write_Opcode(BLOCK& block, common::Opcode opcode, OpMode rmMode) {
			OpcodeWriter<SIZE, BLOCK>::write(block, opcode, rmMode);
		}

		template<class BLOCK>
		static void write_Opcode_Only_RM_Mode(BLOCK& block, common::Opcode opcode, OpMode rmMode) {
			common::write_Opcode(block, opcode | (rmMode << 1));
		}

		template<MemSize SIZE, class BLOCK>
		static void write_Opcode_Only_Extended_Prefixs(BLOCK& block, common::Opcode opcode) {
			OpcodeWriter<SIZE, BLOCK>::writeOnlyExtendedPrefixs(block, opcode);
		}

		template<MemSize SIZE, class BLOCK>
		static void write_Opcode_Extended(BLOCK& block, common::Opcode opcode) {
			OpcodeWriter<SIZE, BLOCK>::writeExtended(block, opcode);
		}

		template<MemSize SIZE, MemSize OPT_SIZE, class BLOCK>
		struct OpcodeImmOptimizedWriter;

		template<class BLOCK>
		struct OpcodeImmOptimizedWriter<BYTE_PTR, BYTE_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode(block, opcode);
			}
		};

		template<class BLOCK>
		struct OpcodeImmOptimizedWriter<WORD_PTR, WORD_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_16bit_Prefix(block);
				common::write_Opcode(block, opcode | 0b1);
			}
		};

		template<class BLOCK>
		struct OpcodeImmOptimizedWriter<WORD_PTR, BYTE_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_16bit_Prefix(block);
				common::write_Opcode(block, opcode | 0b11);
			}
		};

		template<class BLOCK>
		struct OpcodeImmOptimizedWriter<DWORD_PTR, DWORD_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode(block, opcode | 0b1);
			}
		};

		template<class BLOCK>
		struct OpcodeImmOptimizedWriter<DWORD_PTR, WORD_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode(block, opcode | 0b1);
			}
		};

		template<class BLOCK>
		struct OpcodeImmOptimizedWriter<DWORD_PTR, BYTE_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode(block, opcode | 0b11);
			}
		};

		template<MemSize SIZE, MemSize OPT_SIZE, class BLOCK>
		static void write_Opcode_Imm_Optimized(BLOCK& block, common::Opcode opcode) {
			OpcodeImmOptimizedWriter<SIZE, OPT_SIZE, BLOCK>::write(block, opcode);
		}

		template<MemSize SIZE, class T, class BLOCK>
		static void write_Imm_Size_Extend(BLOCK& block, const Imm<T>& imm) {
			block.pushRaw(static_cast<detail::ImmSizeExtend<SIZE, T>::type>(imm));
		}

		template<MemSize SIZE, class T, class BLOCK>
		static void write_Imm_Size_Optimize(BLOCK& block, const Imm<T>& imm) {
			block.pushRaw(static_cast<detail::ImmSizeOptimize<SIZE, T>::type>(imm));
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> template_1mem_operand(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcode.getOpcode());
			return mem.write(block, opcode.getMode());
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> template_1mem_ext_operand(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<SIZE>(block, opcode.getOpcode());
			return mem.write(block, opcode.getMode());
		}

		template<class REG, class BLOCK>
		struct Instruction1RegWriter;

		template<class BLOCK>
		struct Instruction1RegWriter<Reg8, BLOCK> {
			static ReplaceableReg<Reg8> write(BLOCK& block, const detail::OpcodeLarge& opcode, Reg8 reg) {
				write_Opcode<BYTE_PTR>(block, opcode.getOpcode());
				Offset offset = block.getOffset();
				common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
				return ReplaceableReg<Reg8>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
			}

			static ReplaceableReg<Reg8> writeExtended(BLOCK& block, const detail::OpcodeLarge& opcode, Reg8 reg) {
				write_Opcode_Only_Extended_Prefixs<BYTE_PTR>(block, opcode.getOpcode());
				Offset offset = block.getOffset();
				common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
				return ReplaceableReg<Reg8>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
			}
		};

		template<class BLOCK>
		struct Instruction1RegWriter<Reg16, BLOCK> {
			static ReplaceableReg<Reg16> write(BLOCK& block, const detail::OpcodeLarge& opcode, Reg16 reg) {
				write_Opcode<WORD_PTR>(block, opcode.getOpcode());
				Offset offset = block.getOffset();
				common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
				return ReplaceableReg<Reg16>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
			}

			static ReplaceableReg<Reg16> writeExtended(BLOCK& block, const detail::OpcodeLarge& opcode, Reg16 reg) {
				write_Opcode_Only_Extended_Prefixs<WORD_PTR>(block, opcode.getOpcode());
				Offset offset = block.getOffset();
				common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
				return ReplaceableReg<Reg16>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
			}
		};

		template<class BLOCK>
		struct Instruction1RegWriter<Reg32, BLOCK> {
			static ReplaceableReg<Reg32> write(BLOCK& block, const detail::OpcodeLarge& opcode, Reg32 reg) {
				write_Opcode<DWORD_PTR>(block, opcode.getOpcode());
				Offset offset = block.getOffset();
				common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
				return ReplaceableReg<Reg32>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
			}

			static ReplaceableReg<Reg32> writeExtended(BLOCK& block, const detail::OpcodeLarge& opcode, Reg32 reg) {
				write_Opcode_Only_Extended_Prefixs<DWORD_PTR>(block, opcode.getOpcode());
				Offset offset = block.getOffset();
				common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
				return ReplaceableReg<Reg32>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
			}
		};

		template<class REG, class BLOCK>
		static ReplaceableReg<REG> template_1reg_operand(BLOCK& block, const detail::OpcodeLarge& opcode, REG reg) {
			return Instruction1RegWriter<REG, BLOCK>::write(block, opcode, reg);
		}

		template<class REG, class BLOCK>
		static ReplaceableReg<REG> template_1reg_ext_operand(BLOCK& block, const detail::OpcodeLarge& opcode, REG reg) {
			return Instruction1RegWriter<REG, BLOCK>::writeExtended(block, opcode, reg);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableExtendedImm<SIZE, T>> template_mem_imm_operands(BLOCK& block, const detail::OpcodeSet& opcodeSet, const Mem32<MODE>& mem, const Imm<T>& imm) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcodeSet.getSecond().getOpcode());
			auto replaceMem = mem.write(block, opcodeSet.getSecond().getMode());
			Offset offset = block.getOffset();
			write_Imm_Size_Extend<SIZE>(block, imm);
			return std::make_pair(replaceMem, ReplaceableExtendedImm<SIZE, T>(offset));
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableOptimizedImm<SIZE, T>> template_mem_imm_opt_operands(BLOCK& block, const detail::OpcodeSet& opcodeSet, const Mem32<MODE>& mem, const Imm<T>& imm) {
			mem.writeSegmPrefix(block);
			write_Opcode_Imm_Optimized<SIZE, TypeMemSize<Imm<T>>::value>(block, opcodeSet.getSecond().getOpcode());
			auto replaceMem = mem.write(block, opcodeSet.getSecond().getMode());
			Offset offset = block.getOffset();
			write_Imm_Size_Optimize<SIZE>(block, imm);
			return std::make_pair(replaceMem, ReplaceableOptimizedImm<SIZE, T>(offset));
		}

		template<class REG, class T, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableOptimizedImm<TypeMemSize<REG>::value, T>> template_reg_imm_opt_operands(BLOCK& block, const detail::OpcodeSet& opcodeSet, const REG& reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "First param must be register");
			write_Opcode_Imm_Optimized<TypeMemSize<REG>::value, TypeMemSize<Imm<T>>::value>(block, opcodeSet.getSecond().getOpcode());
			Offset modOffset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcodeSet.getSecond().getMode(), reg);
			Offset offset = block.getOffset();
			write_Imm_Size_Optimize<TypeMemSize<REG>::value>(block, imm);
			return std::make_pair(ReplaceableReg<REG>(modOffset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableOptimizedImm<TypeMemSize<REG>::value, T>(offset));
		}

		template<class REG, class T, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableExtendedImm<TypeMemSize<REG>::value, T>> template_reg_imm_operands(BLOCK& block, const detail::OpcodeSet& opcodeSet, const REG& reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "First param must be register");
			write_Opcode<TypeMemSize<REG>::value>(block, opcodeSet.getSecond().getOpcode());
			Offset regOffset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcodeSet.getSecond().getMode(), reg);
			Offset offset = block.getOffset();
			write_Imm_Size_Extend<TypeMemSize<REG>::value>(block, imm);
			return std::make_pair(ReplaceableReg<REG>(regOffset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableExtendedImm<TypeMemSize<REG>::value, T>(offset));
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> template_shift_operands(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcode.getOpcode() | 0x10);
			return mem.write(block, opcode.getMode());
		}

		template<class REG, class BLOCK>
		static ReplaceableReg<REG> template_shift_operands(BLOCK& block, const detail::OpcodeLarge& opcode, const REG& reg) {
			write_Opcode<TypeMemSize<REG>::value>(block, opcode.getOpcode() | 0x10);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			return ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> template_shift_operands(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			static_assert(REG == CL, "Invalid register for shift operation");
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcode.getOpcode() | 0x12);
			return mem.write(block, opcode.getMode());
		}

		template<class REG, Reg8 SH_REG, class BLOCK>
		static ReplaceableReg<REG> template_shift_operands(BLOCK& block, const detail::OpcodeLarge& opcode, const REG& reg) {
			static_assert(SH_REG == CL, "Invalid register for shift operation");
			write_Opcode<TypeMemSize<REG>::value>(block, opcode.getOpcode() | 0x12);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			return ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableValue<U8::type>> template_shift_operands(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem, const U8& imm) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcode.getOpcode());
			auto replaceMem = mem.write(block, opcode.getMode());
			Offset offset = block.getOffset();
			common::write_Immediate(block, imm);
			return std::make_pair(replaceMem, ReplaceableValue<U8::type>(offset));
		}

		template<class REG, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableValue<U8::type>> template_shift_operands(BLOCK& block, const detail::OpcodeLarge& opcode, const REG& reg, const U8& imm) {
			write_Opcode<TypeMemSize<REG>::value>(block, opcode.getOpcode());
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			common::write_Immediate(block, imm);
			return std::make_pair(ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableValue<U8::type>(offset + 1));
		}

		template<MemSize SIZE, Reg8 COUNT_REG = CL, AddressMode MODE, class REG, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableReg<REG>> template_prshift_operands(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& mem, REG reg) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			static_assert(COUNT_REG == CL, "Invalid register for precision shift operation");
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Invalid precision shift size prefix");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<SIZE>(block, opcode | 0x01);
			auto replaceMem = mem.write(block, reg);
			return std::make_pair(replaceMem, replaceMem.getOtherReg<REG>());
		}

		template<Reg8 COUNT_REG = CL, class REG, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableReg<REG>> template_prshift_operands(BLOCK& block, common::Opcode opcode, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			static_assert(COUNT_REG == CL, "Invalid register for precision shift operation");
			static_assert(TypeMemSize<REG>::value == WORD_PTR || TypeMemSize<REG>::value == DWORD_PTR, "Invalid precision shift size prefix");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode | 0x01);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg2, reg1);
			return std::make_pair(ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableReg<REG>(offset, common::MOD_REG_RM::REG_BIT_OFFSET));
		}

		template<MemSize SIZE, AddressMode MODE, class REG, class BLOCK>
		static std::tuple<ReplaceableMem32<MODE>, ReplaceableReg<REG>, ReplaceableValue<U8::type>> template_prshift_operands(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& mem, REG reg, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Invalid precision shift size prefix");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<SIZE>(block, opcode);
			auto replaceMem = mem.write(block, reg);
			Offset offset = block.getOffset();
			common::write_Immediate(block, imm);
			return std::make_tuple(replaceMem, replaceMem.getOtherReg<REG>(), ReplaceableValue<U8::type>(offset));
		}

		template<class REG, class BLOCK>
		static std::tuple<ReplaceableReg<REG>, ReplaceableReg<REG>, ReplaceableValue<U8::type>> template_prshift_operands(BLOCK& block, common::Opcode opcode, REG reg1, REG reg2, const U8& imm) {
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

		template<class BLOCK>
		static std::pair<ReplaceableReg<Reg8>, ReplaceableReg<Reg8>> template_2operands(BLOCK& block, common::Opcode opcode, const Reg8& dst, const Reg8& src) {
			write_Opcode<BYTE_PTR>(block, opcode, MODE_RR);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
			return std::make_pair(ReplaceableReg<Reg8>(offset, common::MOD_REG_RM::REG_BIT_OFFSET),
				ReplaceableReg<Reg8>(offset, common::MOD_REG_RM::RM_BIT_OFFSET));
		}

		template<AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableReg<Reg8>, ReplaceableMem32<MODE>> template_2operands(BLOCK& block, common::Opcode opcode, const Reg8& dst, const Mem32<MODE>& src) {
			src.writeSegmPrefix(block);
			write_Opcode<BYTE_PTR>(block, opcode, MODE_RM);
			auto replaceMem = src.write(block, dst);
			return std::make_pair(replaceMem.getOtherReg<Reg8>(), replaceMem);
		}

		template<AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableReg<Reg8>> template_2operands(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg8& src) {
			dst.writeSegmPrefix(block);
			write_Opcode<BYTE_PTR>(block, opcode, MODE_MR);
			auto replaceMem = dst.write(block, src);
			return std::make_pair(replaceMem, replaceMem.getOtherReg<Reg8>());
		}

		template<class BLOCK>
		static std::pair<ReplaceableReg<Reg16>, ReplaceableReg<Reg16>> template_2operands(BLOCK& block, common::Opcode opcode, const Reg16& dst, const Reg16& src) {
			write_Opcode<WORD_PTR>(block, opcode, MODE_RR);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
			return std::make_pair(ReplaceableReg<Reg16>(offset, common::MOD_REG_RM::REG_BIT_OFFSET),
				ReplaceableReg<Reg16>(offset, common::MOD_REG_RM::RM_BIT_OFFSET));
		}

		template<AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableReg<Reg16>, ReplaceableMem32<MODE>> template_2operands(BLOCK& block, common::Opcode opcode, const Reg16& dst, const Mem32<MODE>& src) {
			src.writeSegmPrefix(block);
			write_Opcode<WORD_PTR>(block, opcode, MODE_RM);
			auto replaceMem = src.write(block, dst);
			return std::make_pair(replaceMem.getOtherReg<Reg16>(), replaceMem);
		}

		template<AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableReg<Reg16>> template_2operands(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg16& src) {
			dst.writeSegmPrefix(block);
			write_Opcode<WORD_PTR>(block, opcode, MODE_MR);
			auto replaceMem = dst.write(block, src);
			return std::make_pair(replaceMem, replaceMem.getOtherReg<Reg16>());
		}

		template<class BLOCK>
		static std::pair<ReplaceableReg<Reg32>, ReplaceableReg<Reg32>> template_2operands(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Reg32& src) {
			write_Opcode<DWORD_PTR>(block, opcode, MODE_RR);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
			return std::make_pair(ReplaceableReg<Reg32>(offset, common::MOD_REG_RM::REG_BIT_OFFSET),
				ReplaceableReg<Reg32>(offset, common::MOD_REG_RM::RM_BIT_OFFSET));
		}

		template<AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableReg<Reg32>, ReplaceableMem32<MODE>> template_2operands(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Mem32<MODE>& src) {
			src.writeSegmPrefix(block);
			write_Opcode<DWORD_PTR>(block, opcode, MODE_RM);
			auto replaceMem = src.write(block, dst);
			return std::make_pair(replaceMem.getOtherReg<Reg32>(), replaceMem);
		}

		template<AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableReg<Reg32>> template_2operands(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg32& src) {
			dst.writeSegmPrefix(block);
			write_Opcode<DWORD_PTR>(block, opcode, MODE_MR);
			auto replaceMem = dst.write(block, src);
			return std::make_pair(replaceMem, replaceMem.getOtherReg<Reg32>());
		}

		static uint8_t getSegmentRegIndex(RegSeg sreg) {
			switch (sreg) {
			case ES:
				return 0;
			case CS:
				return 1;
			case SS:
				return 2;
			case DS:
				return 3;
			case FS:
				return 4;
			case GS:
				return 5;
			}
			return 0;
		}

		template<class BLOCK>
		static void template_2operands_segm(BLOCK& block, common::Opcode opcode, const Reg16& dst, const RegSeg& src) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, getSegmentRegIndex(src), dst);
		}

		template<class BLOCK>
		static void template_2operands_segm(BLOCK& block, common::Opcode opcode, const RegSeg& dst, const Reg16& src) {
			write_Opcode_Only_RM_Mode(block, opcode, MODE_RR);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, getSegmentRegIndex(dst), src);
		}

		template<AddressMode MODE, class BLOCK>
		static void template_2operands_segm(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const RegSeg& src) {
			dst.writeSegmPrefix(block);
			write_Opcode_Only_RM_Mode(block, opcode, MODE_MR);
			dst.write(block, getSegmentRegIndex(src));
		}

		template<AddressMode MODE, class BLOCK>
		static void template_2operands_segm(BLOCK& block, common::Opcode opcode, const RegSeg& dst, const Mem32<MODE>& src) {
			src.writeSegmPrefix(block);
			write_Opcode_Only_RM_Mode(block, opcode, MODE_RM);
			src.write(block, getSegmentRegIndex(dst));
		}

		template<class DST, class SRC, class BLOCK>
		static auto template_2operands_opt_imm(BLOCK& block, const detail::OpcodeSet& opcodeSet, const DST& dst, const SRC& src) {
			return template_2operands(block, opcodeSet.getMain(), dst, src);
		}

		template<class T, class BLOCK>
		static auto template_2operands_opt_imm(BLOCK& block, const detail::OpcodeSet& opcodeSet, const Reg32& dst, const Imm<T>& src) {
			return template_reg_imm_opt_operands(block, opcodeSet, dst, src);
		}

		template<class T, class BLOCK>
		static auto template_2operands_opt_imm(BLOCK& block, const detail::OpcodeSet& opcodeSet, const Reg16& dst, const Imm<T>& src) {
			return template_reg_imm_opt_operands(block, opcodeSet, dst, src);
		}

		template<class T, class BLOCK>
		static auto template_2operands_opt_imm(BLOCK& block, const detail::OpcodeSet& opcodeSet, const Reg8& dst, const Imm<T>& src) {
			return template_reg_imm_opt_operands(block, opcodeSet, dst, src);
		}

#ifdef X86_SIZE_OPTIMIZED
		template<class REG, class T, class BLOCK>
		static bool template_2operands_first_reg_imm(BLOCK& block, common::Opcode opcode, const REG& reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			if ((reg == 0) && (TypeMemSize<REG>::value == TypeMemSize<Imm<T>>::value)) {
				write_Opcode<TypeMemSize<REG>::value>(block, opcode);
				write_Imm_Size_Extend<TypeMemSize<REG>::value>(block, imm);
				return true;
			}
			return false;
		}
#endif
		template<AddressMode MODE, class BLOCK>
		static auto template_2operands_symetric(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg8& src) {
			return template_2operands(block, opcode, dst, src);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_2operands_symetric(BLOCK& block, common::Opcode opcode, const Reg8& dst, const Mem32<MODE>& src) {
			return template_2operands(block, opcode, src, dst);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_2operands_symetric(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg16& src) {
			return template_2operands(block, opcode, dst, src);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_2operands_symetric(BLOCK& block, common::Opcode opcode, const Reg16& dst, const Mem32<MODE>& src) {
			return template_2operands(block, opcode, src, dst);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_2operands_symetric(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg32& src) {
			return template_2operands(block, opcode, dst, src);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_2operands_symetric(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Mem32<MODE>& src) {
			return template_2operands(block, opcode, src, dst);
		}

		template<class DST, class SRC, class BLOCK>
		static void template_2operands_xchange(BLOCK& block, common::Opcode opcode, const DST& dst, const SRC& src) {
			static_assert(TypeMemSize<DST>::value == TypeMemSize<SRC>::value, "Xchange operand size mismatch");
			write_Opcode_Extended<TypeMemSize<DST>::value>(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, src, dst);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void template_2operands_xchange(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& mem, const REG& reg) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			mem.writeSegmPrefix(block);
			write_Opcode_Extended<TypeMemSize<REG>::value>(block, opcode);
			mem.write(block, reg);
		}

		template<class BLOCK>
		static std::pair<ReplaceableReg<Reg32>, ReplaceableReg<Reg16>> template_2operands_extend_val(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Reg16& src) {
			write_Opcode_Extended<TypeMemSize<Reg32>::value>(block, opcode);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
			return std::make_pair(ReplaceableReg<Reg32>(offset, common::MOD_REG_RM::REG_BIT_OFFSET),
				ReplaceableReg<Reg16>(offset, common::MOD_REG_RM::RM_BIT_OFFSET));
		}

		template<class BLOCK>
		static std::pair<ReplaceableReg<Reg16>, ReplaceableReg<Reg8>> template_2operands_extend_val(BLOCK& block, common::Opcode opcode, const Reg16& dst, const Reg8& src) {
			write_Opcode_Extended<TypeMemSize<Reg16>::value>(block, opcode);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
			return std::make_pair(ReplaceableReg<Reg16>(offset, common::MOD_REG_RM::REG_BIT_OFFSET),
				ReplaceableReg<Reg8>(offset, common::MOD_REG_RM::RM_BIT_OFFSET));
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableMem32<MODE>> template_2operands_extend_val(BLOCK& block, common::Opcode opcode, const REG& reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			mem.writeSegmPrefix(block);
			write_Opcode_Extended<TypeMemSize<REG>::value>(block, opcode);
			auto replaceMem = mem.write(block, reg);
			return std::make_pair(replaceMem.getOtherReg<REG>(), replaceMem);
		}

		template<class BLOCK>
		static void template_2_operands_ext(BLOCK& block, common::Opcode opcode, Reg16 reg1, Reg16 reg2) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg1, reg2);
		}

		template<AddressMode MODE, class BLOCK>
		static void template_2_operands_ext(BLOCK& block, common::Opcode opcode, Reg16 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			mem.write(block, reg);
		}

		template<class BLOCK>
		static void template_2_operands_ext(BLOCK& block, common::Opcode opcode, Reg32 reg1, Reg32 reg2) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg1, reg2);
		}

		template<AddressMode MODE, class BLOCK>
		static void template_2_operands_ext(BLOCK& block, common::Opcode opcode, Reg32 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			mem.write(block, reg);
		}

		template<class REG, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableReg<REG>> template_bit_scan(BLOCK& block, common::Opcode opcode, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "Invalid size of operand");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg1, reg2);
			return std::make_pair(ReplaceableReg<REG>(offset, common::MOD_REG_RM::REG_BIT_OFFSET),
				ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET));
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableMem32<MODE>> template_bit_scan(BLOCK& block, common::Opcode opcode, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "Invalid size of operand");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode);
			auto replaceMem = mem.write(block, reg);
			return std::make_pair(replaceMem.getOtherReg<REG>(), replaceMem);
		}

		template<class REG, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableReg<REG>> template_bit_operation(BLOCK& block, const detail::OpcodeSet& opcode, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "Invalid size of operand");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode.getMain());
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg2, reg1);
			return std::make_pair(ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableReg<REG>(offset, common::MOD_REG_RM::REG_BIT_OFFSET));
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableReg<REG>> template_bit_operation(BLOCK& block, const detail::OpcodeSet& opcode, const Mem32<MODE>& mem, REG reg) {
			static_assert(IsRegType<REG>::value, "Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "Invalid size of operand");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode.getMain());
			auto replaceMem = mem.write(block, reg);
			return std::make_pair(replaceMem, replaceMem.getOtherReg<REG>());
		}

		template<class REG, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableValue<U8::type>> template_bit_operation(BLOCK& block, const detail::OpcodeSet& opcode, REG reg, U8 imm) {
			static_assert(IsRegType<REG>::value, "Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "Invalid size of operand");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode.getSecond().getOpcode());
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getSecond().getMode(), reg);
			common::write_Immediate(block, imm);
			return std::make_pair(ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableValue<U8::type>(offset + 1));
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableValue<U8::type>> template_bit_operation(BLOCK& block, const detail::OpcodeSet& opcode, const Mem32<MODE>& mem, U8 imm) {
			static_assert(SIZE != BYTE_PTR, "Invalid size of operand");
			write_Opcode_Only_Extended_Prefixs<SIZE>(block, opcode.getSecond().getOpcode());
			auto replaceMem = mem.write(block, opcode.getSecond().getMode());
			Offset offset = block.getOffset();
			common::write_Immediate(block, imm);
			return std::make_pair(replaceMem, ReplaceableValue<U8::type>(offset));
		}

		template<class BLOCK>
		static ReplaceableReg<Reg8> template_Setcc(BLOCK& block, common::Opcode opcode, const Reg8& reg) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, 0, reg);
			return ReplaceableReg<Reg8>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
		}

		template<MemSize SIZE = BYTE_PTR, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> template_Setcc(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& mem) {
			static_assert(SIZE == BYTE_PTR, "Setcc: Invalid size of operand");
			mem.writeSegmPrefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			return mem.write(block, 0);
		}

		template<JmpSize SIZE, class BLOCK>
		struct JumpWriter;

		template<class BLOCK>
		struct JumpWriter<SHORT, BLOCK> {
			static FwdLabel<SHORT> write(BLOCK& block) {
				common::write_Opcode(block, 0xEB);
				Offset offset = block.getOffset();
				block.skipBytes(FwdLabel<SHORT>::offset_size);
				return FwdLabel<SHORT>(offset);
			}

			static FwdLabel<SHORT> writeConditional(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode(block, opcode);
				Offset offset = block.getOffset();
				block.skipBytes(FwdLabel<SHORT>::offset_size);
				return FwdLabel<SHORT>(offset);
			}
		};

		template<class BLOCK>
		struct JumpWriter<LONG, BLOCK> {
			static FwdLabel<LONG> write(BLOCK& block) {
				common::write_Opcode(block, 0xE9);
				Offset offset = block.getOffset();
				block.skipBytes(FwdLabel<LONG>::offset_size);
				return FwdLabel<LONG>(offset);
			}

			static FwdLabel<LONG> writeConditional(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode & 0x0F | 0x80);
				Offset offset = block.getOffset();
				block.skipBytes(FwdLabel<LONG>::offset_size);
				return FwdLabel<LONG>(offset);
			}
		};

		template<JmpSize SIZE, class BLOCK>
		static FwdLabel<SIZE> template_Jmp(BLOCK& block) {
			return JumpWriter<SIZE, BLOCK>::write(block, opcode);
		}

		template<JmpSize SIZE, class BLOCK>
		static FwdLabel<SIZE> template_Jxx(BLOCK& block, common::Opcode opcode) {
			return JumpWriter<SIZE, BLOCK>::writeConditional(block, opcode);
		}

		template<AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> template_Jmp(BLOCK& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<DWORD_PTR>(block, detail::opcode_JMP, mem);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg32> template_Jmp(BLOCK& block, const Reg32& reg) {
			return template_1reg_operand(block, detail::opcode_JMP, reg);
		}

		template<class BLOCK>
		static void template_Call(BLOCK& block, const Addr& jumpAddress) {
			common::write_Opcode(block, 0xE8);
			block.pushRaw(common::calc_Jump_Offset(block.getCurrentPtr(), jumpAddress, sizeof(Addr)));
		}

		template<AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> template_Call(BLOCK& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<DWORD_PTR>(block, detail::opcode_CALL, mem);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg32> template_Call(BLOCK& block, const Reg32& reg) {
			return template_1reg_operand(block, detail::opcode_CALL, reg);
		}

		template<JmpSize SIZE, class BLOCK>
		static FwdLabel<SIZE> template_Loop(BLOCK& block, common::Opcode opcode) {
			static_assert(SIZE == SHORT, "Loop: unsupported jump size");
			common::write_Opcode(block, opcode);
			Offset offset = block.getOffset();
			block.skipBytes(FwdLabel<SHORT>::offset_size);
			return FwdLabel<SHORT>(offset);
		}

		template<AddressMode MODE, class BLOCK>
		static void template_LoadSegReg(BLOCK& block, common::Opcode opcode, Reg32 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, opcode);
			mem.write(block, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static void template_LoadSegReg(BLOCK& block, common::Opcode opcode, Reg16 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, opcode);
			mem.write(block, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static void template_LoadSegRegEx(BLOCK& block, common::Opcode opcode, Reg32 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			mem.write(block, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static void template_LoadSegRegEx(BLOCK& block, common::Opcode opcode, Reg16 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			mem.write(block, reg);
		}
	public:
#pragma region Data transfer [DONE]
		/* Convert byte to word */
		template <class BLOCK>
		static void Cbw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x98);
		}

		/* Convert byte to word */
		template <class BLOCK>
		static void Cdq(BLOCK& block) {
			common::write_Opcode(block, 0x99);
		}

		/* Convert word to double word */
		template <class BLOCK>
		static void Cwd(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x99);
		}

		/* Convert word to double word */
		template <class BLOCK>
		static void Cwde(BLOCK& block) {
			common::write_Opcode(block, 0x98);
		}

		/* Calculate effective address
		 - LEA reg16, [mem]
		*/
		template<AddressMode MODE, class BLOCK>
		static auto Lea(BLOCK& block, Reg16 reg, const Mem32<MODE>& mem) {
			return template_2operands(block, 0x8D, mem, reg);
		}

		/* Calculate effective address
		 - LEA reg32, [mem]
		*/
		template<AddressMode MODE, class BLOCK>
		static auto Lea(BLOCK& block, Reg32 reg, const Mem32<MODE>& mem) {
			return template_2operands(block, 0x8D, mem, reg);
		}

		/* Moving data
		 - MOV reg,reg
		 - MOV reg,[mem]
		 - MOV [mem],reg
		*/
		template<class DST, class SRC, class BLOCK>
		static auto Mov(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands(block, detail::opcode_MOV.getMain(), dst, src);
		}

		/* Moving data
		 - MOV [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static auto Mov(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_operands<SIZE>(block, detail::opcode_MOV, mem, imm);
		}

		/* Moving data
		 - MOV [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static auto Mov(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Mov<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		/* Moving data
		 - MOV reg,imm
		*/
		template<class REG, class T, class BLOCK>
		static auto Mov(BLOCK& block, const REG& reg, const Imm<T>& imm) {
			return template_reg_imm_operands(block, detail::opcode_MOV, reg, imm);
		}
		
		/* Moving data
		 - MOV reg,sreg
		 - MOV [mem],sreg
		*/
		template<class DST, class BLOCK>
		static auto Mov(BLOCK& block, const DST& dst, RegSeg sreg) {
			return template_2operands_segm(block, 0x8c, dst, sreg);
		}

		/* Moving data
		 - MOV sreg,reg
		 - MOV sreg,[mem]
		*/
		template<class SRC, class BLOCK>
		static auto Mov(BLOCK& block, RegSeg sreg, const SRC& src) {
			return template_2operands_segm(block, 0x8c, sreg, src);
		}

		/* Mov with sign extend
		 - MOVSX reg,reg
		 - MOVSX reg,[mem]
		*/
		template<class DST, class SRC, class BLOCK>
		static auto Movsx(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_extend_val(block, 0xBE, dst, src);
		}

		/* Mov with zero extend
		 - MOVZX reg,reg
		 - MOVZX reg,[mem]
		*/
		template<class DST, class SRC, class BLOCK>
		static auto Movzx(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_extend_val(block, 0xB6, dst, src);
		}

		/* Push value to the stack
		 - PUSH reg16
		*/
		template<class BLOCK>
		static ReplaceableReg<Reg16> Push(BLOCK& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x50 | reg);
			return ReplaceableReg<Reg16>(offset, 0);
		}

		/* Push value to the stack
		 - PUSH reg32
		*/
		template<class BLOCK>
		static ReplaceableReg<Reg32> Push(BLOCK& block, Reg32 reg) {
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x50 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Push value to the stack
		 - PUSH imm8
		*/
		template<class BLOCK>
		static ReplaceableValue<S8> Push(BLOCK& block, S8 number) {
			common::write_Opcode(block, 0x6A);
			Offset offset = block.getOffset();
			common::write_Immediate(block, number);
			return ReplaceableValue<S8>(offset);
		}

		/* Push value to the stack
		 - PUSH imm16
		*/
		template<class BLOCK>
		static ReplaceableValue<S16> Push(BLOCK& block, S16 number) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x68);
			Offset offset = block.getOffset();
			common::write_Immediate(block, number);
			return ReplaceableValue<S16>(offset);
		}

		/* Push value to the stack
		 - PUSH imm32
		*/
		template<class BLOCK>
		static ReplaceableValue<S32> Push(BLOCK& block, S32 number) {
			common::write_Opcode(block, 0x68);
			Offset offset = block.getOffset();
			common::write_Immediate(block, number);
			return ReplaceableValue<S32>(offset);
		}

		/* Push value to the stack
		 - PUSH [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Push(BLOCK& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_PUSH, mem);
		}

		/* Push value to the stack
		 - PUSH sreg
		*/
		template<class BLOCK>
		static void Push(BLOCK& block, RegSeg sreg) {
			if (sreg == ES) {
				common::write_Opcode(block, 0x06);
			} else if (sreg == CS) {
				common::write_Opcode(block, 0x0E);
			} else if (sreg == SS) {
				common::write_Opcode(block, 0x16);
			} else if (sreg == DS) {
				common::write_Opcode(block, 0x1E);
			} else if (sreg == FS) {
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, 0xA0);
			} else if (sreg == GS) {
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, 0xA8);
			}
		}

		/* Store all 16 bit general registers to stack */
		template<class BLOCK>
		static void Pusha(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x60);
		}

		/* Store all 32 bit general registers to stack */
		template<class BLOCK>
		static void Pushad(BLOCK& block) {
			common::write_Opcode(block, 0x60);
		}

		/* Pop value from the stack
		 - POP reg16
		*/
		template<class BLOCK>
		static ReplaceableReg<Reg16> Pop(BLOCK& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x58 | reg);
			return ReplaceableReg<Reg16>(offset, 0);
		}

		/* Pop value from the stack
		 - POP reg32
		*/
		template<class BLOCK>
		static ReplaceableReg<Reg32> Pop(BLOCK& block, Reg32 reg) {
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x58 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Pop value from the stack
		 - POP [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Pop(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Invalid Pop size prefix");
			return template_1mem_operand<SIZE>(block, detail::opcode_POP, mem);
		}
		
		/* Push value to the stack
		 - POP sreg
		*/
		template<class BLOCK>
		static void Pop(BLOCK& block, RegSeg sreg) {
			if (sreg == ES) {
				common::write_Opcode(block, 0x07);
			} else if (sreg == SS) {
				common::write_Opcode(block, 0x17);
			} else if (sreg == DS) {
				common::write_Opcode(block, 0x1F);
			} else if (sreg == FS) {
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, 0xA1);
			} else if (sreg == GS) {
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, 0xA9);
			}
		}

		/* Load all 16 bit general registers from stack */
		template<class BLOCK>
		static void Popa(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x61);
		}

		/* Load all 32 bit general registers from stack */
		template<class BLOCK>
		static void Popad(BLOCK& block) {
			common::write_Opcode(block, 0x61);
		}

		/* Xchange
		 - XCHG reg32,reg32
		*/
		template<class BLOCK>
		static void Xchg(BLOCK& block, Reg32 dst, Reg32 src) {
#ifdef X86_SIZE_OPTIMIZED
			if (dst == X86::EAX) {
				block.pushRaw<uint8_t>(0x90 | src);
				return;
			}
			if (src == X86::EAX) {
				block.pushRaw<uint8_t>(0x90 | dst);
				return;
			}
#endif
			block.pushRaw<uint8_t>(0x87);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
		}

		/* Xchange
		 - XCHG reg16,reg16
		*/
		template<class BLOCK>
		static void Xchg(BLOCK& block, Reg16 dst, Reg16 src) {
			common::write_Opcode_16bit_Prefix(block);
#ifdef X86_SIZE_OPTIMIZED
			if (dst == X86::EAX) {
				block.pushRaw<uint8_t>(0x90 | src);
				return;
			}
			if (src == X86::EAX) {
				block.pushRaw<uint8_t>(0x90 | dst);
				return;
			}
#endif
			block.pushRaw<uint8_t>(0x87);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
		}

		/* Xchange
		 - XCHG reg8,reg8
		*/
		template<class BLOCK>
		static void Xchg(BLOCK& block, Reg8 dst, Reg8 src) {
			block.pushRaw<uint8_t>(0x86);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
		}

		/* Xchange
		 - XCHG reg,[mem]
		 - XCHG [mem],reg
		*/
		template<class DST, class SRC, class BLOCK>
		static auto Xchg(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_symetric(block, 0x86, dst, src);
		}

		/* Translate by table */
		template<class BLOCK>
		static void Xlat(BLOCK& block) {
			common::write_Opcode(block, 0xD7);
		}

		/* Read data from port
		 - IN reg,imm
		*/
		template<MemSize SIZE, class BLOCK>
		static ReplaceableValue<U8> In(BLOCK& block, const U8& imm) {
			write_Opcode<SIZE>(block, 0xE4);
			Offset offset = block.getOffset();
			common::write_Immediate(block, imm);
			return ReplaceableValue<U8>(offset);
		}

		/* Read data from port
		 - IN eax/ax,dx
		*/
		template<MemSize SIZE, Reg16 PORT_REG, class BLOCK>
		static void In(BLOCK& block) {
			write_Opcode<SIZE>(block, 0xEC);
		}

		/* Write data to port
		 - OUT imm, reg
		*/
		template<MemSize SIZE, class BLOCK>
		static ReplaceableValue<U8> Out(BLOCK& block, const U8& imm) {
			write_Opcode<SIZE>(block, 0xE6);
			Offset offset = block.getOffset();
			common::write_Immediate(block, imm);
			return ReplaceableValue<U8>(offset);
		}

		/* Write data to port
		 - OUT dx,eax/ax
		*/
		template<MemSize SIZE, Reg16 PORT_REG, class BLOCK>
		static void Out(BLOCK& block) {
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
		template<class DST, class SRC, class BLOCK>
		static auto Adc(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_ADC, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Addition with carry flag (CF)
		 - ADC reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T, class BLOCK>
		static void Adc(BLOCK& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x14, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_ADC, reg, imm);
			}
		}
#endif
		/* Addition with carry flag (CF)
		 - ADC [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static auto Adc(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_ADC, mem, imm);
		}

		/* Addition with carry flag (CF)
		 - ADC [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static auto Adc(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Adc<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		/* Addition
		 - ADD reg,reg
		 - ADD reg,[mem]
		 - ADD [mem],reg
		 - ADD reg,imm
		*/
		template<class DST, class SRC, class BLOCK>
		static auto Add(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_ADD, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Addition
		 - ADD reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T, class BLOCK>
		static void Add(BLOCK& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x04, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_ADD, reg, imm);
			}
		}
#endif
		/* Addition
		 - ADD [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static auto Add(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_ADD, mem, imm);
		}

		/* Addition
		 - ADD [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static auto Add(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Add<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		/* Compare Two Operands
		 - CMP reg,reg
		 - CMP reg,[mem]
		 - CMP [mem],reg
		 - CMP reg,imm
		*/
		template<class DST, class SRC, class BLOCK>
		static auto Cmp(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_CMP, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Compare Two Operands
		 - CMP reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T, class BLOCK>
		static void Cmp(BLOCK& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x3C, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_CMP, reg, imm);
			}
		}
#endif
		/* Compare Two Operands
		 - CMP [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static auto Cmp(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_CMP, mem, imm);
		}

		/* Compare Two Operands
		 - CMP [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static auto Cmp(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Cmp<TypeMemSize<Imm<T>::value>(block, mem, imm);
		}

		/* Compare and set flags
		 - CMP imm,[mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static auto Cmp(BLOCK& block, const Imm<T>& imm, const Mem32<MODE>& mem) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_CMP, mem, imm);
		}

		/* Compare and set flags
		 - CMP imm,[mem] (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static auto Cmp(BLOCK& block, const Imm<T>& imm, const Mem32<MODE>& mem) {
			return Cmp<TypeMemSize<Imm<T>::value>(block, mem, imm);
		}

		/* Increment
		 - INC reg32
		*/
		template<class BLOCK>
		static ReplaceableReg<Reg32> Inc(BLOCK& block, Reg32 reg) {
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x40 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Increment
		 - INC reg16
		*/
		template<class BLOCK>
		static ReplaceableReg<Reg16> Inc(BLOCK& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x40 | reg);
			return ReplaceableReg<Reg16>(offset, 0);
		}

		/* Increment
		 - INC reg8
		*/
		template<class BLOCK>
		static ReplaceableReg<Reg8> Inc(BLOCK& block, Reg8 reg) {
			return template_1reg_operand(block, detail::opcode_INC, reg);
		}

		/* Increment
		 - INC [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Inc(BLOCK& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_INC, mem);
		}

		/* Decrement
		 - DEC reg32
		*/
		template<class BLOCK>
		static ReplaceableReg<Reg32> Dec(BLOCK& block, Reg32 reg) {
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x48 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Decrement
		 - DEC reg16
		*/
		template<class BLOCK>
		static ReplaceableReg<Reg16> Dec(BLOCK& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x48 | reg);
			return ReplaceableReg<Reg16>(offset, 0);
		}

		/* Decrement
		 - DEC reg8
		*/
		template<class BLOCK>
		static ReplaceableReg<Reg8> Dec(BLOCK& block, Reg8 reg) {
			return template_1reg_operand(block, detail::opcode_DEC, reg);
		}

		/* Decrement
		- DEC [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Dec(BLOCK& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_DEC, mem);
		}

		/* Change sign
		 - NEG reg
		*/
		template<class REG, class BLOCK>
		static ReplaceableReg<REG> Neg(BLOCK& block, REG reg) {
			return template_1reg_operand(block, detail::opcode_NEG, reg);
		}

		/* Change sign
		 - NEG [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Neg(BLOCK& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_NEG, mem);
		}

		/* Multiply unsigned numbers
		 - MUL reg
		*/
		template<class REG, class BLOCK>
		static ReplaceableReg<REG> Mul(BLOCK& block, REG reg) {
			return template_1reg_operand(block, detail::opcode_MUL, reg);
		}

		/* Multiply unsigned numbers
		 - MUL [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Mul(BLOCK& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_MUL, mem);
		}

		/* Signed Multiply
		 - IMUL reg
		*/
		template<class REG, class BLOCK>
		static ReplaceableReg<REG> Imul(BLOCK& block, REG reg) {
			return template_1reg_operand(block, detail::opcode_IMUL, reg);
		}

		/* Signed Multiply
		 - IMUL [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Imul(BLOCK& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_IMUL, mem);
		}

		/* Signed Multiply
		 - IMUL reg,reg
		*/
		template<class REG, class BLOCK>
		static void Imul(BLOCK& block, REG reg1, REG reg2) {
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, 0xAF);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg1, reg2);
		}

		/* Signed Multiply
		- IMUL reg,reg,imm
		*/
		template<class REG, class T, class BLOCK>
		static void Imul(BLOCK& block, REG reg1, REG reg2, const Imm<T>& imm) {
			write_Opcode_Imm_Optimized<TypeMemSize<REG>::value, TypeMemSize<Imm<T>>::value>(block, 0x69);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg1, reg2);
			common::write_Immediate(block, imm);
		}

		/* Signed Multiply
		- IMUL reg,reg,imm
		*/
		template<class REG, AddressMode MODE, class T, class BLOCK>
		static void Imul(BLOCK& block, REG reg, const Mem32<MODE>& mem, const Imm<T>& imm) {
			mem.writeSegmPrefix(block);
			write_Opcode_Imm_Optimized<TypeMemSize<REG>::value, TypeMemSize<Imm<T>>::value>(block, 0x69);
			mem.write(block, reg);
			common::write_Immediate(block, imm);
		}

		/* Divide unsigned numbers
		 - DIV reg
		*/
		template<class REG, class BLOCK>
		static ReplaceableReg<REG> Div(BLOCK& block, REG reg) {
			return template_1reg_operand(block, detail::opcode_DIV, reg);
		}

		/* Divide unsigned numbers
		 - DIV [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Div(BLOCK& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_DIV, mem);
		}

		/* Divide signed numbers
		 - IDIV reg
		*/
		template<class REG, class BLOCK>
		static ReplaceableReg<REG> Idiv(BLOCK& block, REG reg) {
			return template_1reg_operand(block, detail::opcode_IDIV, reg);
		}

		/* Divide signed numbers
		 - IDIV [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Idiv(BLOCK& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_IDIV, mem);
		}

		/* Integer Subtraction with Borrow
		 - SBB reg,reg
		 - SBB reg,[mem]
		 - SBB [mem],reg
		 - SBB reg,imm
		*/
		template<class DST, class SRC, class BLOCK>
		static auto Sbb(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_SBB, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Integer Subtraction with Borrow
		 - SBB reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T, class BLOCK>
		static void Sbb(BLOCK& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x1C, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_SBB, reg, imm);
			}
		}
#endif
		/* Integer Subtraction with Borrow
		 - SBB [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static auto Sbb(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_SBB, mem, imm);
		}

		/* Integer Subtraction with Borrow
		 - SBB [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static auto Sbb(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Sbb<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		/* Subtract
		 - SUB reg,reg
		 - SUB reg,[mem]
		 - SUB [mem],reg
		 - SUB reg,imm
		*/
		template<class DST, class SRC, class BLOCK>
		static auto Sub(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_SUB, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Subtract
		 - SUB reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T, class BLOCK>
		static void Sub(BLOCK& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x2C, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_SBB, reg, imm);
			}
		}
#endif
		/* Subtract
		 - SUB [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static auto Sub(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_SUB, mem, imm);
		}

		/* Subtract
		 - SUB [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static auto Sub(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Sub<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}
#pragma endregion

#pragma region Decimal arithmetics [DONE]
		/* ACII correction after addition */
		template<class BLOCK>
		static void Aaa(BLOCK& block) {
			common::write_Opcode(block, 0x37);
		}

		/* ASCII correction before divide 
		 - AAD {imm} 
		 */
		template<class BLOCK>
		static ReplaceableValue<S8::type> Aad(BLOCK& block, S8 base = S8(10)) {
			common::write_Opcode(block, 0xD5);
			Offset offset = block.getOffset();
			common::write_Immediate(block, base);
			return ReplaceableValue<S8::type>(offset);
		}

		/* ASCII correction after multiply
		 - AAM {imm}
		 */
		template<class BLOCK>
		static ReplaceableValue<S8::type> Aam(BLOCK& block, S8 base = S8(10)) {
			common::write_Opcode(block, 0xD4);
			Offset offset = block.getOffset();
			common::write_Immediate(block, base);
			return ReplaceableValue<S8::type>(offset);
		}

		/* ASCII correction after subtraction */
		template<class BLOCK>
		static void Aas(BLOCK& block) {
			common::write_Opcode(block, 0x3F);
		}

		/* BCD-correction after addition */
		template<class BLOCK>
		static void Daa(BLOCK& block) {
			common::write_Opcode(block, 0x27);
		}

		/* BCD-correction after subtraction */
		template<class BLOCK>
		static void Das(BLOCK& block) {
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
		template<class DST, class SRC, class BLOCK>
		static auto And(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_AND, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Logical AND
		 - AND reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T, class BLOCK>
		static void And(BLOCK& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x24, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_AND, reg, imm);
			}
		}
#endif
		/* Logical AND
		 - AND [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static auto And(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_AND, mem, imm);
		}

		/* Logical AND
		 - AND [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static auto And(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return And<TypeMemSize<Imm<T>::value>(block, mem, imm);
		}

		/* Logical NOT operation
		 - NOT reg
		*/
		template<class REG, class BLOCK>
		static ReplaceableReg<REG> Not(BLOCK& block, REG reg) {
			return template_1reg_operand(block, detail::opcode_NOT, reg);
		}

		/* Logical NOT operation
		 - NOT [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Not(BLOCK& block, const Mem32<MODE>& mem) {
			return template_1mem_operand<SIZE>(block, detail::opcode_NOT, mem);
		}

		/* Logical OR operation
		 - OR reg,reg
		 - OR reg,[mem]
		 - OR [mem],reg
		 - OR reg,imm
		*/
		template<class DST, class SRC, class BLOCK>
		static auto Or(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_OR, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Logical OR
		 - OR reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T, class BLOCK>
		static void Or(BLOCK& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x0C, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_OR, reg, imm);
			}
		}
#endif
		/* Logical OR operation
		 - OR [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static auto Or(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_OR, mem, imm);
		}

		/* Logical OR operation
		 - OR [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static auto Or(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Or<TypeMemSize<Imm<T>::value>(block, mem, imm);
		}

		/* Xchange
		 - TEST reg32,reg32
		*/
		template<class BLOCK>
		static void Test(BLOCK& block, Reg32 dst, Reg32 src) {
			block.pushRaw<uint8_t>(detail::opcode_TEST.getMain() | 1);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
		}

		/* Logical Compare
		 - TEST reg16,reg16
		*/
		template<class BLOCK>
		static void Test(BLOCK& block, Reg16 dst, Reg16 src) {
			common::write_Opcode_16bit_Prefix(block);
			block.pushRaw<uint8_t>(detail::opcode_TEST.getMain() | 1);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
		}

		/* Logical Compare
		 - TEST reg8,reg8
		*/
		template<class BLOCK>
		static void Test(BLOCK& block, Reg8 dst, Reg8 src) {
			block.pushRaw<uint8_t>(detail::opcode_TEST.getMain());
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
		}

		/* Logical Compare
		 - TEST reg,imm
		*/
		template<class REG, class T, class BLOCK>
		static auto Test(BLOCK& block, REG reg, const Imm<T>& imm) {
			return template_reg_imm_operands(block, detail::opcode_TEST, reg, imm);
		}

		/* Logical Compare
		 - TEST reg,[mem]
		 - TEST [mem],reg
		*/
		template<class DST, class SRC, class BLOCK>
		static auto Test(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_symetric(block, detail::opcode_TEST.getMain(), dst, src);
		}

		/* Logical Compare
		 - TEST [mem],imm
		 */
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static auto Test(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_operands<SIZE>(block, detail::opcode_TEST, mem, imm);
		}

		/* Logical Compare
		 - TEST [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static auto Test(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Test<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Logical Compare
		 - TEST reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T, class BLOCK>
		static void Test(BLOCK& block, const REG& reg, const Imm<T>& imm) {
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
		template<class DST, class SRC, class BLOCK>
		static auto Xor(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm(block, detail::opcode_XOR, dst, src);
		}
#ifdef X86_SIZE_OPTIMIZED
		/* Logical Exclusive OR
		 - XOR reg,imm		; if reg == EAX/AX/AL opcode size optimized
		*/
		template<class REG, class T, class BLOCK>
		static void Xor(BLOCK& block, const REG& reg, const Imm<T>& imm) {
			if (!template_2operands_first_reg_imm(block, 0x34, reg, imm)) {
				template_2operands_opt_imm(block, detail::opcode_XOR, reg, imm);
			}
		}
#endif
		/* Logical Exclusive OR
		 - XOR [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static auto Xor(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return template_mem_imm_opt_operands<SIZE>(block, detail::opcode_XOR, mem, imm);
		}

		/* Logical Exclusive OR
		 - XOR [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static auto Xor(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Xor<TypeMemSize<Imm<T>::value>(block, mem, imm);
		}
#pragma endregion
		
#pragma region Bit shift operations [DONE]
		template<MemSize SIZE, Reg8 REG, AddressMode MODE, class BLOCK>
		static auto Rol(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_ROL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Rol(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_ROL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Rol(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_ROL, mem, imm);
		}

		template<class REG, class BLOCK>
		static auto Rol(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_ROL, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Rol(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_ROL, reg);
		}

		template<class REG, class BLOCK>
		static auto Rol(BLOCK& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_ROL, reg, imm);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE, class BLOCK>
		static auto Ror(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_ROR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Ror(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_ROR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Ror(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_ROR, mem, imm);
		}

		template<class REG, class BLOCK>
		static auto Ror(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_ROR, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Ror(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_ROR, reg);
		}

		template<class REG, class BLOCK>
		static auto Ror(BLOCK& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_ROR, reg, imm);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE, class BLOCK>
		static auto Rcl(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_RCL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Rcl(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_RCL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Rcl(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_RCL, mem, imm);
		}

		template<class REG, class BLOCK>
		static auto Rcl(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_RCL, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Rcl(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_RCL, reg);
		}

		template<class REG, class BLOCK>
		static auto Rcl(BLOCK& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_RCL, reg, imm);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE, class BLOCK>
		static auto Rcr(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_RCR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Rcr(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_RCR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Rcr(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_RCR, mem, imm);
		}

		template<class REG, class BLOCK>
		static auto Rcr(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_RCR, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Rcr(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_RCR, reg);
		}

		template<class REG, class BLOCK>
		static auto Rcr(BLOCK& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_RCR, reg, imm);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE, class BLOCK>
		static auto Sal(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_SAL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Sal(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_SAL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Sal(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_SAL, mem, imm);
		}

		template<class REG, class BLOCK>
		static auto Sal(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_SAL, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Sal(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_SAL, reg);
		}

		template<class REG, class BLOCK>
		static auto Sal(BLOCK& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_SAL, reg, imm);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE, class BLOCK>
		static auto Shl(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_SHL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Shl(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_SHL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Shl(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_SHL, mem, imm);
		}

		template<class REG, class BLOCK>
		static auto Shl(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_SHL, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Shl(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_SHL, reg);
		}

		template<class REG, class BLOCK>
		static auto Shl(BLOCK& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_SHL, reg, imm);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE, class BLOCK>
		static auto Shr(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_SHR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Shr(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_SHR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Shr(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_SHR, mem, imm);
		}

		template<class REG, class BLOCK>
		static auto Shr(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_SHR, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Shr(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_SHR, reg);
		}

		template<class REG, class BLOCK>
		static auto Shr(BLOCK& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_SHR, reg, imm);
		}

		template<MemSize SIZE, Reg8 REG, AddressMode MODE, class BLOCK>
		static auto Sar(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE, REG>(block, detail::opcode_SAR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Sar(BLOCK& block, const Mem32<MODE>& mem) {
			return template_shift_operands<SIZE>(block, detail::opcode_SAR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Sar(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			return template_shift_operands<SIZE>(block, detail::opcode_SAR, mem, imm);
		}

		template<class REG, class BLOCK>
		static auto Sar(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG>(block, detail::opcode_SAR, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Sar(BLOCK& block, const REG& reg) {
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_SAR, reg);
		}

		template<class REG, class BLOCK>
		static auto Sar(BLOCK& block, const REG& reg, const U8& imm) {
			return template_shift_operands<REG>(block, detail::opcode_SAR, reg, imm);
		}

		template<MemSize SIZE, Reg8 COUNT_REG, class REG, AddressMode MODE, class BLOCK>
		static auto Shrd(BLOCK& block, const Mem32<MODE>& mem, REG reg) {
			return template_prshift_operands<SIZE, COUNT_REG>(block, 0xAC, mem, reg);
		}

		template<Reg8 COUNT_REG, class REG, class BLOCK>
		static auto Shrd(BLOCK& block, REG reg1, REG reg2) {
			return template_prshift_operands<COUNT_REG>(block, 0xAC, reg1, reg2);
		}

		template<MemSize SIZE, class REG, AddressMode MODE, class BLOCK>
		static auto Shrd(BLOCK& block, const Mem32<MODE>& mem, REG reg, const U8& imm) {
			return template_prshift_operands<SIZE>(block, 0xAC, mem, reg, imm);
		}

		template<class REG, class BLOCK>
		static auto Shrd(BLOCK& block, REG reg1, REG reg2, const U8& imm) {
			return template_prshift_operands(block, 0xAC, reg1, reg2, imm);
		}

		template<MemSize SIZE, Reg8 COUNT_REG, class REG, AddressMode MODE, class BLOCK>
		static auto Shld(BLOCK& block, const Mem32<MODE>& mem, REG reg) {
			return template_prshift_operands<SIZE, COUNT_REG>(block, 0xA4, mem, reg);
		}

		template<Reg8 COUNT_REG, class REG, class BLOCK>
		static auto Shld(BLOCK& block, REG reg1, REG reg2) {
			return template_prshift_operands<COUNT_REG>(block, 0xA4, reg1, reg2);
		}

		template<MemSize SIZE, class REG, AddressMode MODE, class BLOCK>
		static auto Shld(BLOCK& block, const Mem32<MODE>& mem, REG reg, const U8& imm) {
			return template_prshift_operands<SIZE>(block, 0xA4, mem, reg, imm);
		}

		template<class REG, class BLOCK>
		static auto Shld(BLOCK& block, REG reg1, REG reg2, const U8& imm) {
			return template_prshift_operands(block, 0xA4, reg1, reg2, imm);
		}
#pragma endregion

#pragma region Bit manipulations [DONE]
		template<class REG, class BLOCK>
		static auto Bsf(BLOCK& block, REG reg1, REG reg2) {
			return template_bit_scan(block, 0xBC, reg1, reg2);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static auto Bsf(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			return template_bit_scan(block, 0xBC, reg, mem);
		}

		template<class REG, class BLOCK>
		static auto Bsr(BLOCK& block, REG reg1, REG reg2) {
			return template_bit_scan(block, 0xBD, reg1, reg2);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static auto Bsr(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			return template_bit_scan(block, 0xBD, reg, mem);
		}

		template<class T1, class T2, class BLOCK>
		static auto Bt(BLOCK& block, T1 val1, T2 val2) {
			return template_bit_operation(block, detail::opcode_BT, val1, val2);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Bt(BLOCK& block, const Mem32<MODE>& mem, U8 imm) {
			return template_bit_operation<SIZE>(block, detail::opcode_BT, mem, imm);
		}

		template<class T1, class T2, class BLOCK>
		static auto Btc(BLOCK& block, T1 val1, T2 val2) {
			return template_bit_operation(block, detail::opcode_BTC, val1, val2);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Btc(BLOCK& block, const Mem32<MODE>& mem, U8 imm) {
			return template_bit_operation<SIZE>(block, detail::opcode_BTC, mem, imm);
		}

		template<class T1, class T2, class BLOCK>
		static auto Btr(BLOCK& block, T1 val1, T2 val2) {
			return template_bit_operation(block, detail::opcode_BTR, val1, val2);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Btr(BLOCK& block, const Mem32<MODE>& mem, U8 imm) {
			return template_bit_operation<SIZE>(block, detail::opcode_BTR, mem, imm);
		}

		template<class T1, class T2, class BLOCK>
		static auto Bts(BLOCK& block, T1 val1, T2 val2) {
			return template_bit_operation(block, detail::opcode_BTS, val1, val2);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Bts(BLOCK& block, const Mem32<MODE>& mem, U8 imm) {
			return template_bit_operation<SIZE>(block, detail::opcode_BTS, mem, imm);
		}

		/* Set byte if overflow (OF == 1) */
		template<class T, class BLOCK>
		static auto Seto(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x90, val);
		}

		/* Set byte if no overflow (OF == 0) */
		template<class T, class BLOCK>
		static auto Setno(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x91, val);
		}

		/* Set byte if unsigned lower (CF == 1) */
		template<class T, class BLOCK>
		static auto Setb(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x92, val);
		}

		/* Set byte if carry (CF == 1) */
		template<class T, class BLOCK>
		static auto Setc(BLOCK& block, const T& val) {
			return Setb(block, val);
		}

		/* Set byte if unsigned not greater-equal (CF == 1) */
		template<class T, class BLOCK>
		static auto Setnae(BLOCK& block, const T& val) {
			return Setb(block, val);
		}

		/* Set byte if unsigned greater-equal (CF == 0) */
		template<class T, class BLOCK>
		static auto Setae(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x93, val);
		}

		/* Set byte if no carry (CF == 0) */
		template<class T, class BLOCK>
		static auto Setnc(BLOCK& block, const T& val) {
			return Setae(block, val);
		}

		/* Set byte if unsigned not lower (CF == 0) */
		template<class T, class BLOCK>
		static auto Setnb(BLOCK& block, const T& val) {
			return Setae(block, val);
		}

		/* Set byte if equal (ZF == 1) */
		template<class T, class BLOCK>
		static auto Sete(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x94, val);
		}

		/* Set byte if zero (ZF == 1) */
		template<class T, class BLOCK>
		static auto Setz(BLOCK& block, const T& val) {
			return Sete(block, val);
		}

		/* Set byte if not equal (ZF == 0) */
		template<class T, class BLOCK>
		static auto Setne(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x95, val);
		}

		/* Set byte if not zero (ZF == 0) */
		template<class T, class BLOCK>
		static auto Setnz(BLOCK& block, const T& val) {
			return Setne(block, val);
		}

		/* Set byte if unsigned lower-equal (CF == 1 || ZF == 1) */
		template<class T, class BLOCK>
		static auto Setbe(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x96, val);
		}

		/* Set byte if unsigned not greater (CF == 1 || ZF == 1) */
		template<class T, class BLOCK>
		static auto Setna(BLOCK& block, const T& val) {
			return Setbe(block, val);
		}

		/* Set byte if unsigned greater (CF == 0 && ZF == 0) */
		template<class T, class BLOCK>
		static auto Seta(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x97, val);
		}

		/* Set byte if unsigned not lower-equal (CF == 0 && ZF == 0) */
		template<class T, class BLOCK>
		static auto Setnbe(BLOCK& block, const T& val) {
			return Seta(block, val);
		}

		/* Set byte if sign (SF == 1) */
		template<class T, class BLOCK>
		static auto Sets(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x98, val);
		}

		/* Set byte if no sign (SF == 0) */
		template<class T, class BLOCK>
		static auto Setns(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x99, val);
		}

		/* Set byte if parity (PF == 1) */
		template<class T, class BLOCK>
		static auto Setp(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x9A, val);
		}

		/* Set byte if parity (PF == 1) */
		template<class T, class BLOCK>
		static auto Setpe(BLOCK& block, const T& val) {
			return Setp(block, val);
		}

		/* Set byte if no parity (PF == 0) */
		template<class T, class BLOCK>
		static auto Setnp(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x9B, val);
		}

		/* Set byte if no parity (PF == 0) */
		template<class T, class BLOCK>
		static auto Setpo(BLOCK& block, const T& val) {
			return Setnp(block, val);
		}

		/* Set byte if signed lower (SF != OF) */
		template<class T, class BLOCK>
		static auto Setl(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x9C, val);
		}

		/* Set byte if signed not greater-equal (SF != OF) */
		template<class T, class BLOCK>
		static auto Setnge(BLOCK& block, const T& val) {
			return Setl(block, val);
		}

		/* Set byte if signed greater-equal (ZF == 0 && SF == OF) */
		template<class T, class BLOCK>
		static auto Setge(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x9D, val);
		}

		/* Set byte if signed not lower (ZF == 0 && SF == OF) */
		template<class T, class BLOCK>
		static auto Setnl(BLOCK& block, const T& val) {
			return Setge(block, val);
		}

		/* Set byte if signed lower-equal (ZF == 1 && SF != OF) */
		template<class T, class BLOCK>
		static auto Setle(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x9E, val);
		}

		/* Set byte if signed not greater (ZF == 1 && SF != OF) */
		template<class T, class BLOCK>
		static auto Setng(BLOCK& block, const T& val) {
			return Setle(block, val);
		}

		/* Set byte if signed greater (ZF == 0 && SF == OF) */
		template<class T, class BLOCK>
		static auto Setg(BLOCK& block, const T& val) {
			return template_Setcc(block, 0x9F, val);
		}

		/* Set byte if signed not lower-equal (ZF == 0 && SF == OF) */
		template<class T, class BLOCK>
		static auto Setnle(BLOCK& block, const T& val) {
			return Setg(block, val);
		}
#pragma endregion

#pragma region Flow control
		/* Call procedure
		 - CALL reg
		 - CALL [mem]
		*/
		template<class T, class BLOCK>
		static auto Call(BLOCK& block, const T& addr) {
			return template_Call(block, addr);
		}

		/* Call procedure
		 - CALL imm_addr
		 - CALL label
		*/
		template<class BLOCK>
		static FwdLabel<LONG> Call(BLOCK& block) {
			common::write_Opcode(block, 0xE8);
			Offset offset = block.getOffset();
			block.skipBytes(FwdLabel<LONG>::offset_size);
			return FwdLabel<LONG>(offset);
		}

		/* Jump always
		 - JMP reg
		 - JMP [mem]
		*/
		template<class T, class BLOCK>
		static auto Jmp(BLOCK& block, const T& addr) {
			return template_Jmp(block, addr);
		}

		/* Jump always
		 - JMP imm_addr
		 - JMP label
		*/
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jmp(BLOCK& block) {
			return template_Jmp<SIZE>(block);
		}

		/* Jump in overflow (OF == 1) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jo(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x70);
		}

		/* Jump in no overflow (OF == 0) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jno(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x71);
		}

		/* Jump if unsigned lower (CF == 1) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jb(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x72);
		}

		/* Jump if carry (CF == 1) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jc(BLOCK& block) {
			return Jb<SIZE>(block);
		}

		/* Jump if unsigned not greater-equal (CF == 1) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jnae(BLOCK& block) {
			return Jb<SIZE>(block);
		}

		/* Jump if unsigned greater-equal (CF == 0) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jae(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x73);
		}

		/* Jump if no carry (CF == 0) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jnc(BLOCK& block) {
			return Jae<SIZE>(block);
		}

		/* Jump if unsigned not lower (CF == 0) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jnb(BLOCK& block) {
			return Jae<SIZE>(block);
		}

		/* Jump if equal (ZF == 1) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Je(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x74);
		}

		/* Jump if zero (ZF == 1) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jz(BLOCK& block) {
			return Je<SIZE>(block);
		}

		/* Jump if not equal (ZF == 0) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jne(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x75);
		}

		/* Jump if not zero (ZF == 0) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jnz(BLOCK& block) {
			return Jne<SIZE>(block);
		}

		/* Jump if unsigned lower-equal (CF == 1 || ZF == 1) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jbe(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x76);
		}

		/* Jump if unsigned not greater (CF == 1 || ZF == 1) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jna(BLOCK& block) {
			return Jbe<SIZE>(block);
		}

		/* Jump if unsigned greater (CF == 0 && ZF == 0) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Ja(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x77);
		}

		/* Jump if unsigned not lower-equal (CF == 0 && ZF == 0) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jnbe(BLOCK& block) {
			return Ja<SIZE>(block);
		}

		/* Jump if sign (SF == 1) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Js(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x78);
		}

		/* Jump if no sign (SF == 0) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jns(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x79);
		}

		/* Jump if parity (PF == 1) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jp(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x7A);
		}

		/* Jump if parity (PF == 1) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jpe(BLOCK& block) {
			return Jp<SIZE>(block);
		}

		/* Jump if no parity (PF == 0) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jnp(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x7B);
		}

		/* Jump if no parity (PF == 0) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jpo(BLOCK& block) {
			return Jnp<SIZE>(block);
		}

		/* Jump if signed lower (SF != OF) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jl(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x7C);
		}

		/* Jump if signed not greater-equal (SF != OF) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jnge(BLOCK& block) {
			return Jl<SIZE>(block);
		}

		/* Jump if signed greater-equal (ZF == 0 && SF == OF) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jge(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x7D);
		}

		/* Jump if signed not lower (ZF == 0 && SF == OF) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jnl(BLOCK& block) {
			return Jge<SIZE>(block);
		}

		/* Jump if signed lower-equal (ZF == 1 && SF != OF) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jle(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x7E);
		}

		/* Jump if signed not greater (ZF == 1 && SF != OF) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jng(BLOCK& block) {
			return Jle<SIZE>(block);
		}

		/* Jump if signed greater (ZF == 0 && SF == OF) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jg(BLOCK& block) {
			return template_Jxx<SIZE>(block, 0x7F);
		}

		/* Jump if signed not lower-equal (ZF == 0 && SF == OF) */
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jnle(BLOCK& block) {
			return Jg<SIZE>(block);
		}

		/* Launch interrupt
		 - INT imm
		*/
		template<class BLOCK>
		static ReplaceableValue<U8::type> Int(BLOCK& block, U8 number) {
			common::write_Opcode(block, 0xCD);
			Offset offset = block.getOffset();
			common::write_Immediate(block, number);
			return ReplaceableValue<U8::type>(offset);
		}

		/* Launch interrupt number 3 */
		template<class BLOCK>
		static void Int3(BLOCK& block) {
			common::write_Opcode(block, 0xCC);
		}

		/* Launch interrupt number 4 if overflow flag (OF) is set */
		template<class BLOCK>
		static void IntO(BLOCK& block) {
			common::write_Opcode(block, 0xCE);
		}

		/* Return from interrupt */
		template<class BLOCK>
		static void Iret(BLOCK& block) {
			common::write_Opcode(block, 0xCF);
		}

		/* Create enter procedure stack frame */
		template<class BLOCK>
		static auto Enter(BLOCK& block, U16 size, U8 level) {
			common::write_Opcode(block, 0xC8);
			Offset offset = block.getOffset();
			common::write_Immediate(block, size);
			common::write_Immediate(block, level);
			return std::make_pair(ReplaceableValue<U16::type>(offset),
				ReplaceableValue<U8::type>(offset + sizeof(U16::type)));
		}

		/* Create leave procedure stack frame */
		template<class BLOCK>
		static void Leave(BLOCK& block) {
			common::write_Opcode(block, 0xC9);
		}

		/* Return from near procedure */
		template<class BLOCK>
		static void Ret(BLOCK& block) {
			common::write_Opcode(block, 0xC3);
		}

		/* Return from near procedure */
		template<class BLOCK>
		static ReplaceableValue<U16::type> Ret(BLOCK& block, U16 number) {
			common::write_Opcode(block, 0xC2);
			Offset offset = block.getOffset();
			common::write_Immediate(block, number);
			return ReplaceableValue<U16::type>(offset);
		}

		/* Return from near procedure */
		template<class BLOCK>
		static void Retn(BLOCK& block) {
			Ret(block);
		}

		/* Return from near procedure */
		template<class BLOCK>
		static auto Retn(BLOCK& block, U16 number) {
			return Ret(block, number);
		}

		/* Return from far procedure */
		template<class BLOCK>
		static void Retf(BLOCK& block) {
			common::write_Opcode(block, 0xCB);
		}

		/* Return from far procedure */
		template<class BLOCK>
		static ReplaceableValue<U16::type> Retf(BLOCK& block, U16 number) {
			common::write_Opcode(block, 0xCA);
			Offset offset = block.getOffset();
			common::write_Immediate(block, number);
			return ReplaceableValue<U16::type>(offset);
		}

		/* Jump if ECX == 0 */
		template<class BLOCK>
		static FwdLabel<SHORT> Jecxz(BLOCK& block) {
			return template_Loop<SHORT>(block, 0xE3);
		}

		/* Jump if CX == 0 */
		template<class BLOCK>
		static FwdLabel<SHORT> Jcxz(BLOCK& block) {
			block.pushRaw<uint8_t>(0x67);
			return Jecxz(block);
		}

		/* Decrement ECX and jump if ECX != 0 */
		template<class BLOCK>
		static FwdLabel<SHORT> Loop(BLOCK& block) {
			return template_Loop<SHORT>(block, 0xE2);
		}

		/* Decrement ECX and jump if ECX != 0 and ZF == 1 */
		template<class BLOCK>
		static FwdLabel<SHORT> Loope(BLOCK& block) {
			return template_Loop<SHORT>(block, 0xE1);
		}

		/* Decrement ECX and jump if ECX != 0 and ZF == 1 */
		template<class BLOCK>
		static FwdLabel<SHORT> Loopz(BLOCK& block) {
			return Loope(block);
		}

		/* Decrement ECX and jump if ECX != 0 and ZF == 0 */
		template<class BLOCK>
		static FwdLabel<SHORT> Loopne(BLOCK& block) {
			return template_Loop<SHORT>(block, 0xE0);
		}

		/* Decrement ECX and jump if ECX != 0 and ZF == 0 */
		template<class BLOCK>
		static FwdLabel<SHORT> Loopnz(BLOCK& block) {
			return Loopne(block);
		}
#pragma endregion

#pragma region Flags manipulations [DONE]
		/* Reset carry flag (CF) */
		template<class BLOCK>
		static void Clc(BLOCK& block) {
			common::write_Opcode(block, 0xF8);
		}

		/* Reset direction flag (DF) */
		template<class BLOCK>
		static void Cld(BLOCK& block) {
			common::write_Opcode(block, 0xFC);
		}

		/* Clear interrupt flag (IF) */
		template<class BLOCK>
		static void Cli(BLOCK& block) {
			common::write_Opcode(block, 0xFA);
		}

		/* Invert carry flag (CF) */
		template<class BLOCK>
		static void Cmc(BLOCK& block) {
			common::write_Opcode(block, 0xF5);
		}

		/* Load flags to register AH */
		template<class BLOCK>
		static void Lahf(BLOCK& block) {
			common::write_Opcode(block, 0x9F);
		}

		/* Save FLAGS to stack */
		template<class BLOCK>
		static void Pushf(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x9C);
		}

		/* Save EFLAGS to stack */
		template<class BLOCK>
		static void Pushfd(BLOCK& block) {
			common::write_Opcode(block, 0x9C);
		}

		/* Load FLAGS from stack */
		template<class BLOCK>
		static void Popf(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x9D);
		}

		/* Load EFLAGS from stack */
		template<class BLOCK>
		static void Popfd(BLOCK& block) {
			common::write_Opcode(block, 0x9D);
		}

		/* Store register AH to flags */
		template<class BLOCK>
		static void Sahf(BLOCK& block) {
			common::write_Opcode(block, 0x9E);
		}

		/* Set all register AL bits according CF flag 
		 - Remark: Not documented Intel instruction
		*/
		template<class BLOCK>
		static void Salc(BLOCK& block) {
			common::write_Opcode(block, 0xD6);
		}

		/* Set carry flag (CF) */
		template<class BLOCK>
		static void Stc(BLOCK& block) {
			common::write_Opcode(block, 0xF9);
		}

		/* Set direction flag (DF) */
		template<class BLOCK>
		static void Std(BLOCK& block) {
			common::write_Opcode(block, 0xFD);
		}

		/* Set interrupt flag (IF) */
		template<class BLOCK>
		static void Sti(BLOCK& block) {
			common::write_Opcode(block, 0xFB);
		}
#pragma endregion

#pragma region String operations

		template<RepPrefix R = NONE, class BLOCK>
		static void Insb(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0x6C)
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Insw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Rep<R>(block, 0x6D);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Insd(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0x6D);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Outsb(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0x6E);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Outsw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Rep<R>(block, 0x6F);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Outsd(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0x6F);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Lodsb(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xAC);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Lodsw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Rep<R>(block, 0xAD);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Lodsd(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xAD);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Movsb(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xA4);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Movsw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0xA5);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Movsd(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xA5);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Scasb(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xAE);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Scasw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Rep<R>(block, 0xAF);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Scasd(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xAF);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Stosb(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xAA);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Stosw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Rep<R>(block, 0xAB);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Stosd(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xAB);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Cmpsb(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xA6);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Cmpsw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Rep<R>(block, 0xA7);
		}

		template<RepPrefix R = NONE, class BLOCK>
		static void Cmpsd(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xA7);
		}

#pragma endregion

#pragma region Load segment registers
		template<class REG, AddressMode MODE, class BLOCK>
		static void Lds(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			template_LoadSegReg(block, 0xC5, reg, mem);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static void Les(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			template_LoadSegReg(block, 0xC4, reg, mem);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static void Lfs(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			template_LoadSegRegEx(block, 0xB4, reg, mem);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static void Lgs(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			template_LoadSegRegEx(block, 0xB5, reg, mem);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static void Lss(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			template_LoadSegRegEx(block, 0xB2, reg, mem);
		}
#pragma endregion

#pragma region Other operations
		/* TODO: Lock processor memory bus */
		template<class BLOCK>
		static void Lock(BLOCK& block) {
			common::write_Opcode(block, 0xF0);
		}

		template<AddressMode MODE, class BLOCK>
		static void Bound(BLOCK& block, Reg16 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x62);
			mem.write(block, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static void Bound(BLOCK& block, Reg32 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, 0x62);
			mem.write(block, reg);
		}

		/* No operation */
		template<class BLOCK>
		static void Nop(BLOCK& block) {
			common::write_Opcode(block, 0x90);
		}

		/* No operation */
		template<class BLOCK>
		static void Nop(BLOCK& block, Size count) {
			for (; count > 0; count--) {
				common::write_Opcode(block, 0x90);
			}
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

		/* Halt processor */
		template<class BLOCK>
		static void Hlt(BLOCK& block) {
			common::write_Opcode(block, 0xF4);
		}
#pragma endregion

#pragma region Priviledged system instructions
		/* Adjust RPL Field of Segment Selector
		 - ARPL reg,reg 
		*/
		template<class BLOCK>
		static void Arpl(BLOCK& block, Reg16 reg1, Reg16 reg2) {
			common::write_Opcode(block, 0x63);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg2, reg1);
		}

		/* Adjust RPL Field of Segment Selector 
		 - ARPL [mem],reg 
		*/
		template<AddressMode MODE, class BLOCK>
		static void Arpl(BLOCK& block, const Mem32<MODE>& mem, Reg16 reg) {
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, 0x63);
			mem.write(block, reg);
		}

		/* Clears Task-Switched Flag in CR0. */
		template<class BLOCK>
		static void Clts(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x06);
		}

		template<class BLOCK>
		static void Lar(BLOCK& block, Reg16 reg1, Reg16 reg2) {
			template_2_operands_ext(block, 0x02, reg1, reg2);
		}

		template<AddressMode MODE, class BLOCK>
		static void Lar(BLOCK& block, Reg16 reg, const Mem32<MODE>& mem) {
			template_2_operands_ext(block, 0x02, reg, mem);
		}

		template<class BLOCK>
		static void Lar(BLOCK& block, Reg32 reg1, Reg32 reg2) {
			template_2_operands_ext(block, 0x02, reg1, reg2);
		}

		template<AddressMode MODE, class BLOCK>
		static void Lar(BLOCK& block, Reg32 reg, const Mem32<MODE>& mem) {
			template_2_operands_ext(block, 0x02, reg, mem);
		}

		/* Load mem into GDTR */
		template<AddressMode MODE, class BLOCK>
		static void Lgdt(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_LGDT, mem);
		}

		/* Load mem into LIDT */
		template<AddressMode MODE, class BLOCK>
		static void Lidt(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_LIDT, mem);
		}

		/* Load mem into LLDT */
		template<AddressMode MODE, class BLOCK>
		static void Lldt(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_LLDT, mem);
		}

		/* Load register into LLDT */
		template<class BLOCK>
		static void Lldt(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_LLDT, (Reg32)reg);
		}

		/* Load mem in machine status word of CR0 */
		template<AddressMode MODE, class BLOCK>
		static void Lmsw(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_LMSW, mem);
		}

		/* Load register in machine status word of CR0 */
		template<class BLOCK>
		static void Lmsw(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_LMSW, (Reg32)reg);
		}

		template<class BLOCK>
		static void Lsl(BLOCK& block, Reg16 reg1, Reg16 reg2) {
			template_2_operands_ext(block, 0x03, reg1, reg2);
		}

		template<AddressMode MODE, class BLOCK>
		static void Lsl(BLOCK& block, Reg16 reg, const Mem32<MODE>& mem) {
			template_2_operands_ext(block, 0x03, reg, mem);
		}

		template<class BLOCK>
		static void Lsl(BLOCK& block, Reg32 reg1, Reg32 reg2) {
			template_2_operands_ext(block, 0x03, reg1, reg2);
		}

		template<AddressMode MODE, class BLOCK>
		static void Lsl(BLOCK& block, Reg32 reg, const Mem32<MODE>& mem) {
			template_2_operands_ext(block, 0x03, reg, mem);
		}

		/* Load task register */
		template<AddressMode MODE, class BLOCK>
		static void Ltr(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_LTR, mem);
		}

		/* Load task register */
		template<class BLOCK>
		static void Ltr(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_LTR, (Reg32)reg);
		}

		/* Store global descriptor table */
		template<AddressMode MODE, class BLOCK>
		static void Sgdt(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_SGDT, mem);
		}

		/* Store interrupt descriptor table */
		template<AddressMode MODE, class BLOCK>
		static void Sidt(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_SIDT, mem);
		}

		/* Store local descriptor table */
		template<AddressMode MODE, class BLOCK>
		static void Sldt(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_SLDT, mem);
		}

		/* Store local descriptor table */
		template<class BLOCK>
		static void Sldt(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_SLDT, (Reg32)reg);
		}

		/* Store Machine Status Word */
		template<AddressMode MODE, class BLOCK>
		static void Smsw(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_SMSW, mem);
		}

		/* Store Machine Status Word */
		template<class BLOCK>
		static void Smsw(BLOCK& block, Reg32 reg) {
			template_1reg_ext_operand(block, detail::opcode_SMSW, reg);
		}

		/* Store Machine Status Word */
		template<class BLOCK>
		static void Smsw(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_SMSW, reg);
		}

		/* Store Task Register */
		template<AddressMode MODE, class BLOCK>
		static void Str(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_STR, mem);
		}

		/* Store Task Register */
		template<class BLOCK>
		static void Str(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_STR, (Reg32)reg);
		}

		/* Verify a Segment for Reading */
		template<AddressMode MODE, class BLOCK>
		static void Verr(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_VERR, mem);
		}

		/* Verify a Segment for Reading */
		template<class BLOCK>
		static void Verr(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_VERR, (Reg32)reg);
		}

		/* Verify a Segment for Writing */
		template<AddressMode MODE, class BLOCK>
		static void Verw(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_VERW, mem);
		}

		/* Verify a Segment for Writing */
		template<class BLOCK>
		static void Verw(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_VERW, (Reg32)reg);
		}
#pragma endregion
	};
}