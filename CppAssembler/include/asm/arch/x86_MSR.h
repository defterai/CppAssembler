#pragma once

#include "x86_i386.h"

namespace CppAsm::X86
{
	class MSR : private i386 {
	public:
		/* Load MSR specified by ECX into EDX:EAX. */
		template<class BLOCK>
		static void Rdmsr(BLOCK& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x32);
		}

		/* Write the value in EDX:EAX to MSR specified by ECX. */
		template<class BLOCK>
		static void Wrmsr(BLOCK& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x30);
		}
	};
}