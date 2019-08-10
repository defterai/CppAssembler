#pragma once

#include "x86_i586.h"
#include "x86_PMC.h"
#include "x86_CMOV.h"
#include "x86_i387.h"

#pragma warning(push)
#pragma warning(disable : 4584)

namespace CppAsm::X86
{
	class i686 : public i586, /*public i387,*/ public CMOV {
	public:
		template<class BLOCK>
		static constexpr void Fcomi(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDB, 0b110), reg);
		}

		template<class BLOCK>
		static constexpr void Fcomip(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDF, 0b110), reg);
		}

		template<class BLOCK>
		static constexpr void Fucomi(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDB, 0b101), reg);
		}

		template<class BLOCK>
		static constexpr void Fucomip(BLOCK& block, TopRegFPU, RegFPU reg) {
			template_1operand_float(block, detail::OpcodeLarge(0xDF, 0b101), reg);
		}

		template<class BLOCK>
		static constexpr void Ud2(BLOCK& block) {
			common::write_Opcode_Extended_Prefix(block);
			common::write_Opcode(block, 0x0B);
		}
	};
}

#pragma warning(pop) 
