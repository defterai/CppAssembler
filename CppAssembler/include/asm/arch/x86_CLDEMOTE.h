#pragma once

#include "x86_i386.h"

namespace CppAsm::X86
{
	class CLDEMOTE : private i386 {
	public:
		/* Cache Line Demote - Hint to hardware to move the cache line containing mem 
		 * to a more distant level of the cache without writing back to memory. */
		template<AddressMode MODE>
		static void Cldemote(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x1C);
			mem.write(block, 0b000);
		}
	};
}