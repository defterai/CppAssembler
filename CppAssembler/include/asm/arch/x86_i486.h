#pragma once

#include "x86_i386.h"

namespace CppAsm::X86
{
	class i486 : public i386 {
	public:
		/* Swap bytes inside register */
		template<class BLOCK>
		static ReplaceableReg<Reg32> Bswap(BLOCK& block, Reg32 reg) {
			common::write_Opcode_Extended_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0xC8 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Xchange and add
		 - XADD reg,reg
		*/
		template<class REG, class BLOCK>
		static void Xadd(BLOCK& block, const REG& dst, const REG& src) {
			static_assert(IsRegType<REG>::value, "Xadd: First and second parameter is not register");
			write_Opcode_Extended<TypeMemSize<REG>::value>(block, 0xC0);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, src, dst);
		}

		/* Xchange and add
		 - XADD [mem],reg
		*/
		template<AddressMode MODE, class REG, class BLOCK>
		static void Xadd(BLOCK& block, const Mem32<MODE>& mem, const REG& reg) {
			static_assert(IsRegType<REG>::value, "Xadd: Second parameter is not register");
			mem.writeSegmPrefix(block);
			write_Opcode_Extended<TypeMemSize<REG>::value>(block, 0xC0);
			mem.write(block, reg);
		}

		/* Compare and xchange
		 - CMPXCHG reg,reg
		*/
		template<class REG, class BLOCK>
		static void CmpXchg(BLOCK& block, const REG& dst, const REG& src) {
			static_assert(IsRegType<REG>::value, "CmpXchg: First and second parameter is not register");
			write_Opcode_Extended<TypeMemSize<REG>::value>(block, 0xB0);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, src, dst);
		}

		/* Compare and xchange
		 - CMPXCHG [mem],reg
		*/
		template<AddressMode MODE, class REG, class BLOCK>
		static void CmpXchg(BLOCK& block, const Mem32<MODE>& mem, const REG& reg) {
			static_assert(IsRegType<REG>::value, "CmpXchg: Second parameter is not register");
			mem.writeSegmPrefix(block);
			write_Opcode_Extended<TypeMemSize<REG>::value>(block, 0xB0);
			mem.write(block, reg);
		}

		/* Returns processor identification and feature information to the EAX, EBX, ECX, and EDX registers, 
		 * according to the input value entered initially in the EAX register. */
		template<class BLOCK>
		static void Cpuid(BLOCK& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0xA2);
		}

		/* Flush internal caches; initiate flushing of external caches. */
		template<class BLOCK>
		static void Invd(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x08);
		}

		/* Invalidate TLB Entry for page that contains mem */
		template<AddressMode MODE, class BLOCK>
		static void Invlpg(BLOCK& block, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x01);
			mem.write(block, 0b111);
		}

		/* Write Back and Invalidate Cache */
		template<class BLOCK>
		static void Wbinvd(BLOCK& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x09);
		}
	};
}