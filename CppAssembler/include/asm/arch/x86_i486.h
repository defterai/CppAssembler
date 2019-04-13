#pragma once

#include "x86_i386.h"

namespace CppAsm::X86
{
	class i486 : public i386 {
	public:
		/* Swap bytes inside register */
		template<class BLOCK>
		static constexpr void Bswap(BLOCK& block, Reg32 reg) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0xC8 | reg);
		}

		template<class BLOCK>
		static ReplaceableReg<Reg32> Bswap_r(BLOCK& block, Reg32 reg) {
			common::write_Opcode_Extended_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0xC8 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Xchange and add
		 - XADD reg,reg
		*/
		template<class REG, class BLOCK>
		static constexpr void Xadd(BLOCK& block, const REG& dst, const REG& src) {
			static_assert(IsRegType<REG>::value, "Xadd: First and second parameter is not register");
			write_Opcode_Extended<TypeMemSize<REG>::value>(block, 0xC0);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, src, dst);
		}

		template<class REG, class BLOCK>
		static auto Xadd_r(BLOCK& block, const REG& dst, const REG& src) {
			static_assert(IsRegType<REG>::value, "Xadd: First and second parameter is not register");
			write_Opcode_Extended<TypeMemSize<REG>::value>(block, 0xC0);
			return write_MOD_REG_RM_ReplaceableRR(block, src, dst);
		}

		/* Xchange and add
		 - XADD [mem],reg
		*/
		template<LockPrefix L = NO_LOCK, AddressMode MODE, class REG, class BLOCK>
		static constexpr void Xadd(BLOCK& block, const Mem32<MODE>& mem, const REG& reg) {
			static_assert(IsRegType<REG>::value, "Xadd: Second parameter is not register");
			mem.writeSegmPrefix(block);
			write_Opcode_Extended<TypeMemSize<REG>::value, L>(block, 0xC0);
			mem.write(block, reg);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class REG, class BLOCK>
		static auto Xadd_r(BLOCK& block, const Mem32<MODE>& mem, const REG& reg) {
			static_assert(IsRegType<REG>::value, "Xadd: Second parameter is not register");
			mem.writeSegmPrefix(block);
			write_Opcode_Extended<TypeMemSize<REG>::value, L>(block, 0xC0);
			return mem.writeReplaceable(block, reg);
		}

		/* Compare and xchange
		 - CMPXCHG reg,reg
		*/
		template<class REG, class BLOCK>
		static constexpr void CmpXchg(BLOCK& block, const REG& dst, const REG& src) {
			static_assert(IsRegType<REG>::value, "CmpXchg: First and second parameter is not register");
			write_Opcode_Extended<TypeMemSize<REG>::value>(block, 0xB0);
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, src, dst);
		}

		template<class REG, class BLOCK>
		static auto CmpXchg_r(BLOCK& block, const REG& dst, const REG& src) {
			static_assert(IsRegType<REG>::value, "CmpXchg: First and second parameter is not register");
			write_Opcode_Extended<TypeMemSize<REG>::value>(block, 0xB0);
			return write_MOD_REG_RM_ReplaceableRR(block, src, dst);
		}

		/* Compare and xchange
		 - CMPXCHG [mem],reg
		*/
		template<LockPrefix L = NO_LOCK, AddressMode MODE, class REG, class BLOCK>
		static constexpr void CmpXchg(BLOCK& block, const Mem32<MODE>& mem, const REG& reg) {
			static_assert(IsRegType<REG>::value, "CmpXchg: Second parameter is not register");
			mem.writeSegmPrefix(block);
			write_Opcode_Extended<TypeMemSize<REG>::value, L>(block, 0xB0);
			mem.write(block, reg);
		}

		template<LockPrefix L = NO_LOCK, AddressMode MODE, class REG, class BLOCK>
		static auto CmpXchg_r(BLOCK& block, const Mem32<MODE>& mem, const REG& reg) {
			static_assert(IsRegType<REG>::value, "CmpXchg: Second parameter is not register");
			mem.writeSegmPrefix(block);
			write_Opcode_Extended<TypeMemSize<REG>::value, L>(block, 0xB0);
			return mem.writeReplaceable(block, reg);
		}

		/* Returns processor identification and feature information to the EAX, EBX, ECX, and EDX registers, 
		 * according to the input value entered initially in the EAX register. */
		template<class BLOCK>
		static constexpr void Cpuid(BLOCK& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0xA2);
		}

		/* Flush internal caches; initiate flushing of external caches. */
		template<class BLOCK>
		static constexpr void Invd(BLOCK& block) {
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode(block, 0x08);
		}

		/* Invalidate TLB Entry for page that contains mem */
		template<AddressMode MODE, class BLOCK>
		static constexpr void Invlpg(BLOCK& block, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x01);
			mem.write(block, 0b111);
		}

		template<AddressMode MODE, class BLOCK>
		static auto Invlpg_r(BLOCK& block, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x01);
			return mem.writeReplaceable(block, 0b111);
		}

		/* Write Back and Invalidate Cache */
		template<class BLOCK>
		static constexpr void Wbinvd(BLOCK& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x09);
		}
	};
}