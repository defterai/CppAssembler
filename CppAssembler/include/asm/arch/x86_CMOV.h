#pragma once


#include "x86_i686.h"
#include "x86_i387.h"

namespace CppAsm::X86
{
	class CMOV : private i586, private i387 {
	protected:
		static void template_FCMOVcc(Os::CodeBlock& block, const detail::OpcodeLarge& opcode, RegFPU reg) {
			common::write_Opcode(block, opcode.getOpcode());
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
		}
	public:
		static void Fcmovb(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDA, 0b000), reg);
		}

		static void Fcmove(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDA, 0b001), reg);
		}

		static void Fcmovbe(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDA, 0b010), reg);
		}

		static void Fcmovu(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDA, 0b011), reg);
		}

		static void Fcmovnb(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDB, 0b000), reg);
		}

		static void Fcmovne(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDB, 0b001), reg);
		}

		static void Fcmovnbe(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDB, 0b010), reg);
		}

		static void Fcmovnu(Os::CodeBlock& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDB, 0b011), reg);
		}
	};
}