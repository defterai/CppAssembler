#pragma once

#include "x86_i386.h"

namespace CppAsm::X86
{
	class CLFSH : private i386 {
	public:
		/* Flush Cache Line containing mem */
		template<AddressMode MODE, class BLOCK>
		static void Clflush(BLOCK& block, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0xAE);
			mem.write(block, 0b111);
		}
	};
}