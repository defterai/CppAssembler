#pragma once

#include "x86_i386.h"

namespace CppAsm::X86
{
	class CX8 : private i386 {
	public:
		/* Compare and Exchange 8 Bytes */
		template<LockPrefix L = NO_LOCK, MemSize SIZE = QWORD_PTR, AddressMode MODE, class BLOCK>
		static void CmpXchg8b(BLOCK& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == QWORD_PTR, "Invalid operand size");
			mem.writeSegmPrefix(block);
			common::write_Lock_Prefix<L>(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0xC7);
			mem.write(block, 0b001);
		}
	};
}