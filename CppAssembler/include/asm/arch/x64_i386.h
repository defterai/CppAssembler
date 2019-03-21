#pragma once


#include "x64_base.h"

namespace CppAsm::X64
{
	namespace
	{
		namespace common = CppAsm::X86_64::detail;
	}

	class i386 {
	protected:
		enum OpMode : uint8_t {
			MODE_RR = 0b0,
			MODE_RM = 0b0,
			MODE_MR = 0b1
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

			static void write(BLOCK& block, common::Opcode opcode, OpMode rmMode, uint8_t extRegsMask) {
				if (extRegsMask) {
					block.pushRaw<uint8_t>(0x40 | extRegsMask);
				}
				common::write_Opcode(block, opcode | (rmMode << 1));
			}
		};

		template<class BLOCK>
		struct OpcodeWriter<WORD_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode_16bit_Prefix(block);
				common::write_Opcode(block, opcode | 0b1);
			}

			static void write(BLOCK& block, common::Opcode opcode, OpMode rmMode, uint8_t extRegsMask) {
				common::write_Opcode_16bit_Prefix(block);
				if (extRegsMask) {
					block.pushRaw<uint8_t>(0x40 | extRegsMask);
				}
				common::write_Opcode(block, opcode | (rmMode << 1) | 0b1);
			}
		};

		template<class BLOCK>
		struct OpcodeWriter<DWORD_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode) {
				common::write_Opcode(block, opcode | 0b1);
			}

			static void write(BLOCK& block, common::Opcode opcode, OpMode rmMode, uint8_t extRegsMask) {
				if (extRegsMask) {
					block.pushRaw<uint8_t>(0x40 | extRegsMask);
				}
				common::write_Opcode(block, opcode | (rmMode << 1) | 0b1);
			}
		};

		template<class BLOCK>
		struct OpcodeWriter<QWORD_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode) {
				block.pushRaw<uint8_t>(0x48);
				common::write_Opcode(block, opcode | 0b1);
			}

			static void write(BLOCK& block, common::Opcode opcode, OpMode rmMode, uint8_t extRegsMask) {
				block.pushRaw<uint8_t>(0x48 | extRegsMask);
				common::write_Opcode(block, opcode | (rmMode << 1) | 0b1);
			}
		};

		template<MemSize SIZE, class BLOCK>
		static void write_Opcode(BLOCK& block, common::Opcode opcode) {
			OpcodeWriter<MemSize, BLOCK>::write(block, opcode);
		}

		template<MemSize SIZE, class BLOCK>
		static void write_Opcode(BLOCK& block, common::Opcode opcode, OpMode rmMode, uint8_t extRegsMask) {
			OpcodeWriter<MemSize, BLOCK>::write(block, opcode, rmMode, extRegsMask);
		}

		/*template<MemSize SIZE>
		static void write_Opcode_Only_Extended_Prefixs(Os::CodeBlock& block, detail::Opcode opcode);

		template<>
		static void write_Opcode_Only_Extended_Prefixs<WORD_PTR>(Os::CodeBlock& block, detail::Opcode opcode) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Operand_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
		}

		template<>
		static void write_Opcode_Only_Extended_Prefixs<DWORD_PTR>(Os::CodeBlock& block, detail::Opcode opcode) {
			common::write_Operand_Extended_Prefix(block);
			common::write_Opcode(block, opcode);
		}

		template<MemSize SIZE, MemSize OPT_SIZE>
		static void write_Opcode_Imm_Optimized(Os::CodeBlock& block, detail::Opcode opcode);

		template<>
		static void write_Opcode_Imm_Optimized<BYTE_PTR, BYTE_PTR>(Os::CodeBlock& block, detail::Opcode opcode) {
			common::write_Opcode(block, opcode);
		}

		template<>
		static void write_Opcode_Imm_Optimized<WORD_PTR, WORD_PTR>(Os::CodeBlock& block, detail::Opcode opcode) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, opcode | 0b1);
		}

		template<>
		static void write_Opcode_Imm_Optimized<WORD_PTR, BYTE_PTR>(Os::CodeBlock& block, detail::Opcode opcode) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block,opcode | 0b11);
		}

		template<>
		static void write_Opcode_Imm_Optimized<DWORD_PTR, DWORD_PTR>(Os::CodeBlock& block, detail::Opcode opcode) {
			common::write_Opcode(block,opcode | 0b1);
		}

		template<>
		static void write_Opcode_Imm_Optimized<DWORD_PTR, WORD_PTR>(Os::CodeBlock& block, detail::Opcode opcode) {
			common::write_Opcode(block,opcode | 0b1);
		}

		template<>
		static void write_Opcode_Imm_Optimized<DWORD_PTR, BYTE_PTR>(Os::CodeBlock& block, detail::Opcode opcode) {
			common::write_Opcode(block,opcode | 0b11);
		}*/

		template<MemSize DST_SIZE, MemSize SRC_SIZE, class BLOCK>
		struct ExtendedOpcodeWriter;

		template<class BLOCK>
		struct ExtendedOpcodeWriter<WORD_PTR, BYTE_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode, OpMode rmMode, uint8_t extRegsMask) {
				common::write_Opcode_16bit_Prefix(block);
				if (extRegsMask) {
					block.pushRaw<uint8_t>(0x40 | extRegsMask);
				}
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode | (rmMode << 1));
			}
		};

		template<class BLOCK>
		struct ExtendedOpcodeWriter<WORD_PTR, WORD_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode, OpMode rmMode, uint8_t extRegsMask) {
				common::write_Opcode_16bit_Prefix(block);
				if (extRegsMask) {
					block.pushRaw<uint8_t>(0x40 | extRegsMask);
				}
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode | (rmMode << 1) | 0b1);
			}
		};

		template<class BLOCK>
		struct ExtendedOpcodeWriter<DWORD_PTR, BYTE_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode, OpMode rmMode, uint8_t extRegsMask) {
				if (extRegsMask) {
					block.pushRaw<uint8_t>(0x40 | extRegsMask);
				}
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode | (rmMode << 1));
			}
		};

		template<class BLOCK>
		struct ExtendedOpcodeWriter<DWORD_PTR, WORD_PTR, BLOCK> {
			static void write(BLOCK& block, common::Opcode opcode, OpMode rmMode, uint8_t extRegsMask) {
				if (extRegsMask) {
					block.pushRaw<uint8_t>(0x40 | extRegsMask);
				}
				common::write_Opcode_Extended_Prefix(block);
				common::write_Opcode(block, opcode | (rmMode << 1) | 0b1);
			}
		};

		template<MemSize DST_SIZE, MemSize SRC_SIZE, class BLOCK>
		static void write_Opcode_Extended(BLOCK& block, common::Opcode opcode, OpMode rmMode, uint8_t extRegsMask) {
			ExtendedOpcodeWriter<DST_SIZE, SRC_SIZE, BLOCK>::write(block, opcode, rmMode, extRegsMask);
		}

		template<MemSize SIZE, class T, class BLOCK>
		static void write_Imm_Size_Extend(BLOCK& block, const Imm<T>& imm) {
			block.pushRaw(static_cast<detail::ImmSizeExtend<SIZE, T>::type>(imm));
		}

		template<class DST_REG, class SRC_REG, class BLOCK>
		static void template_reg_reg_size_diff(BLOCK& block, common::Opcode opcode, DST_REG dst, SRC_REG src) {
			static_assert(IsRegType<DST_REG>::value, "Param must be register");
			static_assert(IsRegType<SRC_REG>::value, "Param must be register");
			write_Opcode<TypeMemSize<DST_REG>::value>(block, opcode, MODE_RR,
				detail::getExtRegMask(dst, detail::ExtSizePrefix::FIRST_REG_BIT_OFFSET) |
				detail::getExtRegMask(src, detail::ExtSizePrefix::SECOND_REG_BIT_OFFSET));
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR,
				detail::removeExtRegBit(src), detail::removeExtRegBit(dst));
		}

		template<class REG, class BLOCK>
		static void template_reg_reg(BLOCK& block, common::Opcode opcode, REG dst, REG src) {
			template_reg_reg_size_diff(block, opcode, dst, src);
		}

		/* 8/16/32 */
		template<class DST_REG, class SRC_REG, class BLOCK>
		static void template_reg_reg_ext(BLOCK& block, common::Opcode opcode, DST_REG dst, SRC_REG src) {
			static_assert(IsRegType<DST_REG>::value, "Param must be register");
			static_assert(IsRegType<SRC_REG>::value, "Param must be register");
			write_Opcode_Extended<TypeMemSize<DST_REG>::value, TypeMemSize<SRC_REG>::value>(block, opcode, MODE_RR,
				detail::getExtRegMask(dst, detail::ExtSizePrefix::FIRST_REG_BIT_OFFSET) |
				detail::getExtRegMask(src, detail::ExtSizePrefix::SECOND_REG_BIT_OFFSET));
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR,
				detail::removeExtRegBit(src), detail::removeExtRegBit(dst));
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void template_mem_reg(BLOCK& block, common::Opcode opcode, const Mem64<MODE>& dst, REG src) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			write_Opcode<TypeMemSize<REG>::value>(block, opcode, MODE_RM, dst.getExtRegMask() |
				detail::getExtRegMask(src, detail::ExtSizePrefix::SECOND_REG_BIT_OFFSET));
			dst.write(block, detail::removeExtRegBit(src));
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void template_reg_mem(BLOCK& block, common::Opcode opcode, REG dst, const Mem64<MODE>& src) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			write_Opcode<TypeMemSize<REG>::value>(block, opcode, MODE_MR, src.getExtRegMask() |
				detail::getExtRegMask(dst, detail::ExtSizePrefix::SECOND_REG_BIT_OFFSET));
			src.write(block, detail::removeExtRegBit(dst));
		}

		template<MemSize SIZE, AddressMode MODE, class REG, class BLOCK>
		static void template_reg_mem_ext(BLOCK& block, common::Opcode opcode, REG dst, const Mem64<MODE>& src) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			write_Opcode_Extended<TypeMemSize<REG>::value, SIZE>(block, opcode, MODE_MR, src.getExtRegMask() | 
				detail::getExtRegMask(dst, detail::ExtSizePrefix::SECOND_REG_BIT_OFFSET));
			src.write(block, detail::removeExtRegBit(dst));
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void template_mem_reg(BLOCK& block, common::Opcode opcode, const Mem32<MODE>& dst, REG src) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			dst.writePrefix(block);
			write_Opcode<TypeMemSize<REG>::value>(block, opcode, MODE_RM, dst.getExtRegMask() |
				detail::getExtRegMask(src, detail::ExtSizePrefix::SECOND_REG_BIT_OFFSET));
			dst.write(block, detail::removeExtRegBit(src));
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void template_reg_mem(BLOCK& block, common::Opcode opcode, REG dst, const Mem32<MODE>& src) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			src.writePrefix(block);
			write_Opcode<TypeMemSize<REG>::value>(block, opcode, MODE_MR, src.getExtRegMask() |
				detail::getExtRegMask(dst, detail::ExtSizePrefix::SECOND_REG_BIT_OFFSET));
			src.write(block, detail::removeExtRegBit(dst));
		}

		template<MemSize SIZE, AddressMode MODE, class REG, class BLOCK>
		static void template_reg_mem_ext(BLOCK& block, common::Opcode opcode, REG dst, const Mem32<MODE>& src) {
			static_assert(IsRegType<REG>::value, "Param must be register");
			src.writePrefix(block);
			write_Opcode_Extended<TypeMemSize<REG>::value, SIZE>(block, opcode, MODE_MR, src.getExtRegMask() |
				detail::getExtRegMask(dst, detail::ExtSizePrefix::SECOND_REG_BIT_OFFSET));
			src.write(block, detail::removeExtRegBit(dst));
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void template_mem_imm(BLOCK& block, common::Opcode opcode, common::Opcode opcodeExt, const Mem64<MODE>& dst, const Imm<T>& src) {
			write_Opcode<SIZE>(block, opcode, MODE_RR, dst.getExtRegMask());
			dst.write(block, opcodeExt);
			write_Imm_Size_Extend<SIZE>(block, src);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void template_mem_imm(BLOCK& block, common::Opcode opcode, common::Opcode opcodeExt, const Mem32<MODE>& dst, const Imm<T>& src) {
			dst.writePrefix(block);
			write_Opcode<SIZE>(block, opcode, MODE_RR, dst.getExtRegMask());
			dst.write(block, opcodeExt);
			write_Imm_Size_Extend<SIZE>(block, src);
		}

		template<class BLOCK>
		static void template_reg64_extend_val(BLOCK& block, common::Opcode opcode, const Reg64& dst, const Reg32& src) {
			template_reg_reg_size_diff(block, opcode, dst, src);
		}

		template<AddressMode MODE, class BLOCK>
		static void template_reg64_extend_val(BLOCK& block, common::Opcode opcode, const Reg64& dst, const Mem64<MODE>& src) {
			template_reg_mem(block, opcode, dst, src);
		}

		template<AddressMode MODE, class BLOCK>
		static void template_reg64_extend_val(BLOCK& block, common::Opcode opcode, const Reg64& dst, const Mem32<MODE>& src) {
			template_reg_mem(block, opcode, dst, src);
		}

		template<class BLOCK>
		static void template_reg_reg_extend_val(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Reg32& src) {
			template_reg_reg_ext(block, opcode, dst, src);
		}

		template<class BLOCK>
		static void template_reg_reg_extend_val(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Reg16& src) {
			template_reg_reg_ext(block, opcode, dst, src);
		}

		template<class BLOCK>
		static void template_reg_reg_extend_val(BLOCK& block, common::Opcode opcode, const Reg32& dst, const Reg8& src) {
			template_reg_reg_ext(block, opcode, dst, src);
		}

		template<class BLOCK>
		static void template_reg_reg_extend_val(BLOCK& block, common::Opcode opcode, const Reg16& dst, const Reg16& src) {
			template_reg_reg_ext(block, opcode, dst, src);
		}

		template<class BLOCK>
		static void template_reg_reg_extend_val(BLOCK& block, common::Opcode opcode, const Reg16& dst, const Reg8& src) {
			template_reg_reg_ext(block, opcode, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class REG, class BLOCK>
		static void template_reg_mem_extend_val(BLOCK& block, common::Opcode opcode, REG dst, const Mem64<MODE>& src) {
			template_reg_mem_ext<SIZE>(block, opcode, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class REG, class BLOCK>
		static void template_reg_mem_extend_val(BLOCK& block, common::Opcode opcode, REG dst, const Mem32<MODE>& src) {
			template_reg_mem_ext<SIZE>(block, opcode, dst, src);
		}
	public:
#pragma region Data transfer [IN PROGRESS]
		/* Convert byte to word */
		template<class BLOCK>
		static void Cbw(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x98);
		}

		/* Convert byte to word */
		template<class BLOCK>
		static void Cdq(BLOCK& block) {
			common::write_Opcode(block, 0x99);
		}

		/* Convert word to double word */
		template<class BLOCK>
		static void Cwd(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x99);
		}

		/* Convert word to double word */
		template<class BLOCK>
		static void Cwde(BLOCK& block) {
			common::write_Opcode(block, 0x98);
		}

		/* Calculate effective address
		 - LEA reg, [mem64]
		*/
		template<AddressMode MODE, class REG, class BLOCK>
		static void Lea(BLOCK& block, REG reg, const Mem64<MODE>& mem) {
			template_mem_reg(block, 0x8D, mem, reg);
		}

		/* Calculate effective address
		 - LEA reg, [mem32]
		*/
		template<AddressMode MODE, class REG, class BLOCK>
		static void Lea(BLOCK& block, REG reg, const Mem32<MODE>& mem) {
			template_mem_reg(block, 0x8D, mem, reg);
		}

		/* Moving data
		 - MOV reg,reg
		*/
		template<class REG, class BLOCK>
		static void Mov(BLOCK& block, REG dst, REG src) {
			template_reg_reg(block, 0x88, dst, src);
		}

		/* Moving data
		 - MOV [mem64],reg
		*/
		template<AddressMode MODE, class REG, class BLOCK>
		static void Mov(BLOCK& block, const Mem64<MODE>& dst, REG src) {
			template_mem_reg(block, 0x88, dst, src);
		}

		/* Moving data
		 - MOV reg, [mem64]
		*/
		template<AddressMode MODE, class REG, class BLOCK>
		static void Mov(BLOCK& block, REG dst, const Mem64<MODE>& src) {
			template_reg_mem(block, 0x88, dst, src);
		}

		/* Moving data
		 - MOV [mem32],reg
		*/
		template<AddressMode MODE, class REG, class BLOCK>
		static void Mov(BLOCK& block, const Mem32<MODE>& dst, REG src) {
			template_mem_reg(block, 0x88, dst, src);
		}

		/* Moving data
		 - MOV reg,[mem32]
		*/
		template<AddressMode MODE, class REG, class BLOCK>
		static void Mov(BLOCK& block, REG dst, const Mem32<MODE>& src) {
			template_reg_mem(block, 0x88, dst, src);
		}

		/* Moving data
		 - MOV [mem64],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void Mov(BLOCK& block, const Mem64<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0xC6, 0b000, dst, src);
		}

		/* Moving data
		 - MOV [mem32],imm
		*/
		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void Mov(BLOCK& block, const Mem32<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0xC6, 0b000, dst, src);
		}

		/* Mov with sign extend
		 - MOVSX reg16/32,reg
		*/
		template<class DEST_REG, class SRC_REG, class BLOCK>
		static void Movsx(BLOCK& block, DEST_REG dst, SRC_REG src) {
			template_reg_reg_extend_val(block, 0xBE, dst, src);
		}

		/* Mov with sign extend
		 - MOVSX reg16/32,[mem64]
		*/
		template<MemSize SIZE, AddressMode MODE, class REG, class BLOCK>
		static void Movsx(BLOCK& block, REG dst, const Mem64<MODE>& src) {
			template_reg_mem_extend_val<SIZE>(block, 0xBE, dst, src);
		}

		/* Mov with sign extend
		 - MOVSX reg16/32,[mem32]
		*/
		template<MemSize SIZE, AddressMode MODE, class REG, class BLOCK>
		static void Movsx(BLOCK& block, REG dst, const Mem32<MODE>& src) {
			template_reg_mem_extend_val<SIZE>(block, 0xBE, dst, src);
		}

		/* Mov with sign extend
		 - MOVSXD reg64,reg32
		 - MOVSXD reg64,[mem64]
		 - MOVSXD reg64,[mem32]
		*/
		template<class T, class BLOCK>
		static void Movsxd(BLOCK& block, Reg64 dst, const T& src) {
			template_reg64_extend_val(block, 0x63, dst, src);
		}

		/* Mov with sign extend
		 - MOVZX reg16/32,reg
		*/
		template<class DEST_REG, class SRC_REG, class BLOCK>
		static void Movzx(BLOCK& block, DEST_REG dst, SRC_REG src) {
			template_reg_reg_extend_val(block, 0xB6, dst, src);
		}

		/* Mov with sign extend
		 - MOVZX reg16/32,[mem64]
		*/
		template<MemSize SIZE, AddressMode MODE, class REG, class BLOCK>
		static void Movzx(BLOCK& block, REG dst, const Mem64<MODE>& src) {
			template_reg_mem_extend_val<SIZE>(block, 0xB6, dst, src);
		}

		/* Mov with sign extend
		 - MOVZX reg16/32,[mem32]
		*/
		template<MemSize SIZE, AddressMode MODE, class REG, class BLOCK>
		static void Movzx(BLOCK& block, REG dst, const Mem32<MODE>& src) {
			template_reg_mem_extend_val<SIZE>(block, 0xB6, dst, src);
		}


		/* Mov with zero extend
		 - MOVZX reg,reg
		 - MOVZX reg,[mem]
		*/



		/*template<class DST, class SRC, class BLOCK>
		static auto Movzx(BLOCK& block, const DST& dst, const SRC& src) {
			return template_2operands_extend_val(block, 0xB6, dst, src);
		}*/


		template<class REG, class BLOCK>
		static void Add(BLOCK& block, REG dst, REG src) {
			template_reg_reg(block, 0x00, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Add(BLOCK& block, const Mem64<MODE>& dst, REG src) {
			template_mem_reg(block, 0x00, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Add(BLOCK& block, REG dst, const Mem64<MODE>& src) {
			template_reg_mem(block, 0x00, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Add(BLOCK& block, const Mem32<MODE>& dst, REG src) {
			template_mem_reg(block, 0x00, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Add(BLOCK& block, REG dst, const Mem32<MODE>& src) {
			template_reg_mem(block, 0x00, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void Add(BLOCK& block, const Mem64<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0x80, 0b000, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void Add(BLOCK& block, const Mem32<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0x80, 0b000, dst, src);
		}


		template<class REG, class BLOCK>
		static void Adc(BLOCK& block, REG dst, REG src) {
			template_reg_reg(block, 0x10, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Adc(BLOCK& block, const Mem64<MODE>& dst, REG src) {
			template_mem_reg(block, 0x10, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Adc(BLOCK& block, REG dst, const Mem64<MODE>& src) {
			template_reg_mem(block, 0x10, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Adc(BLOCK& block, const Mem32<MODE>& dst, REG src) {
			template_mem_reg(block, 0x10, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Adc(BLOCK& block, REG dst, const Mem32<MODE>& src) {
			template_reg_mem(block, 0x10, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void Adc(BLOCK& block, const Mem64<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0x80, 0b010, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void Adc(BLOCK& block, const Mem32<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0x80, 0b010, dst, src);
		}


		template<class REG, class BLOCK>
		static void Sbb(BLOCK& block, REG dst, REG src) {
			template_reg_reg(block, 0x18, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Sbb(BLOCK& block, const Mem64<MODE>& dst, REG src) {
			template_mem_reg(block, 0x18, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Sbb(BLOCK& block, REG dst, const Mem64<MODE>& src) {
			template_reg_mem(block, 0x18, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Sbb(BLOCK& block, const Mem32<MODE>& dst, REG src) {
			template_mem_reg(block, 0x18, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Sbb(BLOCK& block, REG dst, const Mem32<MODE>& src) {
			template_reg_mem(block, 0x18, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void Sbb(BLOCK& block, const Mem64<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0x80, 0b011, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void Sbb(BLOCK& block, const Mem32<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0x80, 0b011, dst, src);
		}


		template<class REG, class BLOCK>
		static void And(BLOCK& block, REG dst, REG src) {
			template_reg_reg(block, 0x20, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void And(BLOCK& block, const Mem64<MODE>& dst, REG src) {
			template_mem_reg(block, 0x20, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void And(BLOCK& block, REG dst, const Mem64<MODE>& src) {
			template_reg_mem(block, 0x20, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void And(BLOCK& block, const Mem32<MODE>& dst, REG src) {
			template_mem_reg(block, 0x20, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void And(BLOCK& block, REG dst, const Mem32<MODE>& src) {
			template_reg_mem(block, 0x20, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void And(BLOCK& block, const Mem64<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0x80, 0b100, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void And(BLOCK& block, const Mem32<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0x80, 0b100, dst, src);
		}


		template<class REG, class BLOCK>
		static void Sub(BLOCK& block, REG dst, REG src) {
			template_reg_reg(block, 0x28, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Sub(BLOCK& block, const Mem64<MODE>& dst, REG src) {
			template_mem_reg(block, 0x28, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Sub(BLOCK& block, REG dst, const Mem64<MODE>& src) {
			template_reg_mem(block, 0x28, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Sub(BLOCK& block, const Mem32<MODE>& dst, REG src) {
			template_mem_reg(block, 0x28, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Sub(BLOCK& block, REG dst, const Mem32<MODE>& src) {
			template_reg_mem(block, 0x28, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void Sub(BLOCK& block, const Mem64<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0x80, 0b101, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void Sub(BLOCK& block, const Mem32<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0x80, 0b101, dst, src);
		}


		template<class REG, class BLOCK>
		static void Xor(BLOCK& block, REG dst, REG src) {
			template_reg_reg(block, 0x30, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Xor(BLOCK& block, const Mem64<MODE>& dst, REG src) {
			template_mem_reg(block, 0x30, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Xor(BLOCK& block, REG dst, const Mem64<MODE>& src) {
			template_reg_mem(block, 0x30, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Xor(BLOCK& block, const Mem32<MODE>& dst, REG src) {
			template_mem_reg(block, 0x30, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Xor(BLOCK& block, REG dst, const Mem32<MODE>& src) {
			template_reg_mem(block, 0x30, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void Xor(BLOCK& block, const Mem64<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0x80, 0b110, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void Xor(BLOCK& block, const Mem32<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0x80, 0b110, dst, src);
		}


		template<class REG, class BLOCK>
		static void Cmp(BLOCK& block, REG dst, REG src) {
			template_reg_reg(block, 0x38, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Cmp(BLOCK& block, const Mem64<MODE>& dst, REG src) {
			template_mem_reg(block, 0x38, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Cmp(BLOCK& block, REG dst, const Mem64<MODE>& src) {
			template_reg_mem(block, 0x38, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Cmp(BLOCK& block, const Mem32<MODE>& dst, REG src) {
			template_mem_reg(block, 0x38, dst, src);
		}

		template<AddressMode MODE, class REG, class BLOCK>
		static void Cmp(BLOCK& block, REG dst, const Mem32<MODE>& src) {
			template_reg_mem(block, 0x38, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void Cmp(BLOCK& block, const Mem64<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0x80, 0b111, dst, src);
		}

		template<MemSize SIZE, AddressMode MODE, class T, class BLOCK>
		static void Cmp(BLOCK& block, const Mem32<MODE>& dst, const Imm<T>& src) {
			template_mem_imm<SIZE>(block, 0x80, 0b111, dst, src);
		}


		

		template<class BLOCK>
		static void Mov(BLOCK& block, Reg64 dst, S32 src) {
			write_Opcode<TypeMemSize<Reg64>::value>(block, 0xC7, MODE_RR,
				detail::getExtRegMask(dst, detail::ExtSizePrefix::FIRST_REG_BIT_OFFSET));
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, 0, detail::removeExtRegBit(dst));
			common::write_Immediate(block, src);
		}

		
	};
}