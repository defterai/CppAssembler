#pragma once

#include "x86_i386.h"

namespace CppAsm::X86
{
	class SEP : private i386 {
	public:
		/* Fast call to privilege level 0 system procedures. */
		template<class BLOCK>
		static void Sysenter(BLOCK& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x34);
		}

		/* Fast return to privilege level 3 user code. */
		template<class BLOCK>
		static void Sysexit(BLOCK& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x35);
		}
	};
}