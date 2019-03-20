#pragma once

#include "x86_i586.h"
#include "x86_PMC.h"
#include "x86_i387.h"

namespace CppAsm::X86
{
	class i686 : public i586, public PMC, public i387 {
	public:
		static void Fcomi(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDB, 0b110), reg);
		}

		static void Fcomip(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDF, 0b110), reg);
		}

		static void Fucomi(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDB, 0b101), reg);
		}

		static void Fucomip(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDF, 0b101), reg);
		}

		static void Ud2(Os::CodeBlock& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x0B);
		}
	};
}