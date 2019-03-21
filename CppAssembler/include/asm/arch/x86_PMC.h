#pragma once

#include "x86_i386.h"

namespace CppAsm::X86
{
	class PMC : private i386 {
	public:
		template<class BLOCK>
		static void Rdpmc(BLOCK& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x33);
		}
	};
}