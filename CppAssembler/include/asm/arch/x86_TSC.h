#pragma once

#include "x86_i486.h"

namespace CppAsm::X86
{
	class TSC : public i386 {
	public:
		template<class BLOCK>
		static void Rdtsc(BLOCK& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x31);
		}
	};
}