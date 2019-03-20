#pragma once

#include "x86_i486.h"

namespace CppAsm::X86
{
	class TSC : public i386 {
	public:
		static void Rdtsc(Os::CodeBlock& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x31);
		}
	};
}