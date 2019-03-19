#pragma once

#include "x86_i486.h"

namespace CppAsm::X86
{
	class i586 : public i486 {
	public:
		static void Cpuid(Os::CodeBlock& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0xA2);
		}

		template<MemSize SIZE = QWORD_PTR, AddressMode MODE>
		static void CmpXchg8b(Os::CodeBlock& block, const Mem32<MODE>& mem) {
			static_assert(SIZE == QWORD_PTR, "Invalid operand size");
			mem.writeSegmPrefix(block);
			write_Operand_Extended_Prefix(block);
			write_Opcode(block, 0xC7);
			mem.write(block, 0b001);
		}

		static void Wrmsr(Os::CodeBlock& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x30);
		}

		static void Rdtsc(Os::CodeBlock& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x31);
		}

		static void Rdmsr(Os::CodeBlock& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x32);
		}

		static void Rsm(Os::CodeBlock& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0xAA);
		}
	};
}