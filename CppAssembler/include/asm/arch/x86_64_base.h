#pragma once


#include "../defs.h"
#include "../replaceable.h"

namespace CppAsm::X86_64::detail
{
	typedef uint8_t Opcode;

	constexpr auto calc_Jump_Offset(Addr addr, Addr jumpAddr, uint32_t instructionSize) {
		return jumpAddr - (addr + instructionSize);
	}

	template<class T>
	constexpr bool is_Byte_Offset(T offset) {
		return (offset >= -128) && (offset <= 127);
	}

	class MOD_REG_RM {
	public:
		typedef uint8_t Type;
		enum Mod : uint8_t {
			REG_IND_ADDR = 0b00,
			SIB_NO_DISP = 0b00,
			DISP_ADDR_ONLY = 0b00,
			DISP_ADDR_1BYTE = 0b01,
			DISP_ADDR_4BYTE = 0b10,
			REG_ADDR = 0b11
		};
		constexpr static uint8_t MOD_BIT_OFFSET = 6;
		constexpr static uint8_t REG_BIT_OFFSET = 3;
		constexpr static uint8_t RM_BIT_OFFSET = 0;
	private:
		Type mValue;
	public:
		constexpr static Type generate(Mod mod, uint8_t reg, uint8_t rm) {
			return (mod << MOD_BIT_OFFSET) | (reg << REG_BIT_OFFSET) | rm;
		}
		constexpr static Type addReg(Type value, uint8_t reg) {
			return value | (reg << REG_BIT_OFFSET);
		}
	public:
		constexpr MOD_REG_RM(Mod mod, uint8_t rm) : mValue(generate(mod, 0, rm)) {}
		constexpr MOD_REG_RM(Mod mod, uint8_t reg, uint8_t rm) : mValue(generate(mod, reg, rm)) {}

		template<class BLOCK>
		void writeMOD_REG_RM(BLOCK& block) const {
			block.pushRaw(mValue);
		}

		template<class BLOCK>
		void writeMOD_REG_RM(BLOCK& block, uint8_t reg) const {
			block.pushRaw(addReg(mValue, reg));
		}
	};

	class MOD_REG_OFFSET : public MOD_REG_RM {
	public:
		explicit constexpr MOD_REG_OFFSET(Mod mod) : MOD_REG_RM(mod, 0b101) {}
		explicit constexpr MOD_REG_OFFSET(Mod mod, uint8_t reg) : MOD_REG_RM(mod, reg, 0b101) {}
	};

	class MOD_REG_INDEX : public MOD_REG_RM {
	public:
		explicit constexpr MOD_REG_INDEX(Mod mod) : MOD_REG_RM(mod, 0b100) {}
		explicit constexpr MOD_REG_INDEX(Mod mod, uint8_t reg) : MOD_REG_RM(mod, reg, 0b100) {}
	};

	class SIB {
	public:
		typedef uint8_t Type;
		constexpr static uint8_t SCALE_BIT_OFFSET = 6;
		constexpr static uint8_t INDEX_BIT_OFFSET = 3;
		constexpr static uint8_t BASE_BIT_OFFSET = 0;
	private:
		Type mValue;

		constexpr static Type buildSIB(uint8_t scale, uint8_t index, uint8_t base) {
			return (scale << 6) | (index << 3) | base;
		}
	public:
		constexpr SIB(uint8_t scale, uint8_t index, uint8_t base): mValue(buildSIB(scale, index, base)) {}

		constexpr Type getSIB() const {
			return mValue;
		}

		template<class BLOCK>
		void writeSIB(BLOCK& block) const {
			block.pushRaw(mValue);
		}
	};

	template<class BLOCK>
	static void write_Opcode(BLOCK& block, Opcode opcode) {
		block.pushRaw(opcode);
	}

	template<class BLOCK>
	static void write_Opcode_16bit_Prefix(BLOCK& block) {
		block.pushRaw<uint8_t>(0x66);
	}

	template<class BLOCK>
	static void write_Opcode_Extended_Prefix(BLOCK& block) {
		block.pushRaw<uint8_t>(0x0F);
	}

	template<class BLOCK>
	static void write_MOD_REG_RM(BLOCK& block, MOD_REG_RM::Mod mod, uint8_t reg, uint8_t rm) {
		block.pushRaw(MOD_REG_RM::generate(mod, reg, rm));
	}

	template<class IMM, class BLOCK>
	static void write_Immediate(BLOCK& block, const IMM& imm) {
		block.pushRaw<IMM::type>(imm);
	}

	template<uint8_t R, class BLOCK>
	static void write_Opcode_Rep(BLOCK& block, Opcode opcode) {
		/*constexpr*/ if (R) {
			block.pushRaw<uint8_t>(R);
		}
		write_Opcode(block, opcode);
	}

	template<uint8_t L, class BLOCK>
	static void write_Lock_Prefix(BLOCK& block) {
		/*constexpr*/ if (L) {
			block.pushRaw<uint8_t>(L);
		}
	}
}