#pragma once

#include "x86_i486.h"
#include "x86_MSR.h"
#include "x86_TSC.h"
#include "x86_CX8.h"

namespace CppAsm::X86
{
	/* Also known as Pentium */
	class i586 : public i486, public MSR, public TSC, public CX8 {
	public:
		/* Resume from System Management Mode  */
		template<class BLOCK>
		static void Rsm(BLOCK& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0xAA);
		}
	};
}