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
		
		static constexpr bool isGeneralMemSize(MemSize size) {
			return size == BYTE_PTR || size == WORD_PTR || size == DWORD_PTR;
		}

		template<class BLOCK, class DST, class SRC>
		static auto write_MOD_REG_RM_ReplaceableRR(BLOCK& block, const DST& dst, const SRC& src) {
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
			return std::make_pair(ReplaceableReg<DST>(offset, common::MOD_REG_RM::REG_BIT_OFFSET),
				ReplaceableReg<SRC>(offset, common::MOD_REG_RM::RM_BIT_OFFSET));
		}

		template<class BLOCK, class DST, class SRC>
		static auto write_MOD_REG_RM_ReplaceableReverseRR(BLOCK& block, const DST& dst, const SRC& src) {
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, src, dst);
			return std::make_pair(ReplaceableReg<DST>(offset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableReg<SRC>(offset, common::MOD_REG_RM::REG_BIT_OFFSET));
		}

		template<class BLOCK, class REG>
		static ReplaceableReg<REG> write_MOD_REG_RM_ReplaceableReg(BLOCK& block, const REG& reg, uint8_t rm) {
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg, rm);
			return ReplaceableReg<REG>(offset, common::MOD_REG_RM::REG_BIT_OFFSET);
		}

		template<class BLOCK, class REG>
		static ReplaceableReg<REG> write_MOD_REG_RM_ReplaceableRm(BLOCK& block, uint8_t reg, const REG& rm) {
			Offset offset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg, rm);
			return ReplaceableReg<REG>(offset, common::MOD_REG_RM::RM_BIT_OFFSET);
		}

		template<class BLOCK, class IMM>
		static ReplaceableImmediate<typename IMM::type> write_Immediate_Replaceable(BLOCK& block, const IMM& imm) {
			Offset offset = block.getOffset();
			common::write_Immediate(block, imm);
			return ReplaceableImmediate<typename IMM::type>(offset);
		}

		template<MemSize SIZE, class BLOCK>
		struct OpcodeWriter;

		template<class BLOCK>
		struct OpcodeWriter<BYTE_PTR, BLOCK> {
			template<LockPrefix L = NO_LOCK>
			static constexpr void write(BLOCK& block, common::Opcode opcode) {
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode(block, opcode);
			}

			template<LockPrefix L = NO_LOCK>
			static constexpr void write(BLOCK& block, common::Opcode opcode, OpMode rmMode) {
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode(block, opcode | (rmMode << 1));
			}

			template<LockPrefix L = NO_LOCK>
			static constexpr void writeOnlyExtendedPrefixs(BLOCK& block, common::Opcode opcode) {
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode);
			}

			template<LockPrefix L = NO_LOCK>
			static constexpr void writeExtended(BLOCK& block, common::Opcode opcode) {
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode);
			}

			static constexpr void writeWithReg(BLOCK& block, common::Opcode opcode, uint8_t reg) {
				common::write_Opcode(block, opcode | reg);
			}
		};

		template<class BLOCK>
		struct OpcodeWriter<WORD_PTR, BLOCK> {
			template<LockPrefix L = NO_LOCK>
			static constexpr void write(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_16bit_Prefix(block);
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode(block, opcode | 0b1);
			}

			template<LockPrefix L = NO_LOCK>
			static constexpr void write(BLOCK& block, common::Opcode opcode, OpMode rmMode) {
				common::write_Opcode_16bit_Prefix(block);
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode(block, opcode | (rmMode << 1) | 0b1);
			}

			template<LockPrefix L = NO_LOCK>
			static constexpr void writeOnlyExtendedPrefixs(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_16bit_Prefix(block);
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode);
			}

			template<LockPrefix L = NO_LOCK>
			static constexpr void writeExtended(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_16bit_Prefix(block);
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode | 0b1);
			}

			static constexpr void writeWithReg(BLOCK& block, common::Opcode opcode, uint8_t reg) {
				common::write_Opcode_16bit_Prefix(block);
				common::write_Opcode(block, opcode | 0x08 | reg);
			}
		};

		template<class BLOCK>
		struct OpcodeWriter<DWORD_PTR, BLOCK> {
			template<LockPrefix L = NO_LOCK>
			static constexpr void write(BLOCK& block, common::Opcode opcode) {
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode(block, opcode | 0b1);
			}

			template<LockPrefix L = NO_LOCK>
			static constexpr void write(BLOCK& block, common::Opcode opcode, OpMode rmMode) {
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode(block, opcode | (rmMode << 1) | 0b1);
			}

			template<LockPrefix L = NO_LOCK>
			static constexpr void writeOnlyExtendedPrefixs(BLOCK& block, common::Opcode opcode) {
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode);
			}

			template<LockPrefix L = NO_LOCK>
			static constexpr void writeExtended(BLOCK& block, common::Opcode opcode) {
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode | 0b1);
			}

			static constexpr void writeWithReg(BLOCK& block, common::Opcode opcode, uint8_t reg) {
				common::write_Opcode(block, opcode | 0x08 | reg);
			}
		};

		template<MemSize SIZE, LockPrefix L = NO_LOCK, class BLOCK>
		static constexpr void write_Opcode(BLOCK& block, common::Opcode opcode) {
			OpcodeWriter<SIZE, BLOCK>::write<L>(block, opcode);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, class BLOCK>
		static constexpr void write_Opcode(BLOCK& block, common::Opcode opcode, OpMode rmMode) {
			OpcodeWriter<SIZE, BLOCK>::write<L>(block, opcode, rmMode);
		}

		template<class BLOCK>
		static constexpr void write_Opcode_Only_RM_Mode(BLOCK& block, common::Opcode opcode, OpMode rmMode) {
			common::write_Opcode(block, opcode | (rmMode << 1));
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, class BLOCK>
		static constexpr void write_Opcode_Only_Extended_Prefixs(BLOCK& block, common::Opcode opcode) {
			OpcodeWriter<SIZE, BLOCK>::writeOnlyExtendedPrefixs<L>(block, opcode);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, class BLOCK>
		static constexpr void write_Opcode_Extended(BLOCK& block, common::Opcode opcode) {
			OpcodeWriter<SIZE, BLOCK>::writeExtended<L>(block, opcode);
		}

		template<MemSize SIZE, class BLOCK>
		static constexpr void write_Opcode_With_Reg(BLOCK& block, common::Opcode opcode, uint8_t reg) {
			OpcodeWriter<SIZE, BLOCK>::writeWithReg(block, opcode, reg);
		}

		template<MemSize SIZE, MemSize OPT_SIZE, class BLOCK>
		struct OpcodeImmOptimizedWriter;

		template<class BLOCK>
		struct OpcodeImmOptimizedWriter<BYTE_PTR, BYTE_PTR, BLOCK> {
			template<LockPrefix L = NO_LOCK>
			static constexpr void write(BLOCK& block, common::Opcode opcode) {
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode(block, opcode);
			}
		};

		template<class BLOCK>
		struct OpcodeImmOptimizedWriter<WORD_PTR, WORD_PTR, BLOCK> {
			template<LockPrefix L = NO_LOCK>
			static constexpr void write(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_16bit_Prefix(block);
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode(block, opcode | 0b1);
			}
		};

		template<class BLOCK>
		struct OpcodeImmOptimizedWriter<WORD_PTR, BYTE_PTR, BLOCK> {
			template<LockPrefix L = NO_LOCK>
			static constexpr void write(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_16bit_Prefix(block);
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode(block, opcode | 0b11);
			}
		};

		template<class BLOCK>
		struct OpcodeImmOptimizedWriter<DWORD_PTR, DWORD_PTR, BLOCK> {
			template<LockPrefix L = NO_LOCK>
			static constexpr void write(BLOCK& block, common::Opcode opcode) {
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode(block, opcode | 0b1);
			}
		};

		template<class BLOCK>
		struct OpcodeImmOptimizedWriter<DWORD_PTR, WORD_PTR, BLOCK> {
			template<LockPrefix L = NO_LOCK>
			static constexpr void write(BLOCK& block, common::Opcode opcode) {
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode(block, opcode | 0b1);
			}
		};

		template<class BLOCK>
		struct OpcodeImmOptimizedWriter<DWORD_PTR, BYTE_PTR, BLOCK> {
			template<LockPrefix L = NO_LOCK>
			static constexpr void write(BLOCK& block, common::Opcode opcode) {
				common::write_Lock_Prefix<L>(block);
				common::write_Opcode(block, opcode | 0b11);
			}
		};

		template<MemSize SIZE, MemSize OPT_SIZE, LockPrefix L = NO_LOCK, class BLOCK>
		static constexpr void write_Opcode_Imm_Optimized(BLOCK& block, common::Opcode opcode) {
			OpcodeImmOptimizedWriter<SIZE, OPT_SIZE, BLOCK>::write<L>(block, opcode);
		}

		template<MemSize SIZE, class T, class BLOCK>
		static constexpr void write_Imm_Size_Extend(BLOCK& block, const Imm<T>& imm) {
			block.template pushRaw<byteOrder>(static_cast<typename detail::ImmSizeExtend<SIZE, T>::type>(imm));
		}

		template<MemSize SIZE, class T, class BLOCK>
		static constexpr void write_Imm_Size_Optimize(BLOCK& block, const Imm<T>& imm) {
			block.template pushRaw<byteOrder>(static_cast<typename detail::ImmSizeOptimize<SIZE, T>::type>(imm));
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static constexpr void template_1mem_operand(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE, L>(block, opcode.getOpcode());
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> template_1mem_operand_r(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE, L>(block, opcode.getOpcode());
			return mem.writeReplaceable(block, opcode.getMode());
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static constexpr void template_1mem_ext_operand(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<SIZE>(block, opcode.getOpcode());
			mem.write(block, opcode.getMode());
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> template_1mem_ext_operand_r(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<SIZE>(block, opcode.getOpcode());
			return mem.writeReplaceable(block, opcode.getMode());
		}

		template<class REG, class BLOCK>
		struct Instruction1RegWriter;

		template<class BLOCK>
		struct Instruction1RegWriter<Reg8, BLOCK> {
			static constexpr void write(BLOCK& block, const detail::OpcodeLarge& opcode, Reg8 reg) {
				write_Opcode<BYTE_PTR>(block, opcode.getOpcode());
				common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			}

			static ReplaceableReg<Reg8> writeReplaceable(BLOCK& block, const detail::OpcodeLarge& opcode, Reg8 reg) {
				write_Opcode<BYTE_PTR>(block, opcode.getOpcode());
				return write_MOD_REG_RM_ReplaceableRm(block, opcode.getMode(), reg);
			}

			static constexpr void writeExtended(BLOCK& block, const detail::OpcodeLarge& opcode, Reg8 reg) {
				write_Opcode_Only_Extended_Prefixs<BYTE_PTR>(block, opcode.getOpcode());
				common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			}

			static ReplaceableReg<Reg8> writeExtendedReplaceable(BLOCK& block, const detail::OpcodeLarge& opcode, Reg8 reg) {
				write_Opcode_Only_Extended_Prefixs<BYTE_PTR>(block, opcode.getOpcode());
				return write_MOD_REG_RM_ReplaceableRm(block, opcode.getMode(), reg);
			}
		};

		template<class BLOCK>
		struct Instruction1RegWriter<Reg16, BLOCK> {
			static constexpr void write(BLOCK& block, const detail::OpcodeLarge& opcode, Reg16 reg) {
				write_Opcode<WORD_PTR>(block, opcode.getOpcode());
				common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			}
			
			static ReplaceableReg<Reg16> writeReplaceable(BLOCK& block, const detail::OpcodeLarge& opcode, Reg16 reg) {
				write_Opcode<WORD_PTR>(block, opcode.getOpcode());
				return write_MOD_REG_RM_ReplaceableRm(block, opcode.getMode(), reg);
			}

			static constexpr void writeExtended(BLOCK& block, const detail::OpcodeLarge& opcode, Reg16 reg) {
				write_Opcode_Only_Extended_Prefixs<WORD_PTR>(block, opcode.getOpcode());
				common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			}

			static ReplaceableReg<Reg16> writeExtendedReplaceable(BLOCK& block, const detail::OpcodeLarge& opcode, Reg16 reg) {
				write_Opcode_Only_Extended_Prefixs<WORD_PTR>(block, opcode.getOpcode());
				return write_MOD_REG_RM_ReplaceableRm(block, opcode.getMode(), reg);
			}
		};

		template<class BLOCK>
		struct Instruction1RegWriter<Reg32, BLOCK> {
			static constexpr void write(BLOCK& block, const detail::OpcodeLarge& opcode, Reg32 reg) {
				write_Opcode<DWORD_PTR>(block, opcode.getOpcode());
				common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			}
			
			static ReplaceableReg<Reg32> writeReplaceable(BLOCK& block, const detail::OpcodeLarge& opcode, Reg32 reg) {
				write_Opcode<DWORD_PTR>(block, opcode.getOpcode());
				return write_MOD_REG_RM_ReplaceableRm(block, opcode.getMode(), reg);
			}

			static constexpr void writeExtended(BLOCK& block, const detail::OpcodeLarge& opcode, Reg32 reg) {
				write_Opcode_Only_Extended_Prefixs<DWORD_PTR>(block, opcode.getOpcode());
				common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			}

			static ReplaceableReg<Reg32> writeExtendedReplaceable(BLOCK& block, const detail::OpcodeLarge& opcode, Reg32 reg) {
				write_Opcode_Only_Extended_Prefixs<DWORD_PTR>(block, opcode.getOpcode());
				return write_MOD_REG_RM_ReplaceableRm(block, opcode.getMode(), reg);
			}
		};

		template<class REG, class BLOCK>
		static constexpr void template_1reg_operand(BLOCK& block, const detail::OpcodeLarge& opcode, REG reg) {
			Instruction1RegWriter<REG, BLOCK>::write(block, opcode, reg);
		}

		template<class REG, class BLOCK>
		static ReplaceableReg<REG> template_1reg_operand_r(BLOCK& block, const detail::OpcodeLarge& opcode, REG reg) {
			return Instruction1RegWriter<REG, BLOCK>::writeReplaceable(block, opcode, reg);
		}

		template<class REG, class BLOCK>
		static constexpr void template_1reg_ext_operand(BLOCK& block, const detail::OpcodeLarge& opcode, REG reg) {
			Instruction1RegWriter<REG, BLOCK>::writeExtended(block, opcode, reg);
		}

		template<class REG, class BLOCK>
		static ReplaceableReg<REG> template_1reg_ext_operand_r(BLOCK& block, const detail::OpcodeLarge& opcode, REG reg) {
			return Instruction1RegWriter<REG, BLOCK>::writeExtendedReplaceable(block, opcode, reg);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static constexpr void template_mem_imm_operands(BLOCK& block, const detail::OpcodeSet& opcodeSet, const Mem32<MODE>& mem, const Imm<T>& imm) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcodeSet.getSecond().getOpcode());
			mem.write(block, opcodeSet.getSecond().getMode());
			write_Imm_Size_Extend<SIZE>(block, imm);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableExtendedImm<SIZE, T>> template_mem_imm_operands_r(BLOCK& block, const detail::OpcodeSet& opcodeSet, const Mem32<MODE>& mem, const Imm<T>& imm) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcodeSet.getSecond().getOpcode());
			auto replaceMem = mem.writeReplaceable(block, opcodeSet.getSecond().getMode());
			Offset offset = block.getOffset();
			write_Imm_Size_Extend<SIZE>(block, imm);
			return std::make_pair(replaceMem, ReplaceableExtendedImm<SIZE, T>(offset));
		}

		template<MemSize SIZE, LockPrefix L, AddressMode MODE, class T, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableOptimizedImm<SIZE, T>> template_mem_imm_opt_operands_r(BLOCK& block, const detail::OpcodeSet& opcodeSet, const Mem32<MODE>& mem, const Imm<T>& imm) {
			mem.writeSegmPrefix(block);
			write_Opcode_Imm_Optimized<SIZE, TypeMemSize<Imm<T>>::value, L>(block, opcodeSet.getSecond().getOpcode());
			auto replaceMem = mem.writeReplaceable(block, opcodeSet.getSecond().getMode());
			Offset offset = block.getOffset();
			write_Imm_Size_Optimize<SIZE>(block, imm);
			return std::make_pair(replaceMem, ReplaceableOptimizedImm<SIZE, T>(offset));
		}

		template<MemSize SIZE, LockPrefix L, AddressMode MODE, class T, class BLOCK>
		static constexpr void template_mem_imm_opt_operands(BLOCK& block, const detail::OpcodeSet& opcodeSet, const Mem32<MODE>& mem, const Imm<T>& imm) {
			mem.writeSegmPrefix(block);
			write_Opcode_Imm_Optimized<SIZE, TypeMemSize<Imm<T>>::value, L>(block, opcodeSet.getSecond().getOpcode());
			mem.write(block, opcodeSet.getSecond().getMode());
			write_Imm_Size_Optimize<SIZE>(block, imm);
		}

		template<class REG, class T, class BLOCK>
		static constexpr void template_reg_imm_opt_operands(BLOCK& block, const detail::OpcodeSet& opcodeSet, const REG& reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "i386: First param must be register");
			write_Opcode_Imm_Optimized<TypeMemSize<REG>::value, TypeMemSize<Imm<T>>::value>(block, opcodeSet.getSecond().getOpcode());
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcodeSet.getSecond().getMode(), reg);
			write_Imm_Size_Optimize<TypeMemSize<REG>::value>(block, imm);
		}

		template<class REG, class T, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableOptimizedImm<TypeMemSize<REG>::value, T>> template_reg_imm_opt_operands_r(BLOCK& block, const detail::OpcodeSet& opcodeSet, const REG& reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "i386: First param must be register");
			write_Opcode_Imm_Optimized<TypeMemSize<REG>::value, TypeMemSize<Imm<T>>::value>(block, opcodeSet.getSecond().getOpcode());
			Offset modOffset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcodeSet.getSecond().getMode(), reg);
			Offset offset = block.getOffset();
			write_Imm_Size_Optimize<TypeMemSize<REG>::value>(block, imm);
			return std::make_pair(ReplaceableReg<REG>(modOffset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableOptimizedImm<TypeMemSize<REG>::value, T>(offset));
		}

		template<class REG, class T, class BLOCK>
		static constexpr void template_reg_imm_operands(BLOCK& block, const detail::OpcodeSet& opcodeSet, const REG& reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "i386: First param must be register");
			write_Opcode<TypeMemSize<REG>::value>(block, opcodeSet.getSecond().getOpcode());
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcodeSet.getSecond().getMode(), reg);
			write_Imm_Size_Extend<TypeMemSize<REG>::value>(block, imm);
		}

		template<class REG, class T, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableExtendedImm<TypeMemSize<REG>::value, T>> template_reg_imm_operands_r(BLOCK& block, const detail::OpcodeSet& opcodeSet, const REG& reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "i386: First param must be register");
			write_Opcode<TypeMemSize<REG>::value>(block, opcodeSet.getSecond().getOpcode());
			Offset regOffset = block.getOffset();
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcodeSet.getSecond().getMode(), reg);
			Offset offset = block.getOffset();
			write_Imm_Size_Extend<TypeMemSize<REG>::value>(block, imm);
			return std::make_pair(ReplaceableReg<REG>(regOffset, common::MOD_REG_RM::RM_BIT_OFFSET),
				ReplaceableExtendedImm<TypeMemSize<REG>::value, T>(offset));
		}

		template<class REG, class T, class BLOCK>
		static constexpr void template_reg_imm_short_operands(BLOCK& block, common::Opcode opcode, const REG& reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "i386: First param must be register");
			write_Opcode_With_Reg<TypeMemSize<REG>::value>(block, opcode, reg);
			write_Imm_Size_Extend<TypeMemSize<REG>::value>(block, imm);
		}

		template<class REG, class T, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableExtendedImm<TypeMemSize<REG>::value, T>> template_reg_imm_short_operands_r(BLOCK& block, common::Opcode opcode, const REG& reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "i386: First param must be register");
			Offset regOffset = block.getOffset();
			write_Opcode_With_Reg<TypeMemSize<REG>::value>(block, opcode, reg);
			Offset offset = block.getOffset();
			write_Imm_Size_Extend<TypeMemSize<REG>::value>(block, imm);
			return std::make_pair(ReplaceableReg<REG>(regOffset, 0),
				ReplaceableExtendedImm<TypeMemSize<REG>::value, T>(offset));
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static constexpr void template_shift_operands(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcode.getOpcode() | 0x10);
			mem.write(block, opcode.getMode());
		}

		template<class REG, class BLOCK>
		static constexpr void template_shift_operands(BLOCK& block, const detail::OpcodeLarge& opcode, const REG& reg) {
			write_Opcode<TypeMemSize<REG>::value>(block, opcode.getOpcode() | 0x10);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
		}

		template<MemSize SIZE, Reg8 SH_REG, AddressMode MODE, class BLOCK>
		static constexpr void template_shift_operands(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			static_assert(SH_REG == CL, "i386: Invalid register for shift operation");
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcode.getOpcode() | 0x12);
			mem.write(block, opcode.getMode());
		}

		template<class REG, Reg8 SH_REG, class BLOCK>
		static constexpr void template_shift_operands(BLOCK& block, const detail::OpcodeLarge& opcode, const REG& reg) {
			static_assert(SH_REG == CL, "i386: Invalid register for shift operation");
			write_Opcode<TypeMemSize<REG>::value>(block, opcode.getOpcode() | 0x12);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static constexpr void template_shift_operands(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem, const U8& imm) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcode.getOpcode());
			mem.write(block, opcode.getMode());
			common::write_Immediate(block, imm);
		}

		template<class REG, class BLOCK>
		static constexpr void template_shift_operands(BLOCK& block, const detail::OpcodeLarge& opcode, const REG& reg, const U8& imm) {
			write_Opcode<TypeMemSize<REG>::value>(block, opcode.getOpcode());
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
			common::write_Immediate(block, imm);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> template_shift_operands_r(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcode.getOpcode() | 0x10);
			return mem.writeReplaceable(block, opcode.getMode());
		}

		template<class REG, class BLOCK>
		static ReplaceableReg<REG> template_shift_operands_r(BLOCK& block, const detail::OpcodeLarge& opcode, const REG& reg) {
			write_Opcode<TypeMemSize<REG>::value>(block, opcode.getOpcode() | 0x10);
			return write_MOD_REG_RM_ReplaceableRm(block, opcode.getMode(), reg);
		}

		template<MemSize SIZE, Reg8 SH_REG, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> template_shift_operands_r(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem) {
			static_assert(SH_REG == CL, "i386: Invalid register for shift operation");
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcode.getOpcode() | 0x12);
			return mem.writeReplaceable(block, opcode.getMode());
		}

		template<class REG, Reg8 SH_REG, class BLOCK>
		static ReplaceableReg<REG> template_shift_operands_r(BLOCK& block, const detail::OpcodeLarge& opcode, const REG& reg) {
			static_assert(SH_REG == CL, "i386: Invalid register for shift operation");
			write_Opcode<TypeMemSize<REG>::value>(block, opcode.getOpcode() | 0x12);
			return write_MOD_REG_RM_ReplaceableRm(block, opcode.getMode(), reg);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableImmediate<U8::type>> template_shift_operands_r(BLOCK& block, const detail::OpcodeLarge& opcode, const Mem32<MODE>& mem, const U8& imm) {
			mem.writeSegmPrefix(block);
			write_Opcode<SIZE>(block, opcode.getOpcode());
			auto replaceMem = mem.writeReplaceable(block, opcode.getMode());
			auto replaceImm = write_Immediate_Replaceable(block, imm);
			return std::make_pair(replaceMem, replaceImm);
		}

		template<class REG, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableImmediate<U8::type>> template_shift_operands_r(BLOCK& block, const detail::OpcodeLarge& opcode, const REG& reg, const U8& imm) {
			write_Opcode<TypeMemSize<REG>::value>(block, opcode.getOpcode());
			auto replaceReg = write_MOD_REG_RM_ReplaceableRm(block, opcode.getMode(), reg);
			auto replaceImm = write_Immediate_Replaceable(block, imm);
			return std::make_pair(replaceReg, replaceImm);
		}

		template<MemSize SIZE, Reg8 COUNT_REG = CL, AddressMode MODE, class REG, class BLOCK>
		static constexpr void template_prshift_operands(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& mem, REG reg) {
			static_assert(IsRegType<REG>::value, "i386: Param must be register");
			static_assert(COUNT_REG == CL, "i386: Invalid register for precision shift operation");
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "i386: Invalid precision shift size prefix");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<SIZE>(block, opcode | 0x01);
			mem.write(block, reg);
		}

		template<Reg8 COUNT_REG = CL, class REG, class BLOCK>
		static constexpr void template_prshift_operands(BLOCK& block, common::Opcode opcode, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "i386: Param must be register");
			static_assert(COUNT_REG == CL, "i386: Invalid register for precision shift operation");
			static_assert(TypeMemSize<REG>::value == WORD_PTR || TypeMemSize<REG>::value == DWORD_PTR, "i386: Invalid precision shift size prefix");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode | 0x01);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg2, reg1);
		}

		template<MemSize SIZE, AddressMode MODE, class REG, class BLOCK>
		static constexpr void template_prshift_operands(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& mem, REG reg, const U8& imm) {
			static_assert(IsRegType<REG>::value, "i386: Param must be register");
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "i386: Invalid precision shift size prefix");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<SIZE>(block, opcode);
			mem.write(block, reg);
			common::write_Immediate(block, imm);
		}

		template<class REG, class BLOCK>
		static constexpr void template_prshift_operands(BLOCK& block, common::Opcode opcode, REG reg1, REG reg2, const U8& imm) {
			static_assert(IsRegType<REG>::value, "i386: Param must be register");
			static_assert(TypeMemSize<REG>::value == WORD_PTR || TypeMemSize<REG>::value == DWORD_PTR, "Invalid precision shift size prefix");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg2, reg1);
			common::write_Immediate(block, imm);
		}

		template<MemSize SIZE, Reg8 COUNT_REG = CL, AddressMode MODE, class REG, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableReg<REG>> template_prshift_operands_r(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& mem, REG reg) {
			static_assert(IsRegType<REG>::value, "i386: Param must be register");
			static_assert(COUNT_REG == CL, "i386: Invalid register for precision shift operation");
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "i386: Invalid precision shift size prefix");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<SIZE>(block, opcode | 0x01);
			auto replaceMem = mem.writeReplaceable(block, reg);
			return std::make_pair(replaceMem, replaceMem.template getOtherReg<REG>());
		}

		template<Reg8 COUNT_REG = CL, class REG, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableReg<REG>> template_prshift_operands_r(BLOCK& block, common::Opcode opcode, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "i386: Param must be register");
			static_assert(COUNT_REG == CL, "i386: Invalid register for precision shift operation");
			static_assert(TypeMemSize<REG>::value == WORD_PTR || TypeMemSize<REG>::value == DWORD_PTR, "i386: Invalid precision shift size prefix");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode | 0x01);
			return write_MOD_REG_RM_ReplaceableReverseRR(block, reg1, reg2);
		}

		template<MemSize SIZE, AddressMode MODE, class REG, class BLOCK>
		static std::tuple<ReplaceableMem32<MODE>, ReplaceableReg<REG>, ReplaceableImmediate<U8::type>> template_prshift_operands_r(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& mem, REG reg, const U8& imm) {
			static_assert(IsRegType<REG>::value, "i386: Param must be register");
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "i386: Invalid precision shift size prefix");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<SIZE>(block, opcode);
			auto replaceMem = mem.writeReplaceable(block, reg);
			auto replaceImm = write_Immediate_Replaceable(block, imm);
			return std::make_tuple(replaceMem, replaceMem.template getOtherReg<REG>(), replaceImm);
		}

		template<class REG, class BLOCK>
		static std::tuple<ReplaceableReg<REG>, ReplaceableReg<REG>, ReplaceableImmediate<U8::type>> template_prshift_operands_r(BLOCK& block, common::Opcode opcode, REG reg1, REG reg2, const U8& imm) {
			static_assert(IsRegType<REG>::value, "i386: Param must be register");
			static_assert(TypeMemSize<REG>::value == WORD_PTR || TypeMemSize<REG>::value == DWORD_PTR, "Invalid precision shift size prefix");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode);
			auto replaceRegs = write_MOD_REG_RM_ReplaceableReverseRR(block, reg1, reg2);
			auto replaceImm = write_Immediate_Replaceable(block, imm);
			return std::make_tuple(replaceRegs.first, replaceRegs.second, replaceImm);
		}

		template<LockPrefix L = NO_LOCK, class BLOCK>
		static constexpr void template_2operands(BLOCK& block, common::Opcode opcode, const Reg8& dst, const Reg8& src) {
			write_Opcode<BYTE_PTR>(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, src, dst);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static constexpr void template_2operands(BLOCK& block, common::Opcode opcode, const Reg8& dst, const Mem32<MODE>& src) {
			src.writeSegmPrefix(block);
			write_Opcode<BYTE_PTR>(block, opcode, MODE_RM);
			src.write(block, dst);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static constexpr void template_2operands(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg8& src) {
			dst.writeSegmPrefix(block);
			write_Opcode<BYTE_PTR, L>(block, opcode, MODE_MR);
			dst.write(block, src);
		}

		template<LockPrefix L = NO_LOCK, class BLOCK>
		static constexpr void template_2operands(BLOCK& block, common::Opcode opcode, const Reg16& dst, const Reg16& src) {
			write_Opcode<WORD_PTR>(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, src, dst);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static constexpr void template_2operands(BLOCK& block, common::Opcode opcode, const Reg16& dst, const Mem32<MODE>& src) {
			src.writeSegmPrefix(block);
			write_Opcode<WORD_PTR>(block, opcode, MODE_RM);
			src.write(block, dst);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static constexpr void template_2operands(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg16& src) {
			dst.writeSegmPrefix(block);
			write_Opcode<WORD_PTR, L>(block, opcode, MODE_MR);
			dst.write(block, src);
		}

		template<LockPrefix L = NO_LOCK, class BLOCK>
		static constexpr void template_2operands(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Reg32& src) {
			write_Opcode<DWORD_PTR>(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, src, dst);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static constexpr void template_2operands(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Mem32<MODE>& src) {
			src.writeSegmPrefix(block);
			write_Opcode<DWORD_PTR>(block, opcode, MODE_RM);
			src.write(block, dst);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static constexpr void template_2operands(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg32& src) {
			dst.writeSegmPrefix(block);
			write_Opcode<DWORD_PTR, L>(block, opcode, MODE_MR);
			dst.write(block, src);
		}

		template<LockPrefix L = NO_LOCK, class BLOCK>
		static std::pair<ReplaceableReg<Reg8>, ReplaceableReg<Reg8>> template_2operands_r(BLOCK& block, common::Opcode opcode, const Reg8& dst, const Reg8& src) {
			write_Opcode<BYTE_PTR>(block, opcode);
			return write_MOD_REG_RM_ReplaceableReverseRR(block, dst, src);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableReg<Reg8>, ReplaceableMem32<MODE>> template_2operands_r(BLOCK& block, common::Opcode opcode, const Reg8& dst, const Mem32<MODE>& src) {
			src.writeSegmPrefix(block);
			write_Opcode<BYTE_PTR>(block, opcode, MODE_RM);
			auto replaceMem = src.writeReplaceable(block, dst);
			return std::make_pair(replaceMem.template getOtherReg<Reg8>(), replaceMem);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableReg<Reg8>> template_2operands_r(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg8& src) {
			dst.writeSegmPrefix(block);
			write_Opcode<BYTE_PTR, L>(block, opcode, MODE_MR);
			auto replaceMem = dst.writeReplaceable(block, src);
			return std::make_pair(replaceMem, replaceMem.template getOtherReg<Reg8>());
		}

		template<LockPrefix L = NO_LOCK, class BLOCK>
		static std::pair<ReplaceableReg<Reg16>, ReplaceableReg<Reg16>> template_2operands_r(BLOCK& block, common::Opcode opcode, const Reg16& dst, const Reg16& src) {
			write_Opcode<WORD_PTR>(block, opcode);
			return write_MOD_REG_RM_ReplaceableReverseRR(block, dst, src);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableReg<Reg16>, ReplaceableMem32<MODE>> template_2operands_r(BLOCK& block, common::Opcode opcode, const Reg16& dst, const Mem32<MODE>& src) {
			src.writeSegmPrefix(block);
			write_Opcode<WORD_PTR>(block, opcode, MODE_RM);
			auto replaceMem = src.writeReplaceable(block, dst);
			return std::make_pair(replaceMem.template getOtherReg<Reg16>(), replaceMem);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableReg<Reg16>> template_2operands_r(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg16& src) {
			dst.writeSegmPrefix(block);
			write_Opcode<WORD_PTR, L>(block, opcode, MODE_MR);
			auto replaceMem = dst.writeReplaceable(block, src);
			return std::make_pair(replaceMem, replaceMem.template getOtherReg<Reg16>());
		}

		template<LockPrefix L = NO_LOCK, class BLOCK>
		static std::pair<ReplaceableReg<Reg32>, ReplaceableReg<Reg32>> template_2operands_r(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Reg32& src) {
			write_Opcode<DWORD_PTR>(block, opcode);
			return write_MOD_REG_RM_ReplaceableReverseRR(block, dst, src);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableReg<Reg32>, ReplaceableMem32<MODE>> template_2operands_r(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Mem32<MODE>& src) {
			src.writeSegmPrefix(block);
			write_Opcode<DWORD_PTR>(block, opcode, MODE_RM);
			auto replaceMem = src.writeReplaceable(block, dst);
			return std::make_pair(replaceMem.template getOtherReg<Reg32>(), replaceMem);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableReg<Reg32>> template_2operands_r(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg32& src) {
			dst.writeSegmPrefix(block);
			write_Opcode<DWORD_PTR, L>(block, opcode, MODE_MR);
			auto replaceMem = dst.writeReplaceable(block, src);
			return std::make_pair(replaceMem, replaceMem.template getOtherReg<Reg32>());
		}

		template<class BLOCK>
		static constexpr void template_2operands_segm(BLOCK& block, common::Opcode opcode, const Reg16& dst, const RegSeg& src) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, src, dst);
		}

		template<class BLOCK>
		static constexpr void template_2operands_segm(BLOCK& block, common::Opcode opcode, const RegSeg& dst, const Reg16& src) {
			write_Opcode_Only_RM_Mode(block, opcode, MODE_RR);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void template_2operands_segm(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const RegSeg& src) {
			dst.writeSegmPrefix(block);
			write_Opcode_Only_RM_Mode(block, opcode, MODE_MR);
			dst.write(block, src);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void template_2operands_segm(BLOCK& block, common::Opcode opcode, const RegSeg& dst, const Mem32<MODE>& src) {
			src.writeSegmPrefix(block);
			write_Opcode_Only_RM_Mode(block, opcode, MODE_RM);
			src.write(block, dst);
		}

		template<class BLOCK>
		static auto template_2operands_segm_r(BLOCK& block, common::Opcode opcode, const Reg16& dst, const RegSeg& src) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, opcode);
			return write_MOD_REG_RM_ReplaceableReverseRR(block, dst, src);
		}

		template<class BLOCK>
		static auto template_2operands_segm_r(BLOCK& block, common::Opcode opcode, const RegSeg& dst, const Reg16& src) {
			write_Opcode_Only_RM_Mode(block, opcode, MODE_RR);
			return write_MOD_REG_RM_ReplaceableRR(block, dst, src);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_2operands_segm_r(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const RegSeg& src) {
			dst.writeSegmPrefix(block);
			write_Opcode_Only_RM_Mode(block, opcode, MODE_MR);
			return dst.writeReplaceable(block, src);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_2operands_segm_r(BLOCK& block, common::Opcode opcode, const RegSeg& dst, const Mem32<MODE>& src) {
			src.writeSegmPrefix(block);
			write_Opcode_Only_RM_Mode(block, opcode, MODE_RM);
			return src.writeReplaceable(block, dst);
		}

		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static constexpr void template_2operands_opt_imm(BLOCK& block, const detail::OpcodeSet& opcodeSet, const DST& dst, const SRC& src) {
			template_2operands<L>(block, opcodeSet.getMain(), dst, src);
		}

		template<LockPrefix L = NO_LOCK, class T, class BLOCK>
		static constexpr void template_2operands_opt_imm(BLOCK& block, const detail::OpcodeSet& opcodeSet, const Reg32& dst, const Imm<T>& src) {
			template_reg_imm_opt_operands(block, opcodeSet, dst, src);
		}

		template<LockPrefix L = NO_LOCK, class T, class BLOCK>
		static constexpr void template_2operands_opt_imm(BLOCK& block, const detail::OpcodeSet& opcodeSet, const Reg16& dst, const Imm<T>& src) {
			template_reg_imm_opt_operands(block, opcodeSet, dst, src);
		}

		template<LockPrefix L = NO_LOCK, class T, class BLOCK>
		static constexpr void template_2operands_opt_imm(BLOCK& block, const detail::OpcodeSet& opcodeSet, const Reg8& dst, const Imm<T>& src) {
			template_reg_imm_opt_operands(block, opcodeSet, dst, src);
		}

		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static auto template_2operands_opt_imm_r(BLOCK& block, const detail::OpcodeSet& opcodeSet, const DST& dst, const SRC& src) {
			return template_2operands_r<L>(block, opcodeSet.getMain(), dst, src);
		}

		template<LockPrefix L = NO_LOCK, class T, class BLOCK>
		static auto template_2operands_opt_imm_r(BLOCK& block, const detail::OpcodeSet& opcodeSet, const Reg32& dst, const Imm<T>& src) {
			return template_reg_imm_opt_operands_r(block, opcodeSet, dst, src);
		}

		template<LockPrefix L = NO_LOCK, class T, class BLOCK>
		static auto template_2operands_opt_imm_r(BLOCK& block, const detail::OpcodeSet& opcodeSet, const Reg16& dst, const Imm<T>& src) {
			return template_reg_imm_opt_operands_r(block, opcodeSet, dst, src);
		}

		template<LockPrefix L = NO_LOCK, class T, class BLOCK>
		static auto template_2operands_opt_imm_r(BLOCK& block, const detail::OpcodeSet& opcodeSet, const Reg8& dst, const Imm<T>& src) {
			return template_reg_imm_opt_operands_r(block, opcodeSet, dst, src);
		}

		template<MemSize SIZE, class T, class BLOCK>
		static constexpr void template_2operands_first_reg_imm(BLOCK& block, common::Opcode opcode, const Imm<T>& imm) {
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "i386: Immediate size not equal output register size");
			write_Opcode<SIZE>(block, opcode);
			write_Imm_Size_Extend<SIZE>(block, imm);
		}

		template<MemSize SIZE, class T, class BLOCK>
		static auto template_2operands_first_reg_imm_r(BLOCK& block, common::Opcode opcode, const Imm<T>& imm) {
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "i386: Immediate size not equal output register size");
			write_Opcode<SIZE>(block, opcode);
			Offset offset = block.getOffset();
			write_Imm_Size_Extend<SIZE>(block, imm);
			return ReplaceableExtendedImm<TypeMemSize<Imm<T>>::value, T>(offset);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void template_2operands_symetric(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg8& src) {
			template_2operands(block, opcode, dst, src);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void template_2operands_symetric(BLOCK& block, common::Opcode opcode, const Reg8& dst, const Mem32<MODE>& src) {
			template_2operands(block, opcode, src, dst);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void template_2operands_symetric(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg16& src) {
			template_2operands(block, opcode, dst, src);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void template_2operands_symetric(BLOCK& block, common::Opcode opcode, const Reg16& dst, const Mem32<MODE>& src) {
			template_2operands(block, opcode, src, dst);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void template_2operands_symetric(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg32& src) {
			template_2operands(block, opcode, dst, src);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void template_2operands_symetric(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Mem32<MODE>& src) {
			template_2operands(block, opcode, src, dst);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_2operands_symetric_r(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg8& src) {
			return template_2operands_r(block, opcode, dst, src);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_2operands_symetric_r(BLOCK& block, common::Opcode opcode, const Reg8& dst, const Mem32<MODE>& src) {
			return template_2operands_r(block, opcode, src, dst);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_2operands_symetric_r(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg16& src) {
			return template_2operands_r(block, opcode, dst, src);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_2operands_symetric_r(BLOCK& block, common::Opcode opcode, const Reg16& dst, const Mem32<MODE>& src) {
			return template_2operands_r(block, opcode, src, dst);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_2operands_symetric_r(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, const Reg32& src) {
			return template_2operands_r(block, opcode, dst, src);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_2operands_symetric_r(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Mem32<MODE>& src) {
			return template_2operands_r(block, opcode, src, dst);
		}

		template<class BLOCK>
		static constexpr void template_2reg_operands_extend_val(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Reg16& src) {
			write_Opcode_Extended<TypeMemSize<Reg32>::value>(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
		}

		template<class BLOCK>
		static constexpr void template_2reg_operands_extend_val(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Reg8& src) {
			write_Opcode_Extended<TypeMemSize<Reg8>::value>(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
		}

		template<class BLOCK>
		static constexpr void template_2reg_operands_extend_val(BLOCK& block, common::Opcode opcode, const Reg16& dst, const Reg8& src) {
			common::write_Opcode_16bit_Prefix(block);
			write_Opcode_Extended<TypeMemSize<Reg8>::value>(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
		}

		template<class BLOCK>
		static std::pair<ReplaceableReg<Reg32>, ReplaceableReg<Reg16>> template_2reg_operands_extend_val_r(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Reg16& src) {
			write_Opcode_Extended<TypeMemSize<Reg32>::value>(block, opcode);
			return write_MOD_REG_RM_ReplaceableRR(block, dst, src);
		}

		template<class BLOCK>
		static std::pair<ReplaceableReg<Reg32>, ReplaceableReg<Reg8>> template_2reg_operands_extend_val_r(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Reg8& src) {
			write_Opcode_Extended<TypeMemSize<Reg8>::value>(block, opcode);
			return write_MOD_REG_RM_ReplaceableRR(block, dst, src);
		}

		template<class BLOCK>
		static std::pair<ReplaceableReg<Reg16>, ReplaceableReg<Reg8>> template_2reg_operands_extend_val_r(BLOCK& block, common::Opcode opcode, const Reg16& dst, const Reg8& src) {
			common::write_Opcode_16bit_Prefix(block);
			write_Opcode_Extended<TypeMemSize<Reg8>::value>(block, opcode);
			return write_MOD_REG_RM_ReplaceableRR(block, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static constexpr void template_2operands_extend_val(BLOCK& block, common::Opcode opcode, const Reg32& reg, const Mem32<MODE>& mem) {
			static_assert(SIZE == BYTE_PTR || SIZE == WORD_PTR, "Invalid extend mem source size");
			mem.writeSegmPrefix(block);
			common::write_Opcode_Extended_Prefix(block);
			/*constexpr*/ if (SIZE == WORD_PTR) {
				common::write_Opcode(block, opcode | 1);
			} else {
				common::write_Opcode(block, opcode);
			}
			mem.write(block, reg);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static constexpr void template_2operands_extend_val(BLOCK& block, common::Opcode opcode, const Reg16& reg, const Mem32<MODE>& mem) {
			static_assert(SIZE == BYTE_PTR || SIZE == WORD_PTR, "Invalid extend mem source size");
			mem.writeSegmPrefix(block);
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Extended_Prefix(block);
			/*constexpr*/ if (SIZE == WORD_PTR) {
				common::write_Opcode(block, opcode | 1);
			} else {
				common::write_Opcode(block, opcode);
			}
			mem.write(block, reg);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableReg<Reg32>, ReplaceableMem32<MODE>> template_2operands_extend_val_r(BLOCK& block, common::Opcode opcode, const Reg32& reg, const Mem32<MODE>& mem) {
			static_assert(SIZE == BYTE_PTR || SIZE == WORD_PTR, "Invalid extend mem source size");
			mem.writeSegmPrefix(block);
			common::write_Opcode_Extended_Prefix(block);
			/*constexpr*/ if (SIZE == WORD_PTR) {
				common::write_Opcode(block, opcode | 1);
			} else {
				common::write_Opcode(block, opcode);
			}
			auto replaceMem = mem.writeReplaceable(block, reg);
			return std::make_pair(replaceMem.template getOtherReg<Reg32>(), replaceMem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableReg<Reg16>, ReplaceableMem32<MODE>> template_2operands_extend_val_r(BLOCK& block, common::Opcode opcode, const Reg16& reg, const Mem32<MODE>& mem) {
			static_assert(SIZE == BYTE_PTR || SIZE == WORD_PTR, "Invalid extend mem source size");
			mem.writeSegmPrefix(block);
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Extended_Prefix(block);
			/*constexpr*/ if (SIZE == WORD_PTR) {
				common::write_Opcode(block, opcode | 1);
			} else {
				common::write_Opcode(block, opcode);
			}
			auto replaceMem = mem.writeReplaceable(block, reg);
			return std::make_pair(replaceMem.template getOtherReg<Reg16>(), replaceMem);
		}

		template<class BLOCK>
		static constexpr void template_2_operands_ext(BLOCK& block, common::Opcode opcode, Reg16 reg1, Reg16 reg2) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg1, reg2);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void template_2_operands_ext(BLOCK& block, common::Opcode opcode, Reg16 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			mem.write(block, reg);
		}

		template<class BLOCK>
		static constexpr void template_2_operands_ext(BLOCK& block, common::Opcode opcode, Reg32 reg1, Reg32 reg2) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg1, reg2);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void template_2_operands_ext(BLOCK& block, common::Opcode opcode, Reg32 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			mem.write(block, reg);
		}

		template<class REG, class BLOCK>
		static constexpr void template_bit_scan(BLOCK& block, common::Opcode opcode, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "i386: Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "i386: Invalid size of operand");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg1, reg2);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static constexpr void template_bit_scan(BLOCK& block, common::Opcode opcode, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "i386: Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "i386: Invalid size of operand");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode);
			mem.write(block, reg);
		}

		template<class REG, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableReg<REG>> template_bit_scan_r(BLOCK& block, common::Opcode opcode, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "i386: Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "i386: Invalid size of operand");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode);
			return write_MOD_REG_RM_ReplaceableRR(block, reg1, reg2);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableMem32<MODE>> template_bit_scan_r(BLOCK& block, common::Opcode opcode, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "i386: Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "i386: Invalid size of operand");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode);
			auto replaceMem = mem.writeReplaceable(block, reg);
			return std::make_pair(replaceMem.template getOtherReg<REG>(), replaceMem);
		}

		template<LockPrefix L = NO_LOCK, class REG, class BLOCK>
		static constexpr void template_bit_operation(BLOCK& block, const detail::OpcodeSet& opcode, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "i386: Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "i386: Invalid size of operand");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode.getMain());
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg2, reg1);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class REG, class BLOCK>
		static constexpr void template_bit_operation(BLOCK& block, const detail::OpcodeSet& opcode, const Mem32<MODE>& mem, REG reg) {
			static_assert(IsRegType<REG>::value, "i386: Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "i386: Invalid size of operand");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value, L>(block, opcode.getMain());
			mem.write(block, reg);
		}

		template<LockPrefix L = NO_LOCK, class REG, class BLOCK>
		static constexpr void template_bit_operation(BLOCK& block, const detail::OpcodeSet& opcode, REG reg, U8 imm) {
			static_assert(IsRegType<REG>::value, "i386: Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "i386: Invalid size of operand");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode.getSecond().getOpcode());
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getSecond().getMode(), reg);
			common::write_Immediate(block, imm);
		}

		template<LockPrefix L = NO_LOCK, MemSize SIZE, AddressMode MODE, class BLOCK>
		static constexpr void template_bit_operation(BLOCK& block, const detail::OpcodeSet& opcode, const Mem32<MODE>& mem, U8 imm) {
			static_assert(SIZE != BYTE_PTR, "i386: Invalid size of operand");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<SIZE, L>(block, opcode.getSecond().getOpcode());
			mem.write(block, opcode.getSecond().getMode());
			common::write_Immediate(block, imm);
		}

		template<LockPrefix L = NO_LOCK, class REG, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableReg<REG>> template_bit_operation_r(BLOCK& block, const detail::OpcodeSet& opcode, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "i386: Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "i386: Invalid size of operand");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode.getMain());
			return write_MOD_REG_RM_ReplaceableReverseRR(block, reg1, reg2);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class REG, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableReg<REG>> template_bit_operation_r(BLOCK& block, const detail::OpcodeSet& opcode, const Mem32<MODE>& mem, REG reg) {
			static_assert(IsRegType<REG>::value, "i386: Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "i386: Invalid size of operand");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value, L>(block, opcode.getMain());
			auto replaceMem = mem.writeReplaceable(block, reg);
			return std::make_pair(replaceMem, replaceMem.template getOtherReg<REG>());
		}

		template<LockPrefix L = NO_LOCK, class REG, class BLOCK>
		static std::pair<ReplaceableReg<REG>, ReplaceableImmediate<U8::type>> template_bit_operation_r(BLOCK& block, const detail::OpcodeSet& opcode, REG reg, U8 imm) {
			static_assert(IsRegType<REG>::value, "i386: Invalid type of operand");
			static_assert(TypeMemSize<REG>::value != BYTE_PTR, "i386: Invalid size of operand");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, opcode.getSecond().getOpcode());
			auto replaceReg = write_MOD_REG_RM_ReplaceableRm(block, opcode.getSecond().getMode(), reg);
			auto replaceImm = write_Immediate_Replaceable(block, imm);
			return std::make_pair(replaceReg, replaceImm);
		}

		template<LockPrefix L = NO_LOCK, MemSize SIZE, AddressMode MODE, class BLOCK>
		static std::pair<ReplaceableMem32<MODE>, ReplaceableImmediate<U8::type>> template_bit_operation_r(BLOCK& block, const detail::OpcodeSet& opcode, const Mem32<MODE>& mem, U8 imm) {
			static_assert(SIZE != BYTE_PTR, "i386: Invalid size of operand");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<SIZE, L>(block, opcode.getSecond().getOpcode());
			auto replaceMem = mem.writeReplaceable(block, opcode.getSecond().getMode());
			auto replaceImm = write_Immediate_Replaceable(block, imm);
			return std::make_pair(replaceMem, replaceImm);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg8> template_Setcc(BLOCK& block, common::Opcode opcode, const Reg8& reg) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			return write_MOD_REG_RM_ReplaceableRm(block, 0, reg);
		}

		template<MemSize SIZE = BYTE_PTR, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> template_Setcc(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& mem) {
			static_assert(SIZE == BYTE_PTR, "i386: Invalid size of Setcc operand");
			mem.writeSegmPrefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			return mem.writeReplaceable(block, 0);
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
		static FwdLabel<SIZE> template_Jxx(BLOCK& block, common::Opcode opcode) {
			return JumpWriter<SIZE, BLOCK>::writeConditional(block, opcode);
		}

		template<JmpSize SIZE, class BLOCK>
		static FwdLabel<SIZE> template_Jmp(BLOCK& block) {
			static_assert(SIZE == SHORT || SIZE == LONG, "Jmp: Invalid jump size");
			return JumpWriter<SIZE, BLOCK>::write(block);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> template_Jmp(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == DWORD_PTR || SIZE == FWORD_PTR, "Jmp: Invalid size modifier");
			/*constexpr*/ if (SIZE == DWORD_PTR) {
				return template_1mem_operand_r<DWORD_PTR>(block, detail::opcode_JMP_NEAR, mem);
			}
			return template_1mem_operand_r<DWORD_PTR>(block, detail::opcode_JMP_FAR, mem);
		}

		template<MemSize SIZE, class BLOCK>
		static ReplaceableReg<Reg32> template_Jmp(BLOCK& block, const Reg32& reg) {
			static_assert(SIZE == DWORD_PTR, "Jmp: Invalid size modifier");
			return template_1reg_operand_r(block, detail::opcode_JMP_NEAR, reg);
		}

		template<MemSize SIZE, class BLOCK>
		static constexpr void template_Call(BLOCK& block, const Addr& jumpAddress) {
			static_assert(SIZE == DWORD_PTR, "Call: Invalid size modifier");
			common::write_Opcode(block, 0xE8);
			block.template pushRaw<byteOrder>(common::calc_Jump_Offset(block.getCurrentPtr(), jumpAddress, sizeof(Addr)));
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> template_Call(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == DWORD_PTR || SIZE == FWORD_PTR, "Call: Invalid size modifier");
			/*constexpr*/ if (SIZE == DWORD_PTR) {
				return template_1mem_operand_r<DWORD_PTR>(block, detail::opcode_CALL_NEAR, mem);
			}
			return template_1mem_operand_r<DWORD_PTR>(block, detail::opcode_CALL_FAR, mem);
		}

		template<MemSize SIZE, class BLOCK>
		static ReplaceableReg<Reg32> template_Call(BLOCK& block, const Reg32& reg) {
			static_assert(SIZE == DWORD_PTR, "Call: Invalid size modifier");
			return template_1reg_operand_r(block, detail::opcode_CALL_NEAR, reg);
		}

		template<JmpSize SIZE, class BLOCK>
		static FwdLabel<SIZE> template_Loop(BLOCK& block, common::Opcode opcode) {
			static_assert(SIZE == SHORT, "i386: unsupported loop jump size");
			common::write_Opcode(block, opcode);
			Offset offset = block.getOffset();
			block.skipBytes(FwdLabel<SHORT>::offset_size);
			return FwdLabel<SHORT>(offset);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void template_LoadSegReg(BLOCK& block, common::Opcode opcode, Reg32 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, opcode);
			mem.write(block, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void template_LoadSegReg(BLOCK& block, common::Opcode opcode, Reg16 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, opcode);
			mem.write(block, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_LoadSegReg_r(BLOCK& block, common::Opcode opcode, Reg32 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, opcode);
			return mem.writeReplaceable(block, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_LoadSegReg_r(BLOCK& block, common::Opcode opcode, Reg16 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, opcode);
			return mem.writeReplaceable(block, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void template_LoadSegRegEx(BLOCK& block, common::Opcode opcode, Reg32 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			mem.write(block, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void template_LoadSegRegEx(BLOCK& block, common::Opcode opcode, Reg16 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			mem.write(block, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_LoadSegRegEx_r(BLOCK& block, common::Opcode opcode, Reg32 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			return mem.writeReplaceable(block, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static auto template_LoadSegRegEx_r(BLOCK& block, common::Opcode opcode, Reg16 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			return mem.writeReplaceable(block, reg);
		}

		template<class BLOCK, class DST, class SRC>
		static constexpr void template_MovSpecReg(BLOCK& block, common::Opcode opcode, const DST& dst, const SRC& src) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
		}

		template<class BLOCK, class DST, class SRC>
		static auto template_MovSpecReg_r(BLOCK& block, common::Opcode opcode, const DST& dst, const SRC& src) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
			return write_MOD_REG_RM_ReplaceableRR(block, dst, src);
		}
	public:
#pragma region Data transfer [DONE]
		/* Convert byte to word */
		template <class BLOCK>
		static constexpr void Cbw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x98);
		}

		/* Convert byte to word */
		template <class BLOCK>
		static constexpr void Cdq(BLOCK& block) {
			common::write_Opcode(block, 0x99);
		}

		/* Convert word to double word */
		template <class BLOCK>
		static constexpr void Cwd(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x99);
		}

		/* Convert word to double word */
		template <class BLOCK>
		static constexpr void Cwde(BLOCK& block) {
			common::write_Opcode(block, 0x98);
		}

		/* Calculate effective address
		 - LEA reg16, [mem]
		*/
		template<AddressMode MODE, class BLOCK>
		static constexpr void Lea(BLOCK& block, Reg16 reg, const Mem32<MODE>& mem) {
			template_2operands(block, 0x8D, mem, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static auto Lea_r(BLOCK& block, Reg16 reg, const Mem32<MODE>& mem) {
			return template_2operands_r(block, 0x8D, mem, reg);
		}

		/* Calculate effective address
		 - LEA reg32, [mem]
		*/
		template<AddressMode MODE, class BLOCK>
		static constexpr void Lea(BLOCK& block, Reg32 reg, const Mem32<MODE>& mem) {
			template_2operands(block, 0x8D, mem, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static auto Lea_r(BLOCK& block, Reg32 reg, const Mem32<MODE>& mem) {
			return template_2operands_r(block, 0x8D, mem, reg);
		}

		/* Moving data
		 - MOV reg,reg
		 - MOV reg,[mem]
		 - MOV [mem],reg
		*/
		template<class DST, class SRC, class BLOCK>
		static constexpr void Mov(BLOCK& block, const DST& dst, const SRC& src) {
			template_2operands(block, detail::opcode_MOV.getMain(), dst, src);
		}

		template<class DST, class SRC, class BLOCK>
		static auto Mov_r(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_r(block, detail::opcode_MOV.getMain(), dst, src);
		}

		/* Moving data
		 - MOV [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static constexpr void Mov(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Mov: Invalid size modifier");
			template_mem_imm_operands<SIZE>(block, detail::opcode_MOV, mem, imm);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static auto Mov_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Mov: Invalid size modifier");
			return template_mem_imm_operands_r<SIZE>(block, detail::opcode_MOV, mem, imm);
		}

		/* Moving data
		 - MOV [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static constexpr void Mov(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			Mov<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		template<AddressMode MODE, class T, class BLOCK>
		static auto Mov_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Mov_r<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		/* Moving data
		 - MOV reg,imm
		*/
		template<class REG, class T, class BLOCK>
		static constexpr void Mov(BLOCK& block, const REG& reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "Mov: First parameter is not register");
			template_reg_imm_short_operands(block, 0xB0, reg, imm);
		}

		template<class REG, class T, class BLOCK>
		static auto Mov_r(BLOCK& block, const REG& reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "Mov: First parameter is not register");
			return template_reg_imm_short_operands_r(block, 0xB0, reg, imm);
		}
		
		/* Moving data
		 - MOV reg,sreg
		 - MOV [mem],sreg
		*/
		template<class DST, class BLOCK>
		static constexpr void Mov(BLOCK& block, const DST& dst, RegSeg sreg) {
			template_2operands_segm(block, 0x8c, dst, sreg);
		}

		template<class DST, class BLOCK>
		static auto Mov_r(BLOCK& block, const DST& dst, RegSeg sreg) {
			return template_2operands_segm_r(block, 0x8c, dst, sreg);
		}

		/* Moving data
		 - MOV sreg,reg
		 - MOV sreg,[mem]
		*/
		template<class SRC, class BLOCK>
		static constexpr void Mov(BLOCK& block, RegSeg sreg, const SRC& src) {
			template_2operands_segm(block, 0x8c, sreg, src);
		}

		template<class SRC, class BLOCK>
		static auto Mov_r(BLOCK& block, RegSeg sreg, const SRC& src) {
			return template_2operands_segm(block, 0x8c, sreg, src);
		}

		/* Move data to Control Registers 
		 - MOV creg,reg
		*/
		template<class BLOCK>
		static constexpr void Mov(BLOCK& block, RegControl dst, Reg32 src) {
			template_MovSpecReg(block, 0x22, dst, src);
		}

		template<class BLOCK>
		static auto Mov_r(BLOCK& block, RegControl dst, Reg32 src) {
			return template_MovSpecReg_r(block, 0x22, dst, src);
		}

		/* Move data from Control Registers
		 - MOV reg,creg
		*/
		template<class BLOCK>
		static constexpr void Mov(BLOCK& block, Reg32 dst, RegControl src) {
			template_MovSpecReg(block, 0x20, src, dst);
		}

		template<class BLOCK>
		static auto Mov_r(BLOCK& block, Reg32 dst, RegControl src) {
			return template_MovSpecReg_r(block, 0x20, src, dst);
		}

		/* Move data to Control Registers
		 - MOV dreg,reg
		*/
		template<class BLOCK>
		static constexpr void Mov(BLOCK& block, RegDebug dst, Reg32 src) {
			template_MovSpecReg(block, 0x23, dst, src);
		}

		template<class BLOCK>
		static auto Mov_r(BLOCK& block, RegDebug dst, Reg32 src) {
			return template_MovSpecReg_r(block, 0x23, dst, src);
		}

		/* Move data to Control Registers
		 - MOV reg,dreg
		*/
		template<class BLOCK>
		static constexpr void Mov(BLOCK& block, Reg32 dst, RegDebug src) {
			template_MovSpecReg(block, 0x21, src, dst);
		}

		template<class BLOCK>
		static auto Mov_r(BLOCK& block, Reg32 dst, RegDebug src) {
			return template_MovSpecReg_r(block, 0x21, src, dst);
		}

		/* Mov with sign extend
		 - MOVSX reg,reg
		*/
		template<class DST, class SRC, class BLOCK>
		static constexpr void Movsx(BLOCK& block, const DST& dst, const SRC& src) {
			template_2reg_operands_extend_val(block, 0xBE, dst, src);
		}

		template<class DST, class SRC, class BLOCK>
		static auto Movsx_r(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2reg_operands_extend_val_r(block, 0xBE, dst, src);
		}

		/* Mov with sign extend
		 - MOVSX reg,[mem]
		*/
		template<MemSize SIZE, class REG, AddressMode MODE, class BLOCK>
		static constexpr void Movsx(BLOCK& block, const REG& dst, const Mem32<MODE>& src) {
			static_assert(IsRegType<REG>::value, "Movsx: First parameter is not register");
			template_2operands_extend_val<SIZE>(block, 0xBE, dst, src);
		}

		template<MemSize SIZE, class REG, AddressMode MODE, class BLOCK>
		static auto Movsx_r(BLOCK& block, const REG& dst, const Mem32<MODE>& src) {
			static_assert(IsRegType<REG>::value, "Movsx: First parameter is not register");
			return template_2operands_extend_val_r<SIZE>(block, 0xBE, dst, src);
		}

		/* Mov with zero extend
		 - MOVZX reg,reg
		*/
		template<class DST, class SRC, class BLOCK>
		static constexpr void Movzx(BLOCK& block, const DST& dst, const SRC& src) {
			template_2reg_operands_extend_val(block, 0xB6, dst, src);
		}

		template<class DST, class SRC, class BLOCK>
		static auto Movzx_r(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2reg_operands_extend_val_r(block, 0xB6, dst, src);
		}

		/* Mov with zero extend
		 - MOVZX reg,[mem]
		*/
		template<MemSize SIZE, class REG, AddressMode MODE, class BLOCK>
		static constexpr void Movzx(BLOCK& block, const REG& dst, const Mem32<MODE>& src) {
			static_assert(IsRegType<REG>::value, "Movzx: First parameter is not register");
			template_2operands_extend_val<SIZE>(block, 0xB6, dst, src);
		}

		template<MemSize SIZE, class REG, AddressMode MODE, class BLOCK>
		static auto Movzx_r(BLOCK& block, const REG& dst, const Mem32<MODE>& src) {
			static_assert(IsRegType<REG>::value, "Movzx: First parameter is not register");
			return template_2operands_extend_val_r<SIZE>(block, 0xB6, dst, src);
		}

		/* Push value to the stack
		 - PUSH reg16
		*/
		template<class BLOCK>
		static constexpr void Push(BLOCK& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x50 | reg);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg16> Push_r(BLOCK& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x50 | reg);
			return ReplaceableReg<Reg16>(offset, 0);
		}

		/* Push value to the stack
		 - PUSH reg32
		*/
		template<class BLOCK>
		static constexpr void Push(BLOCK& block, Reg32 reg) {
			common::write_Opcode(block, 0x50 | reg);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg32> Push_r(BLOCK& block, Reg32 reg) {
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x50 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Push value to the stack
		 - PUSH imm8
		*/
		template<class BLOCK>
		static constexpr void Push(BLOCK& block, S8 number) {
			common::write_Opcode(block, 0x6A);
			common::write_Immediate(block, number);
		}

		template<class BLOCK>
		static ReplaceableImmediate<S8> Push_r(BLOCK& block, S8 number) {
			common::write_Opcode(block, 0x6A);
			return write_Immediate_Replaceable(block, number);
		}

		/* Push value to the stack
		 - PUSH imm16
		*/
		template<class BLOCK>
		static constexpr void Push(BLOCK& block, S16 number) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x68);
			common::write_Immediate(block, number);
		}

		template<class BLOCK>
		static ReplaceableImmediate<S16> Push_r(BLOCK& block, S16 number) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x68);
			return write_Immediate_Replaceable(block, number);
		}

		/* Push value to the stack
		 - PUSH imm32
		*/
		template<class BLOCK>
		static constexpr void Push(BLOCK& block, S32 number) {
			common::write_Opcode(block, 0x68);
			common::write_Immediate(block, number);
		}

		template<class BLOCK>
		static ReplaceableImmediate<S32> Push_r(BLOCK& block, S32 number) {
			common::write_Opcode(block, 0x68);
			return write_Immediate_Replaceable(block, number);
		}

		/* Push value to the stack
		 - PUSH [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static constexpr void Push(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Push: Invalid size modifier");
			template_1mem_operand<SIZE>(block, detail::opcode_PUSH, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Push_r(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Push: Invalid size modifier");
			return template_1mem_operand_r<SIZE>(block, detail::opcode_PUSH, mem);
		}

		/* Push value to the stack
		 - PUSH sreg
		*/
		template<class BLOCK>
		static void Push(BLOCK& block, RegSeg sreg) {
			static constexpr common::Opcode opcodes[] = { 0x06, 0x0E, 0x16, 0x1E, 0xA0, 0xA8 };
			if (sreg == FS || sreg == GS) {
				common::write_Opcode_Extended_Prefix(block);
			}
			common::write_Opcode(block, opcodes[sreg]);
		}

		/* Store all 16 bit general registers to stack */
		template<class BLOCK>
		static constexpr void Pusha(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x60);
		}

		/* Store all 32 bit general registers to stack */
		template<class BLOCK>
		static constexpr void Pushad(BLOCK& block) {
			common::write_Opcode(block, 0x60);
		}

		/* Pop value from the stack
		 - POP reg16
		*/
		template<class BLOCK>
		static constexpr void Pop(BLOCK& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x58 | reg);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg16> Pop_r(BLOCK& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x58 | reg);
			return ReplaceableReg<Reg16>(offset, 0);
		}

		/* Pop value from the stack
		 - POP reg32
		*/
		template<class BLOCK>
		static constexpr void Pop(BLOCK& block, Reg32 reg) {
			common::write_Opcode(block, 0x58 | reg);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg32> Pop_r(BLOCK& block, Reg32 reg) {
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x58 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Pop value from the stack
		 - POP [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static constexpr void Pop(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Pop: Invalid size modifier");
			template_1mem_operand<SIZE>(block, detail::opcode_POP, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Pop_r(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Pop: Invalid size modifier");
			return template_1mem_operand_r<SIZE>(block, detail::opcode_POP, mem);
		}
		
		/* Push value to the stack
		 - POP sreg
		*/
		template<class BLOCK>
		static void Pop(BLOCK& block, RegSeg sreg) {
			// replace invalid pop cs by nop
			static constexpr common::Opcode opcodes[] = { 0x07, 0x90, 0x17, 0x1F, 0xA1, 0xA9 };
			if (sreg == FS || sreg == GS) {
				common::write_Opcode_Extended_Prefix(block);
			}
			common::write_Opcode(block, opcodes[sreg]);
		}

		/* Load all 16 bit general registers from stack */
		template<class BLOCK>
		static constexpr void Popa(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x61);
		}

		/* Load all 32 bit general registers from stack */
		template<class BLOCK>
		static constexpr void Popad(BLOCK& block) {
			common::write_Opcode(block, 0x61);
		}

		/* Xchange
		 - XCHG EAX,reg32
		*/
		template<class BLOCK>
		static constexpr void Xchg(BLOCK& block, Reg32 reg) {
			common::write_Opcode(block, 0x90 | reg);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg32> Xchg_r(BLOCK& block, Reg32 reg) {
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x90 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Xchange
		 - XCHG reg32,reg32
		*/
		template<class BLOCK>
		static constexpr void Xchg(BLOCK& block, Reg32 dst, Reg32 src) {
			common::write_Opcode(block, 0x87);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
		}

		template<class BLOCK>
		static std::pair<ReplaceableReg<Reg32>, ReplaceableReg<Reg32>> Xchg_r(BLOCK& block, Reg32 dst, Reg32 src) {
			common::write_Opcode(block, 0x87);
			return write_MOD_REG_RM_ReplaceableRR(block, dst, src);
		}

		/* Xchange
		 - XCHG AX,reg16
		*/
		template<class BLOCK>
		static constexpr void Xchg(BLOCK& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x90 | reg);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg16> Xchg_r(BLOCK& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x90 | reg);
			return ReplaceableReg<Reg16>(offset, 0);
		}

		/* Xchange
		 - XCHG reg16,reg16
		*/
		template<class BLOCK>
		static constexpr void Xchg(BLOCK& block, Reg16 dst, Reg16 src) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x87);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
		}

		template<class BLOCK>
		static std::pair<ReplaceableReg<Reg16>, ReplaceableReg<Reg16>> Xchg_r(BLOCK& block, Reg16 dst, Reg16 src) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x87);
			return write_MOD_REG_RM_ReplaceableRR(block, dst, src);
		}

		/* Xchange
		 - XCHG reg8,reg8
		*/
		template<class BLOCK>
		static constexpr void Xchg(BLOCK& block, Reg8 dst, Reg8 src) {
			common::write_Opcode(block, 0x86);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, dst, src);
		}

		template<class BLOCK>
		static std::pair<ReplaceableReg<Reg8>, ReplaceableReg<Reg8>> Xchg_r(BLOCK& block, Reg8 dst, Reg8 src) {
			common::write_Opcode(block, 0x86);
			return write_MOD_REG_RM_ReplaceableRR(block, dst, src);
		}

		/* Xchange
		 - XCHG reg,[mem]
		 - XCHG [mem],reg
		*/
		template<class DST, class SRC, class BLOCK>
		static constexpr void Xchg(BLOCK& block, const DST& dst, const SRC& src) {
			template_2operands_symetric(block, 0x86, dst, src);
		}

		template<class DST, class SRC, class BLOCK>
		static auto Xchg_r(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_symetric_r(block, 0x86, dst, src);
		}

		/* Translate by table */
		template<class BLOCK>
		static constexpr void Xlat(BLOCK& block) {
			common::write_Opcode(block, 0xD7);
		}

		/* Read data from port
		 - IN reg,imm
		*/
		template<MemSize SIZE, class BLOCK>
		static constexpr void In(BLOCK& block, const U8& imm) {
			static_assert(isGeneralMemSize(SIZE), "In: Invalid size modifier");
			write_Opcode<SIZE>(block, 0xE4);
			common::write_Immediate(block, imm);
		}

		template<MemSize SIZE, class BLOCK>
		static ReplaceableImmediate<U8::type> In_r(BLOCK& block, const U8& imm) {
			static_assert(isGeneralMemSize(SIZE), "In: Invalid size modifier");
			write_Opcode<SIZE>(block, 0xE4);
			return write_Immediate_Replaceable(block, imm);
		}

		/* Read data from port
		 - IN eax/ax/al,dx
		*/
		template<MemSize SIZE, Reg16 PORT_REG, class BLOCK>
		static constexpr void In(BLOCK& block) {
			static_assert(isGeneralMemSize(SIZE), "In: Invalid size modifier");
			write_Opcode<SIZE>(block, 0xEC);
		}

		/* Write data to port
		 - OUT imm, reg
		*/
		template<MemSize SIZE, class BLOCK>
		static constexpr void Out(BLOCK& block, const U8& imm) {
			static_assert(isGeneralMemSize(SIZE), "Out: Invalid size modifier");
			write_Opcode<SIZE>(block, 0xE6);
			common::write_Immediate(block, imm);
		}

		template<MemSize SIZE, class BLOCK>
		static ReplaceableImmediate<U8::type> Out_r(BLOCK& block, const U8& imm) {
			static_assert(isGeneralMemSize(SIZE), "Out: Invalid size modifier");
			write_Opcode<SIZE>(block, 0xE6);
			return write_Immediate_Replaceable(block, imm);
		}

		/* Write data to port
		 - OUT dx,eax/ax
		*/
		template<MemSize SIZE, Reg16 PORT_REG, class BLOCK>
		static constexpr void Out(BLOCK& block) {
			static_assert(isGeneralMemSize(SIZE), "Out: Invalid size modifier");
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
		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static constexpr void Adc(BLOCK& block, const DST& dst, const SRC& src) {
			template_2operands_opt_imm<L>(block, detail::opcode_ADC, dst, src);
		}

		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static auto Adc_r(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm_r<L>(block, detail::opcode_ADC, dst, src);
		}

		/* Addition with carry flag (CF)
		 - ADC EAX/AX/AL,imm
		*/
		template<MemSize SIZE, class T, class BLOCK>
		static constexpr void Adc(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Adc: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Adc: Immediate size not equal size modifier");
			template_2operands_first_reg_imm<SIZE>(block, 0x14, imm);
		}

		template<MemSize SIZE, class T, class BLOCK>
		static auto Adc_r(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Adc: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Adc: Immediate size not equal size modifier");
			return template_2operands_first_reg_imm_r<SIZE>(block, 0x14, imm);
		}

		/* Addition with carry flag (CF)
		 - ADC [mem],imm
		*/
		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static constexpr void Adc(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Adc: Invalid size modifier");
			template_mem_imm_opt_operands<SIZE, L>(block, detail::opcode_ADC, mem, imm);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static auto Adc_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Adc: Invalid size modifier");
			return template_mem_imm_opt_operands_r<SIZE, L>(block, detail::opcode_ADC, mem, imm);
		}

		/* Addition with carry flag (CF)
		 - ADC [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static constexpr void Adc(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			Adc<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		template<AddressMode MODE, class T, class BLOCK>
		static auto Adc_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Adc_r<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		/* Addition
		 - ADD reg,reg
		 - ADD reg,[mem]
		 - ADD [mem],reg
		 - ADD reg,imm
		*/
		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static constexpr void Add(BLOCK& block, const DST& dst, const SRC& src) {
			template_2operands_opt_imm<L>(block, detail::opcode_ADD, dst, src);
		}

		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static auto Add_r(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm_r<L>(block, detail::opcode_ADD, dst, src);
		}

		/* Addition
		 - ADD EAX,AX,AL,imm
		*/
		template<MemSize SIZE, class T, class BLOCK>
		static constexpr void Add(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Add: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Add: Immediate size not equal size modifier");
			template_2operands_first_reg_imm<SIZE>(block, 0x04, imm);
		}

		template<MemSize SIZE, class T, class BLOCK>
		static auto Add_r(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Add: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Add: Immediate size not equal size modifier");
			return template_2operands_first_reg_imm_r<SIZE>(block, 0x04, imm);
		}

		/* Addition
		 - ADD [mem],imm
		*/
		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static constexpr void Add(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Add: Invalid size modifier");
			template_mem_imm_opt_operands<SIZE, L>(block, detail::opcode_ADD, mem, imm);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static auto Add_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Add: Invalid size modifier");
			return template_mem_imm_opt_operands_r<SIZE, L>(block, detail::opcode_ADD, mem, imm);
		}

		/* Addition
		 - ADD [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static constexpr void Add(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			Add<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		template<AddressMode MODE, class T, class BLOCK>
		static auto Add_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Add_r<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		/* Compare Two Operands
		 - CMP reg,reg
		 - CMP reg,[mem]
		 - CMP [mem],reg
		 - CMP reg,imm
		*/
		template<class DST, class SRC, class BLOCK>
		static constexpr void Cmp(BLOCK& block, const DST& dst, const SRC& src) {
			template_2operands_opt_imm(block, detail::opcode_CMP, dst, src);
		}

		template<class DST, class SRC, class BLOCK>
		static auto Cmp_r(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm_r(block, detail::opcode_CMP, dst, src);
		}

		template<MemSize SIZE, class T, class BLOCK>
		static constexpr void Cmp(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Cmp: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Cmp: Immediate size not equal size modifier");
			template_2operands_first_reg_imm<SIZE>(block, 0x3C, imm);
		}

		template<MemSize SIZE, class T, class BLOCK>
		static auto Cmp_r(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Cmp: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Cmp: Immediate size not equal size modifier");
			return template_2operands_first_reg_imm_r<SIZE>(block, 0x3C, imm);
		}

		/* Compare Two Operands
		 - CMP [mem],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static constexpr void Cmp(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Cmp: Invalid size modifier");
			template_mem_imm_opt_operands<SIZE, NO_LOCK>(block, detail::opcode_CMP, mem, imm);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static auto Cmp_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Cmp: Invalid size modifier");
			return template_mem_imm_opt_operands_r<SIZE, NO_LOCK>(block, detail::opcode_CMP, mem, imm);
		}

		/* Compare Two Operands
		 - CMP [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static constexpr void Cmp(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			Cmp<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		template<AddressMode MODE, class T, class BLOCK>
		static auto Cmp_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Cmp_r<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		/* Compare and set flags
		 - CMP imm,[mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static constexpr void Cmp(BLOCK& block, const Imm<T>& imm, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Cmp: Invalid size modifier");
			template_mem_imm_opt_operands<SIZE, NO_LOCK>(block, detail::opcode_CMP, mem, imm);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static auto Cmp_r(BLOCK& block, const Imm<T>& imm, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Cmp: Invalid size modifier");
			return template_mem_imm_opt_operands_r<SIZE, NO_LOCK>(block, detail::opcode_CMP, mem, imm);
		}

		/* Compare and set flags
		 - CMP imm,[mem] (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static constexpr void Cmp(BLOCK& block, const Imm<T>& imm, const Mem32<MODE>& mem) {
			Cmp<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		template<AddressMode MODE, class T, class BLOCK>
		static auto Cmp_r(BLOCK& block, const Imm<T>& imm, const Mem32<MODE>& mem) {
			return Cmp_r<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		/* Increment
		 - INC reg32
		*/
		template<class BLOCK>
		static constexpr void Inc(BLOCK& block, Reg32 reg) {
			common::write_Opcode(block, 0x40 | reg);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg32> Inc_r(BLOCK& block, Reg32 reg) {
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x40 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Increment
		 - INC reg16
		*/
		template<class BLOCK>
		static constexpr void Inc(BLOCK& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x40 | reg);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg16> Inc_r(BLOCK& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x40 | reg);
			return ReplaceableReg<Reg16>(offset, 0);
		}

		/* Increment
		 - INC reg8
		*/
		template<class BLOCK>
		static constexpr void Inc(BLOCK& block, Reg8 reg) {
			template_1reg_operand(block, detail::opcode_INC, reg);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg8> Inc_r(BLOCK& block, Reg8 reg) {
			return template_1reg_operand_r(block, detail::opcode_INC, reg);
		}

		/* Increment
		 - INC [mem]
		*/
		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static constexpr void Inc(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Inc: Invalid size modifier");
			template_1mem_operand<SIZE, L>(block, detail::opcode_INC, mem);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Inc_r(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Inc: Invalid size modifier");
			return template_1mem_operand_r<SIZE, L>(block, detail::opcode_INC, mem);
		}

		/* Decrement
		 - DEC reg32
		*/
		template<class BLOCK>
		static constexpr void Dec(BLOCK& block, Reg32 reg) {
			common::write_Opcode(block, 0x48 | reg);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg32> Dec_r(BLOCK& block, Reg32 reg) {
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x48 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Decrement
		 - DEC reg16
		*/
		template<class BLOCK>
		static constexpr void Dec(BLOCK& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x48 | reg);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg16> Dec_r(BLOCK& block, Reg16 reg) {
			common::write_Opcode_16bit_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0x48 | reg);
			return ReplaceableReg<Reg16>(offset, 0);
		}

		/* Decrement
		 - DEC reg8
		*/
		template<class BLOCK>
		static constexpr void Dec(BLOCK& block, Reg8 reg) {
			template_1reg_operand(block, detail::opcode_DEC, reg);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg8> Dec_r(BLOCK& block, Reg8 reg) {
			return template_1reg_operand_r(block, detail::opcode_DEC, reg);
		}

		/* Decrement
		- DEC [mem]
		*/
		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static constexpr void Dec(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Dec: Invalid size modifier");
			template_1mem_operand<SIZE, L>(block, detail::opcode_DEC, mem);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Dec_r(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Dec: Invalid size modifier");
			return template_1mem_operand_r<SIZE, L>(block, detail::opcode_DEC, mem);
		}

		/* Change sign
		 - NEG reg
		*/
		template<class REG, class BLOCK>
		static constexpr void Neg(BLOCK& block, REG reg) {
			static_assert(IsRegType<REG>::value, "Neg: Parameter is not register");
			template_1reg_operand(block, detail::opcode_NEG, reg);
		}

		template<class REG, class BLOCK>
		static ReplaceableReg<REG> Neg_r(BLOCK& block, REG reg) {
			static_assert(IsRegType<REG>::value, "Neg: Parameter is not register");
			return template_1reg_operand_r(block, detail::opcode_NEG, reg);
		}

		/* Change sign
		 - NEG [mem]
		*/
		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static constexpr void Neg(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Neg: Invalid size modifier");
			template_1mem_operand<SIZE, L>(block, detail::opcode_NEG, mem);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Neg_r(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Neg: Invalid size modifier");
			return template_1mem_operand_r<SIZE, L>(block, detail::opcode_NEG, mem);
		}

		/* Multiply unsigned numbers
		 - MUL reg
		*/
		template<class REG, class BLOCK>
		static constexpr void Mul(BLOCK& block, REG reg) {
			static_assert(IsRegType<REG>::value, "Mul: Parameter is not register");
			template_1reg_operand(block, detail::opcode_MUL, reg);
		}

		template<class REG, class BLOCK>
		static ReplaceableReg<REG> Mul_r(BLOCK& block, REG reg) {
			static_assert(IsRegType<REG>::value, "Mul: Parameter is not register");
			return template_1reg_operand_r(block, detail::opcode_MUL, reg);
		}

		/* Multiply unsigned numbers
		 - MUL [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static constexpr void Mul(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Mul: Invalid size modifier");
			template_1mem_operand<SIZE>(block, detail::opcode_MUL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Mul_r(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Mul: Invalid size modifier");
			return template_1mem_operand_r<SIZE>(block, detail::opcode_MUL, mem);
		}

		/* Signed Multiply
		 - IMUL reg
		*/
		template<class REG, class BLOCK>
		static constexpr void Imul(BLOCK& block, REG reg) {
			static_assert(IsRegType<REG>::value, "Imul: Parameter is not register");
			template_1reg_operand(block, detail::opcode_IMUL, reg);
		}

		template<class REG, class BLOCK>
		static ReplaceableReg<REG> Imul_r(BLOCK& block, REG reg) {
			static_assert(IsRegType<REG>::value, "Imul: Parameter is not register");
			return template_1reg_operand_r(block, detail::opcode_IMUL, reg);
		}

		/* Signed Multiply
		 - IMUL [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static constexpr void Imul(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Imul: Invalid size modifier");
			template_1mem_operand<SIZE>(block, detail::opcode_IMUL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Imul_r(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Imul: Invalid size modifier");
			return template_1mem_operand_r<SIZE>(block, detail::opcode_IMUL, mem);
		}

		/* Signed Multiply
		 - IMUL reg,reg
		*/
		template<class REG, class BLOCK>
		static constexpr void Imul(BLOCK& block, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Imul: First and second parameter is not register");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, 0xAF);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg1, reg2);
		}

		template<class REG, class BLOCK>
		static auto Imul_r(BLOCK& block, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Imul: First and second parameter is not register");
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, 0xAF);
			return write_MOD_REG_RM_ReplaceableRR(block, reg1, reg2);
		}

		/* Signed Multiply
		 - IMUL reg,[mem]
		*/
		template<class REG, AddressMode MODE, class BLOCK>
		static constexpr void Imul(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Imul: First parameter is not register");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, 0xAF);
			mem.write(block, reg);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static auto Imul_r(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Imul: First parameter is not register");
			mem.writeSegmPrefix(block);
			write_Opcode_Only_Extended_Prefixs<TypeMemSize<REG>::value>(block, 0xAF);
			return mem.writeReplaceable(block, reg);
		}

		/* Signed Multiply
		 - IMUL reg16,imm8
		*/
		template<class BLOCK>
		static constexpr void Imul(BLOCK& block, Reg16 reg, S8 imm) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x6B);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg, 0);
			common::write_Immediate(block, imm);
		}

		template<class BLOCK>
		static auto Imul_r(BLOCK& block, Reg16 reg, S8 imm) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x6B);
			auto replaceableReg = write_MOD_REG_RM_ReplaceableReg(block, reg, 0);
			auto replaceableImm = write_Immediate_Replaceable(block, imm);
			return std::make_pair(replaceableReg, replaceableImm);
		}

		/* Signed Multiply
		 - IMUL reg32,imm8
		*/
		template<class BLOCK>
		static constexpr void Imul(BLOCK& block, Reg32 reg, S8 imm) {
			common::write_Opcode(block, 0x6B);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg, 0);
			common::write_Immediate(block, imm);
		}

		template<class BLOCK>
		static auto Imul_r(BLOCK& block, Reg32 reg, S8 imm) {
			common::write_Opcode(block, 0x6B);
			auto replaceableReg = write_MOD_REG_RM_ReplaceableReg(block, reg, 0);
			auto replaceableImm = write_Immediate_Replaceable(block, imm);
			return std::make_pair(replaceableReg, replaceableImm);
		}

		/* Signed Multiply
		 - IMUL reg16,imm16
		*/
		template<class BLOCK>
		static constexpr void Imul(BLOCK& block, Reg16 reg, S16 imm) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x69);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg, 0);
			common::write_Immediate(block, imm);
		}

		template<class BLOCK>
		static auto Imul_r(BLOCK& block, Reg16 reg, S16 imm) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x69);
			auto replaceableReg = write_MOD_REG_RM_ReplaceableReg(block, reg, 0);
			auto replaceableImm = write_Immediate_Replaceable(block, imm);
			return std::make_pair(replaceableReg, replaceableImm);
		}

		/* Signed Multiply
		 - IMUL reg32,imm32
		*/
		template<class BLOCK>
		static constexpr void Imul(BLOCK& block, Reg32 reg, S32 imm) {
			common::write_Opcode(block, 0x69);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg, 0);
			common::write_Immediate(block, imm);
		}

		template<class BLOCK>
		static auto Imul_r(BLOCK& block, Reg32 reg, S32 imm) {
			common::write_Opcode(block, 0x69);
			auto replaceableReg = write_MOD_REG_RM_ReplaceableReg(block, reg, 0);
			auto replaceableImm = write_Immediate_Replaceable(block, imm);
			return std::make_pair(replaceableReg, replaceableImm);
		}

		/* Signed Multiply
		 - IMUL reg,reg,imm
		*/
		template<class REG, class T, class BLOCK>
		static constexpr void Imul(BLOCK& block, REG reg1, REG reg2, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "Imul: First and second parameter is not register");
			write_Opcode_Imm_Optimized<TypeMemSize<REG>::value, TypeMemSize<Imm<T>>::value>(block, 0x69);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg1, reg2);
			common::write_Immediate(block, imm);
		}

		template<class REG, class T, class BLOCK>
		static auto Imul_r(BLOCK& block, REG reg1, REG reg2, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "Imul: First and second parameter is not register");
			write_Opcode_Imm_Optimized<TypeMemSize<REG>::value, TypeMemSize<Imm<T>>::value>(block, 0x69);
			auto replaceableRegs = write_MOD_REG_RM_ReplaceableRR(block, reg1, reg2);
			auto replaceableImm = write_Immediate_Replaceable(block, imm);
			return std::make_tuple(replaceableRegs.first, replaceableRegs.second, replaceableImm);
		}

		/* Signed Multiply
		 - IMUL reg,reg,imm
		*/
		template<class REG, AddressMode MODE, class T, class BLOCK>
		static constexpr void Imul(BLOCK& block, REG reg, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "Imul: First parameter is not register");
			mem.writeSegmPrefix(block);
			write_Opcode_Imm_Optimized<TypeMemSize<REG>::value, TypeMemSize<Imm<T>>::value>(block, 0x69);
			mem.write(block, reg);
			common::write_Immediate(block, imm);
		}

		template<class REG, AddressMode MODE, class T, class BLOCK>
		static auto Imul_r(BLOCK& block, REG reg, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "Imul: First parameter is not register");
			mem.writeSegmPrefix(block);
			write_Opcode_Imm_Optimized<TypeMemSize<REG>::value, TypeMemSize<Imm<T>>::value>(block, 0x69);
			auto replaceableMem = mem.writeReplaceable(block, reg);
			auto replaceableImm = write_Immediate_Replaceable(block, imm);
			return std::make_tuple(replaceableMem.template getOtherReg<REG>(), replaceableMem, replaceableImm);
		}

		/* Divide unsigned numbers
		 - DIV reg
		*/
		template<class REG, class BLOCK>
		static constexpr void Div(BLOCK& block, REG reg) {
			static_assert(IsRegType<REG>::value, "Div: Parameter is not register");
			template_1reg_operand(block, detail::opcode_DIV, reg);
		}

		template<class REG, class BLOCK>
		static ReplaceableReg<REG> Div_r(BLOCK& block, REG reg) {
			static_assert(IsRegType<REG>::value, "Div: Parameter is not register");
			return template_1reg_operand_r(block, detail::opcode_DIV, reg);
		}

		/* Divide unsigned numbers
		 - DIV [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static constexpr void Div(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Div: Invalid size modifier");
			template_1mem_operand<SIZE>(block, detail::opcode_DIV, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Div_r(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Div: Invalid size modifier");
			return template_1mem_operand_r<SIZE>(block, detail::opcode_DIV, mem);
		}

		/* Divide signed numbers
		 - IDIV reg
		*/
		template<class REG, class BLOCK>
		static constexpr void Idiv(BLOCK& block, REG reg) {
			static_assert(IsRegType<REG>::value, "Idiv: Parameter is not register");
			template_1reg_operand(block, detail::opcode_IDIV, reg);
		}

		template<class REG, class BLOCK>
		static ReplaceableReg<REG> Idiv_r(BLOCK& block, REG reg) {
			static_assert(IsRegType<REG>::value, "Idiv: Parameter is not register");
			return template_1reg_operand_r(block, detail::opcode_IDIV, reg);
		}

		/* Divide signed numbers
		 - IDIV [mem]
		*/
		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static constexpr void Idiv(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Idiv: Invalid size modifier");
			template_1mem_operand<SIZE>(block, detail::opcode_IDIV, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Idiv_r(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Idiv: Invalid size modifier");
			return template_1mem_operand_r<SIZE>(block, detail::opcode_IDIV, mem);
		}

		/* Integer Subtraction with Borrow
		 - SBB reg,reg
		 - SBB reg,[mem]
		 - SBB [mem],reg
		 - SBB reg,imm
		*/
		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static constexpr void Sbb(BLOCK& block, const DST& dst, const SRC& src) {
			template_2operands_opt_imm<L>(block, detail::opcode_SBB, dst, src);
		}

		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static auto Sbb_r(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm_r<L>(block, detail::opcode_SBB, dst, src);
		}

		/* Integer Subtraction with Borrow
		 - SBB EAX/AX/AL,imm
		*/
		template<MemSize SIZE, class T, class BLOCK>
		static constexpr void Sbb(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Sbb: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Sbb: Immediate size not equal size modifier");
			template_2operands_first_reg_imm<SIZE>(block, 0x1C, imm);
		}

		template<MemSize SIZE, class T, class BLOCK>
		static auto Sbb_r(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Sbb: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Sbb: Immediate size not equal size modifier");
			return template_2operands_first_reg_imm_r<SIZE>(block, 0x1C, imm);
		}

		/* Integer Subtraction with Borrow
		 - SBB [mem],imm
		*/
		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static constexpr void Sbb(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Sbb: Invalid size modifier");
			template_mem_imm_opt_operands<SIZE, L>(block, detail::opcode_SBB, mem, imm);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static auto Sbb_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Sbb: Invalid size modifier");
			return template_mem_imm_opt_operands_r<SIZE, L>(block, detail::opcode_SBB, mem, imm);
		}

		/* Integer Subtraction with Borrow
		 - SBB [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static constexpr void Sbb(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			Sbb<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		template<AddressMode MODE, class T, class BLOCK>
		static auto Sbb_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Sbb_r<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		/* Subtract
		 - SUB reg,reg
		 - SUB reg,[mem]
		 - SUB [mem],reg
		 - SUB reg,imm
		*/
		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static constexpr void Sub(BLOCK& block, const DST& dst, const SRC& src) {
			template_2operands_opt_imm<L>(block, detail::opcode_SUB, dst, src);
		}

		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static auto Sub_r(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm_r<L>(block, detail::opcode_SUB, dst, src);
		}

		/* Subtract
		 - SUB EAX/AX/AL,imm
		*/
		template<MemSize SIZE, class T, class BLOCK>
		static constexpr void Sub(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Sub: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Sub: Immediate size not equal size modifier");
			template_2operands_first_reg_imm<SIZE>(block, 0x2C, imm);
		}

		template<MemSize SIZE, class T, class BLOCK>
		static auto Sub_r(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Sub: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Sub: Immediate size not equal size modifier");
			return template_2operands_first_reg_imm_r<SIZE>(block, 0x2C, imm);
		}

		/* Subtract
		 - SUB [mem],imm
		*/
		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static constexpr void Sub(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Sub: Invalid size modifier");
			template_mem_imm_opt_operands<SIZE, L>(block, detail::opcode_SUB, mem, imm);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static auto Sub_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Sub: Invalid size modifier");
			return template_mem_imm_opt_operands_r<SIZE, L>(block, detail::opcode_SUB, mem, imm);
		}

		/* Subtract
		 - SUB [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static constexpr void Sub(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			Sub<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		template<AddressMode MODE, class T, class BLOCK>
		static auto Sub_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Sub_r<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}
#pragma endregion

#pragma region Decimal arithmetics [DONE]
		/* ACII correction after addition */
		template<class BLOCK>
		static constexpr void Aaa(BLOCK& block) {
			common::write_Opcode(block, 0x37);
		}

		/* ASCII correction before divide 
		 - AAD {imm} 
		 */
		template<class BLOCK>
		static constexpr void Aad(BLOCK& block, S8 base = S8(10)) {
			common::write_Opcode(block, 0xD5);
			common::write_Immediate(block, base);
		}

		template<class BLOCK>
		static ReplaceableImmediate<S8::type> Aad_r(BLOCK& block, S8 base = S8(10)) {
			common::write_Opcode(block, 0xD5);
			return write_Immediate_Replaceable(block, base);
		}

		/* ASCII correction after multiply
		 - AAM {imm}
		 */
		template<class BLOCK>
		static constexpr void Aam(BLOCK& block, S8 base = S8(10)) {
			common::write_Opcode(block, 0xD4);
			common::write_Immediate(block, base);
		}

		template<class BLOCK>
		static ReplaceableImmediate<S8::type> Aam_r(BLOCK& block, S8 base = S8(10)) {
			common::write_Opcode(block, 0xD4);
			return write_Immediate_Replaceable(block, base);
		}

		/* ASCII correction after subtraction */
		template<class BLOCK>
		static constexpr void Aas(BLOCK& block) {
			common::write_Opcode(block, 0x3F);
		}

		/* BCD-correction after addition */
		template<class BLOCK>
		static constexpr void Daa(BLOCK& block) {
			common::write_Opcode(block, 0x27);
		}

		/* BCD-correction after subtraction */
		template<class BLOCK>
		static constexpr void Das(BLOCK& block) {
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
		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static constexpr void And(BLOCK& block, const DST& dst, const SRC& src) {
			template_2operands_opt_imm<L>(block, detail::opcode_AND, dst, src);
		}

		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static auto And_r(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm_r<L>(block, detail::opcode_AND, dst, src);
		}

		/* Logical AND
		 - AND EAX/AX/Al,imm
		*/
		template<MemSize SIZE, class T, class BLOCK>
		static constexpr void And(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "And: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "And: Immediate size not equal size modifier");
			template_2operands_first_reg_imm<SIZE>(block, 0x24, imm);
		}

		template<MemSize SIZE, class T, class BLOCK>
		static auto And_r(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "And: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "And: Immediate size not equal size modifier");
			template_2operands_first_reg_imm_r<SIZE>(block, 0x24, imm);
		}

		/* Logical AND
		 - AND [mem],imm
		*/
		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static constexpr void And(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "And: Invalid size modifier");
			template_mem_imm_opt_operands<SIZE, L>(block, detail::opcode_AND, mem, imm);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static auto And_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "And: Invalid size modifier");
			return template_mem_imm_opt_operands_r<SIZE, L>(block, detail::opcode_AND, mem, imm);
		}

		/* Logical AND
		 - AND [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static constexpr void And(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			And<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		template<AddressMode MODE, class T, class BLOCK>
		static auto And_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return And_r<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		/* Logical NOT operation
		 - NOT reg
		*/
		template<class REG, class BLOCK>
		static constexpr void Not(BLOCK& block, REG reg) {
			static_assert(IsRegType<REG>::value, "Not: Parameter is not register");
			template_1reg_operand(block, detail::opcode_NOT, reg);
		}

		template<class REG, class BLOCK>
		static ReplaceableReg<REG> Not_r(BLOCK& block, REG reg) {
			static_assert(IsRegType<REG>::value, "Not: Parameter is not register");
			return template_1reg_operand_r(block, detail::opcode_NOT, reg);
		}

		/* Logical NOT operation
		 - NOT [mem]
		*/
		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static constexpr void Not(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Not: Invalid size modifier");
			template_1mem_operand<SIZE, L>(block, detail::opcode_NOT, mem);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class BLOCK>
		static ReplaceableMem32<MODE> Not_r(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Not: Invalid size modifier");
			return template_1mem_operand_r<SIZE, L>(block, detail::opcode_NOT, mem);
		}

		/* Logical OR operation
		 - OR reg,reg
		 - OR reg,[mem]
		 - OR [mem],reg
		 - OR reg,imm
		*/
		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static constexpr void Or(BLOCK& block, const DST& dst, const SRC& src) {
			template_2operands_opt_imm<L>(block, detail::opcode_OR, dst, src);
		}

		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static auto Or_r(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm_r<L>(block, detail::opcode_OR, dst, src);
		}

		/* Logical OR operation
		 - OR EAX/AX/AL,imm
		*/
		template<MemSize SIZE, class T, class BLOCK>
		static constexpr void Or(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Or: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Or: Immediate size not equal size modifier");
			template_2operands_first_reg_imm<SIZE>(block, 0x0C, imm);
		}

		template<MemSize SIZE, class T, class BLOCK>
		static auto Or_r(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Or: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Or: Immediate size not equal size modifier");
			return template_2operands_first_reg_imm_r<SIZE>(block, 0x0C, imm);
		}

		/* Logical OR operation
		 - OR [mem],imm
		*/
		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static constexpr void Or(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Or: Invalid size modifier");
			template_mem_imm_opt_operands<SIZE, L>(block, detail::opcode_OR, mem, imm);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static auto Or_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Or: Invalid size modifier");
			return template_mem_imm_opt_operands_r<SIZE, L>(block, detail::opcode_OR, mem, imm);
		}

		/* Logical OR operation
		 - OR [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static constexpr void Or(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			Or<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		template<AddressMode MODE, class T, class BLOCK>
		static auto Or_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Or_r<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		/* Xchange
		 - TEST reg32,reg32
		*/
		template<class BLOCK>
		static constexpr void Test(BLOCK& block, Reg32 dst, Reg32 src) {
			common::write_Opcode(block, detail::opcode_TEST.getMain() | 1);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, src, dst);
		}

		template<class BLOCK>
		static auto Test_r(BLOCK& block, Reg32 dst, Reg32 src) {
			common::write_Opcode(block, detail::opcode_TEST.getMain() | 1);
			return write_MOD_REG_RM_ReplaceableReverseRR(block, dst, src);
		}

		/* Logical Compare
		 - TEST reg16,reg16
		*/
		template<class BLOCK>
		static constexpr void Test(BLOCK& block, Reg16 dst, Reg16 src) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, detail::opcode_TEST.getMain() | 1);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, src, dst);
		}

		template<class BLOCK>
		static auto Test_r(BLOCK& block, Reg16 dst, Reg16 src) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, detail::opcode_TEST.getMain() | 1);
			return write_MOD_REG_RM_ReplaceableReverseRR(block, dst, src);
		}

		/* Logical Compare
		 - TEST reg8,reg8
		*/
		template<class BLOCK>
		static constexpr void Test(BLOCK& block, Reg8 dst, Reg8 src) {
			common::write_Opcode(block, detail::opcode_TEST.getMain());
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, src, dst);
		}

		template<class BLOCK>
		static auto Test_r(BLOCK& block, Reg8 dst, Reg8 src) {
			common::write_Opcode(block, detail::opcode_TEST.getMain());
			return write_MOD_REG_RM_ReplaceableReverseRR(block, dst, src);
		}

		/* Logical Compare
		 - TEST reg,imm
		*/
		template<class REG, class T, class BLOCK>
		static constexpr void Test(BLOCK& block, REG reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "Test: First parameter is not register");
			template_reg_imm_operands(block, detail::opcode_TEST, reg, imm);
		}

		template<class REG, class T, class BLOCK>
		static auto Test_r(BLOCK& block, REG reg, const Imm<T>& imm) {
			static_assert(IsRegType<REG>::value, "Test: First parameter is not register");
			return template_reg_imm_operands_r(block, detail::opcode_TEST, reg, imm);
		}

		/* Logical Compare
		 - TEST reg,[mem]
		 - TEST [mem],reg
		*/
		template<class DST, class SRC, class BLOCK>
		static constexpr void Test(BLOCK& block, const DST& dst, const SRC& src) {
			template_2operands_symetric(block, detail::opcode_TEST.getMain(), dst, src);
		}

		template<class DST, class SRC, class BLOCK>
		static auto Test_r(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_symetric_r(block, detail::opcode_TEST.getMain(), dst, src);
		}

		/* Logical Compare
		 - TEST [mem],imm
		 */
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static constexpr void Test(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Test: Invalid size modifier");
			template_mem_imm_operands<SIZE>(block, detail::opcode_TEST, mem, imm);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static auto Test_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Test: Invalid size modifier");
			return template_mem_imm_operands_r<SIZE>(block, detail::opcode_TEST, mem, imm);
		}

		/* Logical Compare
		 - TEST [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static constexpr void Test(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			Test<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		template<AddressMode MODE, class T, class BLOCK>
		static auto Test_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Test_r<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		/* Logical Compare
		 - TEST EAX/AX/AL,imm
		*/
		template<MemSize SIZE, class T, class BLOCK>
		static constexpr void Test(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Test: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Test: Immediate size not equal size modifier");
			template_2operands_first_reg_imm<SIZE>(block, 0xA8, imm);
		}

		template<MemSize SIZE, class T, class BLOCK>
		static auto Test_r(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Test: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Test: Immediate size not equal size modifier");
			return template_2operands_first_reg_imm_r<SIZE>(block, 0xA8, imm);
		}

		/* Logical Exclusive OR
		 - XOR reg,reg
		 - XOR reg,[mem]
		 - XOR [mem],reg
		 - XOR reg,imm
		*/
		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static constexpr void Xor(BLOCK& block, const DST& dst, const SRC& src) {
			template_2operands_opt_imm<L>(block, detail::opcode_XOR, dst, src);
		}

		template<LockPrefix L = NO_LOCK, class DST, class SRC, class BLOCK>
		static auto Xor_r(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_opt_imm_r<L>(block, detail::opcode_XOR, dst, src);
		}

		/* Logical Exclusive OR
		 - XOR EAX/AX/AL,imm
		*/
		template<MemSize SIZE, class T, class BLOCK>
		static constexpr void Xor(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Xor: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Xor: Immediate size not equal size modifier");
			template_2operands_first_reg_imm<SIZE>(block, 0x34, imm);
		}

		template<MemSize SIZE, class T, class BLOCK>
		static auto Xor_r(BLOCK& block, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Xor: Invalid size modifier");
			static_assert(TypeMemSize<Imm<T>>::value == SIZE, "Xor: Immediate size not equal size modifier");
			return template_2operands_first_reg_imm_r<SIZE>(block, 0x34, imm);
		}

		/* Logical Exclusive OR
		 - XOR [mem],imm
		*/
		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static constexpr void Xor(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Xor: Invalid size modifier");
			template_mem_imm_opt_operands<SIZE, L>(block, detail::opcode_XOR, mem, imm);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static auto Xor_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(isGeneralMemSize(SIZE), "Xor: Invalid size modifier");
			return template_mem_imm_opt_operands_r<SIZE, L>(block, detail::opcode_XOR, mem, imm);
		}

		/* Logical Exclusive OR
		 - XOR [mem],imm (auto mem size)
		*/
		template<AddressMode MODE, class T, class BLOCK>
		static constexpr void Xor(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			Xor<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}

		template<AddressMode MODE, class T, class BLOCK>
		static auto Xor_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			return Xor_r<TypeMemSize<Imm<T>>::value>(block, mem, imm);
		}
#pragma endregion
		
#pragma region Bit shift operations [DONE]
		template<MemSize SIZE, Reg8 SH_REG, AddressMode MODE, class BLOCK>
		static constexpr void Rol(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Rol: Invalid size modifier");
			static_assert(SH_REG == CL, "Rol: Invalid shift register");
			template_shift_operands<SIZE, SH_REG>(block, detail::opcode_ROL, mem);
		}

		template<MemSize SIZE, Reg8 SH_REG, AddressMode MODE, class BLOCK>
		static auto Rol_r(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Rol: Invalid size modifier");
			static_assert(SH_REG == CL, "Rol: Invalid shift register");
			return template_shift_operands_r<SIZE, SH_REG>(block, detail::opcode_ROL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static constexpr void Rol(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Rol: Invalid size modifier");
			template_shift_operands<SIZE>(block, detail::opcode_ROL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Rol_r(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Rol: Invalid size modifier");
			return template_shift_operands_r<SIZE>(block, detail::opcode_ROL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static constexpr void Rol(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			static_assert(isGeneralMemSize(SIZE), "Rol: Invalid size modifier");
			template_shift_operands<SIZE>(block, detail::opcode_ROL, mem, imm);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Rol_r(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			static_assert(isGeneralMemSize(SIZE), "Rol: Invalid size modifier");
			return template_shift_operands_r<SIZE>(block, detail::opcode_ROL, mem, imm);
		}

		template<class REG, class BLOCK>
		static constexpr void Rol(BLOCK& block, const REG& reg) {
			static_assert(IsRegType<REG>::value, "Rol: Parameter is not register");
			template_shift_operands<REG>(block, detail::opcode_ROL, reg);
		}

		template<class REG, class BLOCK>
		static auto Rol_r(BLOCK& block, const REG& reg) {
			static_assert(IsRegType<REG>::value, "Rol: Parameter is not register");
			return template_shift_operands_r<REG>(block, detail::opcode_ROL, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static constexpr void Rol(BLOCK& block, const REG& reg) {
			static_assert(SH_REG == CL, "Rol: Invalid shift register");
			static_assert(IsRegType<REG>::value, "Rol: Parameter is not register");
			template_shift_operands<REG, SH_REG>(block, detail::opcode_ROL, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Rol_r(BLOCK& block, const REG& reg) {
			static_assert(SH_REG == CL, "Rol: Invalid shift register");
			static_assert(IsRegType<REG>::value, "Rol: Parameter is not register");
			return template_shift_operands_r<REG, SH_REG>(block, detail::opcode_ROL, reg);
		}

		template<class REG, class BLOCK>
		static constexpr void Rol(BLOCK& block, const REG& reg, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Rol: First parameter is not register");
			template_shift_operands<REG>(block, detail::opcode_ROL, reg, imm);
		}

		template<class REG, class BLOCK>
		static auto Rol_r(BLOCK& block, const REG& reg, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Rol: First parameter is not register");
			return template_shift_operands_r<REG>(block, detail::opcode_ROL, reg, imm);
		}

		template<MemSize SIZE, Reg8 SH_REG, AddressMode MODE, class BLOCK>
		static auto Ror(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Ror: Invalid size modifier");
			static_assert(SH_REG == CL, "Ror: Invalid shift register");
			return template_shift_operands<SIZE, SH_REG>(block, detail::opcode_ROR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Ror(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Ror: Invalid size modifier");
			return template_shift_operands<SIZE>(block, detail::opcode_ROR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Ror(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			static_assert(isGeneralMemSize(SIZE), "Ror: Invalid size modifier");
			return template_shift_operands<SIZE>(block, detail::opcode_ROR, mem, imm);
		}

		template<class REG, class BLOCK>
		static auto Ror(BLOCK& block, const REG& reg) {
			static_assert(IsRegType<REG>::value, "Ror: Parameter is not register");
			return template_shift_operands<REG>(block, detail::opcode_ROR, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Ror(BLOCK& block, const REG& reg) {
			static_assert(SH_REG == CL, "Rol: Invalid shift register");
			static_assert(IsRegType<REG>::value, "Ror: First parameter is not register");
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_ROR, reg);
		}

		template<class REG, class BLOCK>
		static auto Ror(BLOCK& block, const REG& reg, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Ror: First parameter is not register");
			return template_shift_operands<REG>(block, detail::opcode_ROR, reg, imm);
		}

		template<MemSize SIZE, Reg8 SH_REG, AddressMode MODE, class BLOCK>
		static auto Rcl(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Rcl: Invalid size modifier");
			static_assert(SH_REG == CL, "Rcl: Invalid shift register");
			return template_shift_operands<SIZE, SH_REG>(block, detail::opcode_RCL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Rcl(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Rcl: Invalid size modifier");
			return template_shift_operands<SIZE>(block, detail::opcode_RCL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Rcl(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			static_assert(isGeneralMemSize(SIZE), "Rcl: Invalid size modifier");
			return template_shift_operands<SIZE>(block, detail::opcode_RCL, mem, imm);
		}

		template<class REG, class BLOCK>
		static auto Rcl(BLOCK& block, const REG& reg) {
			static_assert(IsRegType<REG>::value, "Rcl: Parameter is not register");
			return template_shift_operands<REG>(block, detail::opcode_RCL, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Rcl(BLOCK& block, const REG& reg) {
			static_assert(SH_REG == CL, "Rcl: Invalid shift register");
			static_assert(IsRegType<REG>::value, "Rcl: First parameter is not register");
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_RCL, reg);
		}

		template<class REG, class BLOCK>
		static auto Rcl(BLOCK& block, const REG& reg, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Rcl: First parameter is not register");
			return template_shift_operands<REG>(block, detail::opcode_RCL, reg, imm);
		}

		template<MemSize SIZE, Reg8 SH_REG, AddressMode MODE, class BLOCK>
		static auto Rcr(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Rcr: Invalid size modifier");
			static_assert(SH_REG == CL, "Rcr: Invalid shift register");
			return template_shift_operands<SIZE, SH_REG>(block, detail::opcode_RCR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Rcr(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Rcr: Invalid size modifier");
			return template_shift_operands<SIZE>(block, detail::opcode_RCR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Rcr(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			static_assert(isGeneralMemSize(SIZE), "Rcr: Invalid size modifier");
			return template_shift_operands<SIZE>(block, detail::opcode_RCR, mem, imm);
		}

		template<class REG, class BLOCK>
		static auto Rcr(BLOCK& block, const REG& reg) {
			static_assert(IsRegType<REG>::value, "Rcr: Parameter is not register");
			return template_shift_operands<REG>(block, detail::opcode_RCR, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Rcr(BLOCK& block, const REG& reg) {
			static_assert(SH_REG == CL, "Rcr: Invalid shift register");
			static_assert(IsRegType<REG>::value, "Rcr: First parameter is not register");
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_RCR, reg);
		}

		template<class REG, class BLOCK>
		static auto Rcr(BLOCK& block, const REG& reg, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Rcr: First parameter is not register");
			return template_shift_operands<REG>(block, detail::opcode_RCR, reg, imm);
		}

		template<MemSize SIZE, Reg8 SH_REG, AddressMode MODE, class BLOCK>
		static auto Sal(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Sal: Invalid size modifier");
			static_assert(SH_REG == CL, "Rcr: Invalid shift register");
			return template_shift_operands<SIZE, SH_REG>(block, detail::opcode_SAL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Sal(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Sal: Invalid size modifier");
			return template_shift_operands<SIZE>(block, detail::opcode_SAL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Sal(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			static_assert(isGeneralMemSize(SIZE), "Sal: Invalid size modifier");
			return template_shift_operands<SIZE>(block, detail::opcode_SAL, mem, imm);
		}

		template<class REG, class BLOCK>
		static auto Sal(BLOCK& block, const REG& reg) {
			static_assert(IsRegType<REG>::value, "Sal: Parameter is not register");
			return template_shift_operands<REG>(block, detail::opcode_SAL, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Sal(BLOCK& block, const REG& reg) {
			static_assert(IsRegType<REG>::value, "Sal: First parameter is not register");
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_SAL, reg);
		}

		template<class REG, class BLOCK>
		static auto Sal(BLOCK& block, const REG& reg, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Sal: First parameter is not register");
			return template_shift_operands<REG>(block, detail::opcode_SAL, reg, imm);
		}

		template<MemSize SIZE, Reg8 SH_REG, AddressMode MODE, class BLOCK>
		static auto Shl(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Shl: Invalid size modifier");
			static_assert(SH_REG == CL, "Shl: Invalid shift register");
			return template_shift_operands<SIZE, SH_REG>(block, detail::opcode_SHL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Shl(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Shl: Invalid size modifier");
			return template_shift_operands<SIZE>(block, detail::opcode_SHL, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Shl(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			static_assert(isGeneralMemSize(SIZE), "Shl: Invalid size modifier");
			return template_shift_operands<SIZE>(block, detail::opcode_SHL, mem, imm);
		}

		template<class REG, class BLOCK>
		static auto Shl(BLOCK& block, const REG& reg) {
			static_assert(IsRegType<REG>::value, "Shl: Parameter is not register");
			return template_shift_operands<REG>(block, detail::opcode_SHL, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Shl(BLOCK& block, const REG& reg) {
			static_assert(SH_REG == CL, "Shl: Invalid shift register");
			static_assert(IsRegType<REG>::value, "Shl: First parameter is not register");
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_SHL, reg);
		}

		template<class REG, class BLOCK>
		static auto Shl(BLOCK& block, const REG& reg, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Shl: First parameter is not register");
			return template_shift_operands<REG>(block, detail::opcode_SHL, reg, imm);
		}

		template<MemSize SIZE, Reg8 SH_REG, AddressMode MODE, class BLOCK>
		static auto Shr(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Shr: Invalid size modifier");
			static_assert(SH_REG == CL, "Shr: Invalid shift register");
			return template_shift_operands<SIZE, SH_REG>(block, detail::opcode_SHR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Shr(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Shr: Invalid size modifier");
			return template_shift_operands<SIZE>(block, detail::opcode_SHR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Shr(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			static_assert(isGeneralMemSize(SIZE), "Shr: Invalid size modifier");
			return template_shift_operands<SIZE>(block, detail::opcode_SHR, mem, imm);
		}

		template<class REG, class BLOCK>
		static auto Shr(BLOCK& block, const REG& reg) {
			static_assert(IsRegType<REG>::value, "Shr: Parameter is not register");
			return template_shift_operands<REG>(block, detail::opcode_SHR, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Shr(BLOCK& block, const REG& reg) {
			static_assert(SH_REG == CL, "Shr: Invalid shift register");
			static_assert(IsRegType<REG>::value, "Shr: First parameter is not register");
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_SHR, reg);
		}

		template<class REG, class BLOCK>
		static auto Shr(BLOCK& block, const REG& reg, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Shr: First parameter is not register");
			return template_shift_operands<REG>(block, detail::opcode_SHR, reg, imm);
		}

		template<MemSize SIZE, Reg8 SH_REG, AddressMode MODE, class BLOCK>
		static auto Sar(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Sar: Invalid size modifier");
			static_assert(SH_REG == CL, "Sar: Invalid shift register");
			return template_shift_operands<SIZE, SH_REG>(block, detail::opcode_SAR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Sar(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(isGeneralMemSize(SIZE), "Sar: Invalid size modifier");
			return template_shift_operands<SIZE>(block, detail::opcode_SAR, mem);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Sar(BLOCK& block, const Mem32<MODE>& mem, const U8& imm) {
			static_assert(isGeneralMemSize(SIZE), "Sar: Invalid size modifier");
			return template_shift_operands<SIZE>(block, detail::opcode_SAR, mem, imm);
		}

		template<class REG, class BLOCK>
		static auto Sar(BLOCK& block, const REG& reg) {
			static_assert(IsRegType<REG>::value, "Sar: Parameter is not register");
			return template_shift_operands<REG>(block, detail::opcode_SAR, reg);
		}

		template<Reg8 SH_REG, class REG, class BLOCK>
		static auto Sar(BLOCK& block, const REG& reg) {
			static_assert(SH_REG == CL, "Sar: Invalid shift register");
			static_assert(IsRegType<REG>::value, "Sar: First parameter is not register");
			return template_shift_operands<REG, SH_REG>(block, detail::opcode_SAR, reg);
		}

		template<class REG, class BLOCK>
		static auto Sar(BLOCK& block, const REG& reg, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Sar: First parameter is not register");
			return template_shift_operands<REG>(block, detail::opcode_SAR, reg, imm);
		}

		template<MemSize SIZE, Reg8 COUNT_REG, class REG, AddressMode MODE, class BLOCK>
		static constexpr void Shrd(BLOCK& block, const Mem32<MODE>& mem, REG reg) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Shrd: Invalid size modifier");
			static_assert(COUNT_REG == CL, "Shrd: Invalid count register");
			static_assert(IsRegType<REG>::value, "Shrd: Second parameter is not register");
			template_prshift_operands<SIZE, COUNT_REG>(block, 0xAC, mem, reg);
		}

		template<MemSize SIZE, Reg8 COUNT_REG, class REG, AddressMode MODE, class BLOCK>
		static auto Shrd_r(BLOCK& block, const Mem32<MODE>& mem, REG reg) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Shrd: Invalid size modifier");
			static_assert(COUNT_REG == CL, "Shrd: Invalid count register");
			static_assert(IsRegType<REG>::value, "Shrd: Second parameter is not register");
			return template_prshift_operands_r<SIZE, COUNT_REG>(block, 0xAC, mem, reg);
		}

		template<Reg8 COUNT_REG, class REG, class BLOCK>
		static constexpr void Shrd(BLOCK& block, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Shrd: First and second parameter is not register");
			template_prshift_operands<COUNT_REG>(block, 0xAC, reg1, reg2);
		}

		template<Reg8 COUNT_REG, class REG, class BLOCK>
		static auto Shrd_r(BLOCK& block, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Shrd: First and second parameter is not register");
			return template_prshift_operands_r<COUNT_REG>(block, 0xAC, reg1, reg2);
		}

		template<MemSize SIZE, class REG, AddressMode MODE, class BLOCK>
		static constexpr void Shrd(BLOCK& block, const Mem32<MODE>& mem, REG reg, const U8& imm) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Shrd: Invalid size modifier");
			static_assert(IsRegType<REG>::value, "Shrd: Second parameter is not register");
			template_prshift_operands<SIZE>(block, 0xAC, mem, reg, imm);
		}

		template<MemSize SIZE, class REG, AddressMode MODE, class BLOCK>
		static auto Shrd_r(BLOCK& block, const Mem32<MODE>& mem, REG reg, const U8& imm) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Shrd: Invalid size modifier");
			static_assert(IsRegType<REG>::value, "Shrd: Second parameter is not register");
			return template_prshift_operands_r<SIZE>(block, 0xAC, mem, reg, imm);
		}

		template<class REG, class BLOCK>
		static constexpr void Shrd(BLOCK& block, REG reg1, REG reg2, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Shrd: First and second parameter is not register");
			template_prshift_operands(block, 0xAC, reg1, reg2, imm);
		}

		template<class REG, class BLOCK>
		static auto Shrd_r(BLOCK& block, REG reg1, REG reg2, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Shrd: First and second parameter is not register");
			return template_prshift_operands_r(block, 0xAC, reg1, reg2, imm);
		}

		template<MemSize SIZE, Reg8 COUNT_REG, class REG, AddressMode MODE, class BLOCK>
		static constexpr void Shld(BLOCK& block, const Mem32<MODE>& mem, REG reg) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Shld: Invalid size modifier");
			static_assert(COUNT_REG == CL, "Shld: Invalid count register");
			static_assert(IsRegType<REG>::value, "Shld: Second parameter is not register");
			template_prshift_operands<SIZE, COUNT_REG>(block, 0xA4, mem, reg);
		}

		template<MemSize SIZE, Reg8 COUNT_REG, class REG, AddressMode MODE, class BLOCK>
		static auto Shld_r(BLOCK& block, const Mem32<MODE>& mem, REG reg) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Shld: Invalid size modifier");
			static_assert(COUNT_REG == CL, "Shld: Invalid count register");
			static_assert(IsRegType<REG>::value, "Shld: Second parameter is not register");
			return template_prshift_operands_r<SIZE, COUNT_REG>(block, 0xA4, mem, reg);
		}

		template<Reg8 COUNT_REG, class REG, class BLOCK>
		static constexpr void Shld(BLOCK& block, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Shld: First and second parameter is not register");
			template_prshift_operands<COUNT_REG>(block, 0xA4, reg1, reg2);
		}

		template<Reg8 COUNT_REG, class REG, class BLOCK>
		static auto Shld_r(BLOCK& block, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Shld: First and second parameter is not register");
			return template_prshift_operands_r<COUNT_REG>(block, 0xA4, reg1, reg2);
		}

		template<MemSize SIZE, class REG, AddressMode MODE, class BLOCK>
		static constexpr void Shld(BLOCK& block, const Mem32<MODE>& mem, REG reg, const U8& imm) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Shld: Invalid size modifier");
			static_assert(IsRegType<REG>::value, "Shld: Second parameter is not register");
			template_prshift_operands<SIZE>(block, 0xA4, mem, reg, imm);
		}

		template<MemSize SIZE, class REG, AddressMode MODE, class BLOCK>
		static auto Shld_r(BLOCK& block, const Mem32<MODE>& mem, REG reg, const U8& imm) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Shld: Invalid size modifier");
			static_assert(IsRegType<REG>::value, "Shld: Second parameter is not register");
			return template_prshift_operands_r<SIZE>(block, 0xA4, mem, reg, imm);
		}

		template<class REG, class BLOCK>
		static constexpr void Shld(BLOCK& block, REG reg1, REG reg2, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Shld: First and second parameter is not register");
			template_prshift_operands(block, 0xA4, reg1, reg2, imm);
		}

		template<class REG, class BLOCK>
		static auto Shld_r(BLOCK& block, REG reg1, REG reg2, const U8& imm) {
			static_assert(IsRegType<REG>::value, "Shld: First and second parameter is not register");
			return template_prshift_operands_r(block, 0xA4, reg1, reg2, imm);
		}
#pragma endregion

#pragma region Bit manipulations [DONE]
		template<class REG, class BLOCK>
		static constexpr void Bsf(BLOCK& block, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Bsf: First and second parameter is not register");
			template_bit_scan(block, 0xBC, reg1, reg2);
		}

		template<class REG, class BLOCK>
		static auto Bsf_r(BLOCK& block, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Bsf: First and second parameter is not register");
			return template_bit_scan_r(block, 0xBC, reg1, reg2);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static constexpr void Bsf(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Bsf: First parameter is not register");
			template_bit_scan(block, 0xBC, reg, mem);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static auto Bsf_r(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Bsf: First parameter is not register");
			return template_bit_scan_r(block, 0xBC, reg, mem);
		}

		template<class REG, class BLOCK>
		static constexpr void Bsr(BLOCK& block, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Bsr: First and second parameter is not register");
			template_bit_scan(block, 0xBD, reg1, reg2);
		}

		template<class REG, class BLOCK>
		static auto Bsr_r(BLOCK& block, REG reg1, REG reg2) {
			static_assert(IsRegType<REG>::value, "Bsr: First and second parameter is not register");
			return template_bit_scan_r(block, 0xBD, reg1, reg2);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static constexpr void Bsr(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Bsr: First parameter is not register");
			template_bit_scan(block, 0xBD, reg, mem);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static auto Bsr_r(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Bsr: First parameter is not register");
			return template_bit_scan_r(block, 0xBD, reg, mem);
		}

		template<class T1, class T2, class BLOCK>
		static constexpr void Bt(BLOCK& block, T1 val1, T2 val2) {
			template_bit_operation(block, detail::opcode_BT, val1, val2);
		}

		template<class T1, class T2, class BLOCK>
		static auto Bt_r(BLOCK& block, T1 val1, T2 val2) {
			return template_bit_operation_r(block, detail::opcode_BT, val1, val2);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static constexpr void Bt(BLOCK& block, const Mem32<MODE>& mem, U8 imm) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Bt: Invalid size modifier");
			template_bit_operation<NO_LOCK, SIZE>(block, detail::opcode_BT, mem, imm);
		}

		template<MemSize SIZE, AddressMode MODE, class BLOCK>
		static auto Bt_r(BLOCK& block, const Mem32<MODE>& mem, U8 imm) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Bt: Invalid size modifier");
			return template_bit_operation_r<NO_LOCK, SIZE>(block, detail::opcode_BT, mem, imm);
		}

		template<LockPrefix L = NO_LOCK, class T1, class T2, class BLOCK>
		static constexpr void Btc(BLOCK& block, T1 val1, T2 val2) {
			template_bit_operation<L>(block, detail::opcode_BTC, val1, val2);
		}

		template<LockPrefix L = NO_LOCK, class T1, class T2, class BLOCK>
		static auto Btc_r(BLOCK& block, T1 val1, T2 val2) {
			return template_bit_operation_r<L>(block, detail::opcode_BTC, val1, val2);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static constexpr void Btc(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Btc: Invalid size modifier");
			template_bit_operation<L, SIZE>(block, detail::opcode_BTC, mem, imm);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static auto Btc_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Btc: Invalid size modifier");
			return template_bit_operation_r<L, SIZE>(block, detail::opcode_BTC, mem, imm);
		}

		template<LockPrefix L = NO_LOCK, class T1, class T2, class BLOCK>
		static constexpr void Btr(BLOCK& block, T1 val1, T2 val2) {
			template_bit_operation<L>(block, detail::opcode_BTR, val1, val2);
		}

		template<LockPrefix L = NO_LOCK, class T1, class T2, class BLOCK>
		static auto Btr_r(BLOCK& block, T1 val1, T2 val2) {
			return template_bit_operation_r<L>(block, detail::opcode_BTR, val1, val2);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static constexpr void Btr(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Btr: Invalid size modifier");
			template_bit_operation<L, SIZE>(block, detail::opcode_BTR, mem, imm);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static auto Btr_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Btr: Invalid size modifier");
			return template_bit_operation_r<L, SIZE>(block, detail::opcode_BTR, mem, imm);
		}

		template<LockPrefix L = NO_LOCK, class T1, class T2, class BLOCK>
		static constexpr void Bts(BLOCK& block, T1 val1, T2 val2) {
			template_bit_operation<L>(block, detail::opcode_BTS, val1, val2);
		}

		template<LockPrefix L = NO_LOCK, class T1, class T2, class BLOCK>
		static auto Bts_r(BLOCK& block, T1 val1, T2 val2) {
			return template_bit_operation_r<L>(block, detail::opcode_BTS, val1, val2);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static constexpr void Bts(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Bts: Invalid size modifier");
			template_bit_operation<L, SIZE>(block, detail::opcode_BTS, mem, imm);
		}

		template<MemSize SIZE, LockPrefix L = NO_LOCK, AddressMode MODE, class T, class BLOCK>
		static auto Bts_r(BLOCK& block, const Mem32<MODE>& mem, const Imm<T>& imm) {
			static_assert(SIZE == WORD_PTR || SIZE == DWORD_PTR, "Bts: Invalid size modifier");
			return template_bit_operation_r<L, SIZE>(block, detail::opcode_BTS, mem, imm);
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
		 - CALL reg (near only)
		 - CALL [mem] (near/far)
		*/
		template<MemSize SIZE = DWORD_PTR, class T, class BLOCK>
		static auto Call(BLOCK& block, const T& addr) {
			return template_Call<SIZE>(block, addr);
		}

		/* Call Near procedure
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

		/* Call Far procedure
		 - CALL imm16:32
		*/
		template<class BLOCK>
		static constexpr void Call(BLOCK& block, S16 sel, S32 addr) {
			common::write_Opcode(block, 0x9A);
			common::write_Immediate(block, addr);
			common::write_Immediate(block, sel);
		}

		/* Jump always
		 - JMP reg
		 - JMP [mem]
		*/
		template<MemSize SIZE = DWORD_PTR, class T, class BLOCK>
		static auto Jmp(BLOCK& block, const T& addr) {
			return template_Jmp<SIZE>(block, addr);
		}

		/* Jump always
		 - JMP imm_addr
		 - JMP label
		*/
		template<JmpSize SIZE = SHORT, class BLOCK>
		static FwdLabel<SIZE> Jmp(BLOCK& block) {
			return template_Jmp<SIZE>(block);
		}

		/* Jump Far procedure
		 - JMP imm16:32
		*/
		template<class BLOCK>
		static constexpr void Jmp(BLOCK& block, S16 sel, S32 addr) {
			common::write_Opcode(block, 0xEA);
			common::write_Immediate(block, addr);
			common::write_Immediate(block, sel);
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
		static constexpr void Int(BLOCK& block, U8 number) {
			common::write_Opcode(block, 0xCD);
			common::write_Immediate(block, number);
		}

		template<class BLOCK>
		static ReplaceableImmediate<U8::type> Int_r(BLOCK& block, U8 number) {
			common::write_Opcode(block, 0xCD);
			return write_Immediate_Replaceable(block, number);
		}

		/* Launch interrupt number 3 */
		template<class BLOCK>
		static constexpr void Int3(BLOCK& block) {
			common::write_Opcode(block, 0xCC);
		}

		/* Launch interrupt number 4 if overflow flag (OF) is set */
		template<class BLOCK>
		static constexpr void IntO(BLOCK& block) {
			common::write_Opcode(block, 0xCE);
		}

		/* Return from interrupt */
		template<class BLOCK>
		static constexpr void Iret(BLOCK& block) {
			common::write_Opcode(block, 0xCF);
		}

		/* Create enter procedure stack frame */
		template<class BLOCK>
		static constexpr void Enter(BLOCK& block, U16 size, U8 level) {
			common::write_Opcode(block, 0xC8);
			common::write_Immediate(block, size);
			common::write_Immediate(block, level);
		}

		template<class BLOCK>
		static auto Enter_r(BLOCK& block, U16 size, U8 level) {
			common::write_Opcode(block, 0xC8);
			auto replaceableSize = write_Immediate_Replaceable(block, size);
			auto replaceableLevel = write_Immediate_Replaceable(block, level);
			return std::make_pair(replaceableSize, replaceableLevel);
		}

		/* Create leave procedure stack frame */
		template<class BLOCK>
		static constexpr void Leave(BLOCK& block) {
			common::write_Opcode(block, 0xC9);
		}

		/* Return from near procedure */
		template<class BLOCK>
		static constexpr void Ret(BLOCK& block) {
			common::write_Opcode(block, 0xC3);
		}

		/* Return from near procedure */
		template<class BLOCK>
		static constexpr void Ret(BLOCK& block, U16 number) {
			common::write_Opcode(block, 0xC2);
			common::write_Immediate(block, number);
		}

		template<class BLOCK>
		static ReplaceableImmediate<U16::type> Ret_r(BLOCK& block, U16 number) {
			common::write_Opcode(block, 0xC2);
			return write_Immediate_Replaceable(block, number);
		}

		/* Return from near procedure */
		template<class BLOCK>
		static constexpr void Retn(BLOCK& block) {
			Ret(block);
		}

		/* Return from near procedure */
		template<class BLOCK>
		static constexpr void Retn(BLOCK& block, U16 number) {
			Ret(block, number);
		}

		template<class BLOCK>
		static ReplaceableImmediate<U16::type> Retn_r(BLOCK& block, U16 number) {
			return Ret_r(block, number);
		}

		/* Return from far procedure */
		template<class BLOCK>
		static constexpr void Retf(BLOCK& block) {
			common::write_Opcode(block, 0xCB);
		}

		/* Return from far procedure */
		template<class BLOCK>
		static constexpr void Retf(BLOCK& block, U16 number) {
			common::write_Opcode(block, 0xCA);
			common::write_Immediate(block, number);
		}

		template<class BLOCK>
		static ReplaceableImmediate<U16::type> Retf_r(BLOCK& block, U16 number) {
			common::write_Opcode(block, 0xCA);
			return write_Immediate_Replaceable(block, number);
		}

		/* Jump if ECX == 0 */
		template<class BLOCK>
		static FwdLabel<SHORT> Jecxz(BLOCK& block) {
			return template_Loop<SHORT>(block, 0xE3);
		}

		/* Jump if CX == 0 */
		template<class BLOCK>
		static FwdLabel<SHORT> Jcxz(BLOCK& block) {
			block.template pushRaw<byteOrder, uint8_t>(0x67);
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
		static constexpr void Clc(BLOCK& block) {
			common::write_Opcode(block, 0xF8);
		}

		/* Reset direction flag (DF) */
		template<class BLOCK>
		static constexpr void Cld(BLOCK& block) {
			common::write_Opcode(block, 0xFC);
		}

		/* Clear interrupt flag (IF) */
		template<class BLOCK>
		static constexpr void Cli(BLOCK& block) {
			common::write_Opcode(block, 0xFA);
		}

		/* Invert carry flag (CF) */
		template<class BLOCK>
		static constexpr void Cmc(BLOCK& block) {
			common::write_Opcode(block, 0xF5);
		}

		/* Load flags to register AH */
		template<class BLOCK>
		static constexpr void Lahf(BLOCK& block) {
			common::write_Opcode(block, 0x9F);
		}

		/* Save FLAGS to stack */
		template<class BLOCK>
		static constexpr void Pushf(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x9C);
		}

		/* Save EFLAGS to stack */
		template<class BLOCK>
		static constexpr void Pushfd(BLOCK& block) {
			common::write_Opcode(block, 0x9C);
		}

		/* Load FLAGS from stack */
		template<class BLOCK>
		static constexpr void Popf(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x9D);
		}

		/* Load EFLAGS from stack */
		template<class BLOCK>
		static constexpr void Popfd(BLOCK& block) {
			common::write_Opcode(block, 0x9D);
		}

		/* Store register AH to flags */
		template<class BLOCK>
		static constexpr void Sahf(BLOCK& block) {
			common::write_Opcode(block, 0x9E);
		}

		/* Set all register AL bits according CF flag 
		 - Remark: Not documented Intel instruction
		*/
		template<class BLOCK>
		static constexpr void Salc(BLOCK& block) {
			common::write_Opcode(block, 0xD6);
		}

		/* Set carry flag (CF) */
		template<class BLOCK>
		static constexpr void Stc(BLOCK& block) {
			common::write_Opcode(block, 0xF9);
		}

		/* Set direction flag (DF) */
		template<class BLOCK>
		static constexpr void Std(BLOCK& block) {
			common::write_Opcode(block, 0xFD);
		}

		/* Set interrupt flag (IF) */
		template<class BLOCK>
		static constexpr void Sti(BLOCK& block) {
			common::write_Opcode(block, 0xFB);
		}
#pragma endregion

#pragma region String operations

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Insb(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0x6C);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Insw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Rep<R>(block, 0x6D);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Insd(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0x6D);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Outsb(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0x6E);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Outsw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Rep<R>(block, 0x6F);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Outsd(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0x6F);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Lodsb(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xAC);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Lodsw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Rep<R>(block, 0xAD);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Lodsd(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xAD);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Movsb(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xA4);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Movsw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0xA5);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Movsd(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xA5);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Scasb(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xAE);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Scasw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Rep<R>(block, 0xAF);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Scasd(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xAF);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Stosb(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xAA);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Stosw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Rep<R>(block, 0xAB);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Stosd(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xAB);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Cmpsb(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xA6);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Cmpsw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Rep<R>(block, 0xA7);
		}

		template<RepPrefix R = NO_REP, class BLOCK>
		static constexpr void Cmpsd(BLOCK& block) {
			common::write_Opcode_Rep<R>(block, 0xA7);
		}

#pragma endregion

#pragma region Load segment registers
		template<class REG, AddressMode MODE, class BLOCK>
		static constexpr void Lds(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Lds: First parameter is not register");
			template_LoadSegReg(block, 0xC5, reg, mem);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static auto Lds_r(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Lds: First parameter is not register");
			return template_LoadSegReg_r(block, 0xC5, reg, mem);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static constexpr void Les(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Les: First parameter is not register");
			template_LoadSegReg(block, 0xC4, reg, mem);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static auto Les_r(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Les: First parameter is not register");
			return template_LoadSegReg_r(block, 0xC4, reg, mem);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static constexpr void Lfs(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Lfs: First parameter is not register");
			template_LoadSegRegEx(block, 0xB4, reg, mem);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static auto Lfs_r(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Lfs: First parameter is not register");
			return template_LoadSegRegEx_r(block, 0xB4, reg, mem);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static constexpr void Lgs(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Lgs: First parameter is not register");
			template_LoadSegRegEx(block, 0xB5, reg, mem);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static auto Lgs_r(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Lgs: First parameter is not register");
			return template_LoadSegRegEx_r(block, 0xB5, reg, mem);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static constexpr void Lss(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Lss: First parameter is not register");
			template_LoadSegRegEx(block, 0xB2, reg, mem);
		}

		template<class REG, AddressMode MODE, class BLOCK>
		static auto Lss_r(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			static_assert(IsRegType<REG>::value, "Lss: First parameter is not register");
			return template_LoadSegRegEx_r(block, 0xB2, reg, mem);
		}
#pragma endregion

#pragma region Other operations
		template<AddressMode MODE, class BLOCK>
		static constexpr void Bound(BLOCK& block, Reg16 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x62);
			mem.write(block, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void Bound(BLOCK& block, Reg32 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, 0x62);
			mem.write(block, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static auto Bound_r(BLOCK& block, Reg16 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x62);
			return mem.writeReplaceable(block, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static auto Bound_r(BLOCK& block, Reg32 reg, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, 0x62);
			return mem.writeReplaceable(block, reg);
		}

		/* No operation */
		template<class BLOCK>
		static constexpr void Nop(BLOCK& block) {
			common::write_Opcode(block, 0x90);
		}

		/* No operation */
		template<class BLOCK>
		static void Nop(BLOCK& block, Size count) {
			for (; count > 0; count--) {
				common::write_Opcode(block, 0x90);
			}
		}

		/* Halt processor */
		template<class BLOCK>
		static constexpr void Hlt(BLOCK& block) {
			common::write_Opcode(block, 0xF4);
		}
#pragma endregion

#pragma region Priviledged system instructions
		/* Adjust RPL Field of Segment Selector
		 - ARPL reg,reg 
		*/
		template<class BLOCK>
		static constexpr void Arpl(BLOCK& block, Reg16 reg1, Reg16 reg2) {
			common::write_Opcode(block, 0x63);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, reg2, reg1);
		}

		template<class BLOCK>
		static auto Arpl_r(BLOCK& block, Reg16 reg1, Reg16 reg2) {
			common::write_Opcode(block, 0x63);
			return write_MOD_REG_RM_ReplaceableRR(block, reg2, reg1);
		}

		/* Adjust RPL Field of Segment Selector 
		 - ARPL [mem],reg 
		*/
		template<AddressMode MODE, class BLOCK>
		static constexpr void Arpl(BLOCK& block, const Mem32<MODE>& mem, Reg16 reg) {
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, 0x63);
			mem.write(block, reg);
		}

		template<AddressMode MODE, class BLOCK>
		static auto Arpl_r(BLOCK& block, const Mem32<MODE>& mem, Reg16 reg) {
			mem.writeSegmPrefix(block);
			common::write_Opcode(block, 0x63);
			return mem.writeReplaceable(block, reg);
		}

		/* Clears Task-Switched Flag in CR0. */
		template<class BLOCK>
		static constexpr void Clts(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x06);
		}

		template<class BLOCK>
		static constexpr void Lar(BLOCK& block, Reg16 reg1, Reg16 reg2) {
			template_2_operands_ext(block, 0x02, reg1, reg2);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void Lar(BLOCK& block, Reg16 reg, const Mem32<MODE>& mem) {
			template_2_operands_ext(block, 0x02, reg, mem);
		}

		template<class BLOCK>
		static constexpr void Lar(BLOCK& block, Reg32 reg1, Reg32 reg2) {
			template_2_operands_ext(block, 0x02, reg1, reg2);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void Lar(BLOCK& block, Reg32 reg, const Mem32<MODE>& mem) {
			template_2_operands_ext(block, 0x02, reg, mem);
		}

		/* Load mem into GDTR */
		template<AddressMode MODE, class BLOCK>
		static constexpr void Lgdt(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_LGDT, mem);
		}

		/* Load mem into LIDT */
		template<AddressMode MODE, class BLOCK>
		static constexpr void Lidt(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_LIDT, mem);
		}

		/* Load mem into LLDT */
		template<AddressMode MODE, class BLOCK>
		static constexpr void Lldt(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_LLDT, mem);
		}

		/* Load register into LLDT */
		template<class BLOCK>
		static constexpr void Lldt(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_LLDT, (Reg32)reg);
		}

		/* Load mem in machine status word of CR0 */
		template<AddressMode MODE, class BLOCK>
		static constexpr void Lmsw(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_LMSW, mem);
		}

		/* Load register in machine status word of CR0 */
		template<class BLOCK>
		static constexpr void Lmsw(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_LMSW, (Reg32)reg);
		}

		template<class BLOCK>
		static constexpr void Lsl(BLOCK& block, Reg16 reg1, Reg16 reg2) {
			template_2_operands_ext(block, 0x03, reg1, reg2);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void Lsl(BLOCK& block, Reg16 reg, const Mem32<MODE>& mem) {
			template_2_operands_ext(block, 0x03, reg, mem);
		}

		template<class BLOCK>
		static constexpr void Lsl(BLOCK& block, Reg32 reg1, Reg32 reg2) {
			template_2_operands_ext(block, 0x03, reg1, reg2);
		}

		template<AddressMode MODE, class BLOCK>
		static constexpr void Lsl(BLOCK& block, Reg32 reg, const Mem32<MODE>& mem) {
			template_2_operands_ext(block, 0x03, reg, mem);
		}

		/* Load task register */
		template<AddressMode MODE, class BLOCK>
		static constexpr void Ltr(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_LTR, mem);
		}

		/* Load task register */
		template<class BLOCK>
		static constexpr void Ltr(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_LTR, (Reg32)reg);
		}

		/* Store global descriptor table */
		template<AddressMode MODE, class BLOCK>
		static constexpr void Sgdt(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_SGDT, mem);
		}

		/* Store interrupt descriptor table */
		template<AddressMode MODE, class BLOCK>
		static constexpr void Sidt(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_SIDT, mem);
		}

		/* Store local descriptor table */
		template<AddressMode MODE, class BLOCK>
		static constexpr void Sldt(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_SLDT, mem);
		}

		/* Store local descriptor table */
		template<class BLOCK>
		static constexpr void Sldt(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_SLDT, (Reg32)reg);
		}

		/* Store Machine Status Word */
		template<AddressMode MODE, class BLOCK>
		static constexpr void Smsw(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_SMSW, mem);
		}

		/* Store Machine Status Word */
		template<class BLOCK>
		static constexpr void Smsw(BLOCK& block, Reg32 reg) {
			template_1reg_ext_operand(block, detail::opcode_SMSW, reg);
		}

		/* Store Machine Status Word */
		template<class BLOCK>
		static constexpr void Smsw(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_SMSW, reg);
		}

		/* Store Task Register */
		template<AddressMode MODE, class BLOCK>
		static constexpr void Str(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_STR, mem);
		}

		/* Store Task Register */
		template<class BLOCK>
		static constexpr void Str(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_STR, (Reg32)reg);
		}

		/* Verify a Segment for Reading */
		template<AddressMode MODE, class BLOCK>
		static constexpr void Verr(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_VERR, mem);
		}

		/* Verify a Segment for Reading */
		template<class BLOCK>
		static constexpr void Verr(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_VERR, (Reg32)reg);
		}

		/* Verify a Segment for Writing */
		template<AddressMode MODE, class BLOCK>
		static constexpr void Verw(BLOCK& block, const Mem32<MODE>& mem) {
			template_1mem_ext_operand<DWORD_PTR>(block, detail::opcode_VERW, mem);
		}

		/* Verify a Segment for Writing */
		template<class BLOCK>
		static constexpr void Verw(BLOCK& block, Reg16 reg) {
			template_1reg_ext_operand(block, detail::opcode_VERW, (Reg32)reg);
		}
#pragma endregion
	};
}
