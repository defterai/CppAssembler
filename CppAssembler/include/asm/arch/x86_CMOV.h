#pragma once


#include "x86_i686.h"
#include "x86_i387.h"

namespace CppAsm::X86
{
	class CMOV : private i386, private i387 {
	protected:
		template<class BLOCK>
		static void template_FCMOVcc(BLOCK& block, const detail::OpcodeLarge& opcode, RegFPU reg) {
			common::write_Opcode(block, opcode.getOpcode());
			common::write_MOD_REG_RM(block, common::MOD_REG_RM::REG_ADDR, opcode.getMode(), reg);
		}
	public:
		template<class BLOCK>
		static void Fcmovb(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDA, 0b000), reg);
		}

		template<class BLOCK>
		static void Fcmove(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDA, 0b001), reg);
		}

		template<class BLOCK>
		static void Fcmovbe(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDA, 0b010), reg);
		}

		template<class BLOCK>
		static void Fcmovu(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDA, 0b011), reg);
		}

		template<class BLOCK>
		static void Fcmovnb(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDB, 0b000), reg);
		}

		template<class BLOCK>
		static void Fcmovne(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDB, 0b001), reg);
		}

		template<class BLOCK>
		static void Fcmovnbe(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDB, 0b010), reg);
		}

		template<class BLOCK>
		static void Fcmovnu(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_FCMOVcc(block, detail::OpcodeLarge(0xDB, 0b011), reg);
		}
	};
}