#pragma once

#include "x86_i386.h"

namespace CppAsm::X86
{
	class SEP : private i386 {
	public:
		/* Fast call to privilege level 0 system procedures. */
		static void Sysenter(Os::CodeBlock& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x34);
		}

		/* Fast return to privilege level 3 user code. */
		static void Sysexit(Os::CodeBlock& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x35);
		}
	};
}