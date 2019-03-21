#pragma once

#include "x86_i386.h"

namespace CppAsm::X86
{
	class CLFLUSHOPT : private i386 {
	public:
		/* Flush Cache Line Optimized containing mem */
		template<AddressMode MODE, class BLOCK>
		static void Clflushopt(BLOCK& block, const Mem32<MODE>& mem) {
			mem.writeSegmPrefix(block);
			common::write_Opcode_16bit_Prefix(block);
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0xAE);
			mem.write(block, 0b111);
		}
	};
}