#pragma once

#include "x86_i386.h"

namespace CppAsm::X86
{
	class i486 : public i386 {
	public:
		/* Swap bytes inside register */
		static ReplaceableReg<Reg32> Bswap(Os::CodeBlock& block, Reg32 reg) {
			common::write_Opcode_Extended_Prefix(block);
			Offset offset = block.getOffset();
			common::write_Opcode(block, 0xC8 | reg);
			return ReplaceableReg<Reg32>(offset, 0);
		}

		/* Xchange and add
		 - XADD reg,reg
		 - XADD [mem],reg
		*/
		template<class DST, class SRC>
		static void Xadd(Os::CodeBlock& block, const DST& dst, const SRC& src) {
			template_2operands_xchange(block, 0xC0, dst, src);
		}

		/* Compare and xchange
		 - CMPXCHG reg,reg
		 - CMPXCHG [mem],reg
		*/
		template<class DST, class SRC>
		static void CmpXchg(Os::CodeBlock& block, const DST& dst, const SRC& src) {
			template_2operands_xchange(block, 0xB0, dst, src);
		}
	};
}