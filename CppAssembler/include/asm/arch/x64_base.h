#pragma once


#include "x86_64_base.h"

namespace CppAsm::X64
{
	namespace
	{
		namespace common = CppAsm::X86_64::detail;
	}

	enum Reg8 : uint8_t {
		AL = 0b000,
		CL = 0b001,
		DL = 0b010,
		BL = 0b011,
		AH = 0b100,
		CH = 0b101,
		DH = 0b110,
		BH = 0b111
	};

	enum Reg16 : uint8_t {
		AX = 0b000,
		CX = 0b001,
		DX = 0b010,
		BX = 0b011,
		SP = 0b100,
		BP = 0b101,
		SI = 0b110,
		DI = 0b111
	};

	enum Reg32 : uint8_t {
		EAX = 0b0000,
		ECX = 0b0001,
		EDX = 0b0010,
		EBX = 0b0011,
		ESP = 0b0100,
		EBP = 0b0101,
		ESI = 0b0110,
		EDI = 0b0111,
		R8d = 0b1000,
		R9d = 0b1001,
		R10d= 0b1010,
		R11d= 0b1011,
		R12d= 0b1100,
		R13d= 0b1101,
		R14d= 0b1110,
		R15d= 0b1111
	};

	enum Reg64 : uint8_t {
		RAX = 0b0000,
		RCX = 0b0001,
		RDX = 0b0010,
		RBX = 0b0011,
		RSP = 0b0100,
		RBP = 0b0101,
		RSI = 0b0110,
		RDI = 0b0111,
		R8 =  0b1000,
		R9 =  0b1001,
		R10 = 0b1010,
		R11 = 0b1011,
		R12 = 0b1100,
		R13 = 0b1101,
		R14 = 0b1110,
		R15 = 0b1111
	};

	enum RegSeg : uint8_t {
		ES = 0x26,
		CS = 0x2E,
		SS = 0x36,
		DS = 0x3E,
		FS = 0x64,
		GS = 0x65
	};

	enum IndexScale : uint8_t {
		SCALE_1 = 0b00,
		SCALE_2 = 0b01,
		SCALE_4 = 0b10,
		SCALE_8 = 0b11,
	};

	enum MemSize {
		BYTE_PTR,
		WORD_PTR,
		DWORD_PTR,
		QWORD_PTR,
		TBYTE_PTR
	};

	enum AddressMode {
		OFFSET,			// segment:[offset]
		BASE,			// segment:[base_reg]
		BASE_OFFSET,	// segment:[base_reg+offset]
		INDEX_OFFSET,	// segment:[index_reg*multiplier+offset]
		BASE_INDEX,		// segment:[base_reg+index_reg*index]
		BASE_INDEX_OFFSET // segment:[register+index_reg*multiplier+offset]
	};

	template<class T>
	struct TypeMemSize;

	template<>
	struct TypeMemSize<Reg8> {
		constexpr static MemSize value = MemSize::BYTE_PTR;
	};

	template<>
	struct TypeMemSize<Reg16> {
		constexpr static MemSize value = MemSize::WORD_PTR;
	};

	template<>
	struct TypeMemSize<Reg32> {
		constexpr static MemSize value = MemSize::DWORD_PTR;
	};

	template<>
	struct TypeMemSize<Reg64> {
		constexpr static MemSize value = MemSize::QWORD_PTR;
	};

	template<>
	struct TypeMemSize<S8> {
		constexpr static MemSize value = MemSize::BYTE_PTR;
	};

	template<>
	struct TypeMemSize<S16> {
		constexpr static MemSize value = MemSize::WORD_PTR;
	};

	template<>
	struct TypeMemSize<S32> {
		constexpr static MemSize value = MemSize::DWORD_PTR;
	};

	template<>
	struct TypeMemSize<U8> {
		constexpr static MemSize value = MemSize::BYTE_PTR;
	};

	template<>
	struct TypeMemSize<U16> {
		constexpr static MemSize value = MemSize::WORD_PTR;
	};

	template<>
	struct TypeMemSize<U32> {
		constexpr static MemSize value = MemSize::DWORD_PTR;
	};

	template<class T>
	struct IsRegType {
		constexpr static bool value = false;
	};

	template<>
	struct IsRegType<Reg64> {
		constexpr static bool value = true;
	};

	template<>
	struct IsRegType<Reg32> {
		constexpr static bool value = true;
	};

	template<>
	struct IsRegType<Reg16> {
		constexpr static bool value = true;
	};

	template<>
	struct IsRegType<Reg8> {
		constexpr static bool value = true;
	};

	namespace detail
	{
		constexpr bool isExtReg(Reg32 reg) {
			return (reg & 0b111) != 0;
		}

		constexpr bool isExtReg(Reg64 reg) {
			return (reg & 0b111) != 0;
		}

		constexpr Reg8 removeExtRegBit(Reg8 reg) {
			return reg;
		}

		constexpr Reg16 removeExtRegBit(Reg16 reg) {
			return reg;
		}

		constexpr Reg32 removeExtRegBit(Reg32 reg) {
			return Reg32(reg & 0b111);
		}

		constexpr Reg64 removeExtRegBit(Reg64 reg) {
			return Reg64(reg & 0b111);
		}

		template<class REG>
		constexpr uint8_t getExtRegMask(REG reg, uint8_t bitNumber) {
			return 0;
		}

		template<>
		constexpr uint8_t getExtRegMask(Reg32 reg, uint8_t bitNumber) {
			return (reg >> 3) << bitNumber;
		}

		template<>
		constexpr uint8_t getExtRegMask(Reg64 reg, uint8_t bitNumber) {
			return (reg >> 3) << bitNumber;
		}

		class Mem64_Seg {
		private:
			RegSeg mSegReg;
			bool mCustomSegReg;

			constexpr static RegSeg segRegFromBase(Reg64 baseReg) {
				return ((baseReg == RBP) || (baseReg == RSP)) ? SS : DS;
			}
			constexpr static bool isCustomSegReg(RegSeg segReg) {
				return segReg != DS;
			}
			constexpr static bool isCustomSegReg(Reg64 baseReg, RegSeg segReg) {
				return segRegFromBase(baseReg) != segReg;
			}
		public:
			constexpr Mem64_Seg() : mSegReg(DS), mCustomSegReg(false) {}
			explicit constexpr Mem64_Seg(RegSeg segReg) : mSegReg(segReg), mCustomSegReg(isCustomSegReg(segReg)) {}
			constexpr Mem64_Seg(RegSeg segReg, Reg64 baseReg) : mSegReg(segReg), mCustomSegReg(isCustomSegReg(baseReg, segReg)) {}

			constexpr RegSeg getSegReg() const {
				return mSegReg;
			}
			constexpr bool hasCustomSegReg() const {
				return mCustomSegReg;
			}
			void writeSegmPrefix(Os::CodeBlock& block) const {
				block.writeRaw(mSegReg);
				block.skipBytes(mCustomSegReg ? sizeof(mSegReg) : 0);
			}
		};

		class Mem32_Seg {
		private:
			RegSeg mSegReg;
			bool mCustomSegReg;

			constexpr static RegSeg segRegFromBase(Reg32 baseReg) {
				return ((baseReg == EBP) || (baseReg == ESP)) ? SS : DS;
			}
			constexpr static bool isCustomSegReg(RegSeg segReg) {
				return segReg != DS;
			}
			constexpr static bool isCustomSegReg(Reg32 baseReg, RegSeg segReg) {
				return segRegFromBase(baseReg) != segReg;
			}
		public:
			constexpr Mem32_Seg() : mSegReg(DS), mCustomSegReg(false) {}
			explicit constexpr Mem32_Seg(RegSeg segReg) : mSegReg(segReg), mCustomSegReg(isCustomSegReg(segReg)) {}
			constexpr Mem32_Seg(RegSeg segReg, Reg32 baseReg) : mSegReg(segReg), mCustomSegReg(isCustomSegReg(baseReg, segReg)) {}

			constexpr RegSeg getSegReg() const {
				return mSegReg;
			}
			constexpr bool hasCustomSegReg() const {
				return mCustomSegReg;
			}
			void writeSegmPrefix(Os::CodeBlock& block) const {
				block.writeRaw(mSegReg);
				block.skipBytes(mCustomSegReg ? sizeof(mSegReg) : 0);
			}
		};

		class Mem64_Offset {
		private:
			int32_t mOffset;
			uint8_t mOptimalBytes;
			bool mByteOffset;
			common::MOD_REG_RM::Mod mDispMod;

			constexpr static uint8_t calcOptimalBytes(int32_t offset) {
				return common::is_Byte_Offset(offset) ? sizeof(uint8_t) : sizeof(mOffset);
			}
			constexpr static common::MOD_REG_RM::Mod calcDispMod(int32_t offset) {
				return common::is_Byte_Offset(offset) ? common::MOD_REG_RM::DISP_ADDR_1BYTE : common::MOD_REG_RM::DISP_ADDR_4BYTE;
			}
		public:
			constexpr Mem64_Offset() : mOffset(0), mOptimalBytes(sizeof(uint8_t)), mByteOffset(true), mDispMod(common::MOD_REG_RM::DISP_ADDR_1BYTE) {}
			explicit constexpr Mem64_Offset(int8_t offset) : mOffset(offset), mOptimalBytes(sizeof(uint8_t)), mByteOffset(true), mDispMod(common::MOD_REG_RM::DISP_ADDR_1BYTE) {}
			explicit constexpr Mem64_Offset(int32_t offset) : mOffset(offset), mOptimalBytes(calcOptimalBytes(offset)), mByteOffset(mOptimalBytes == 1), mDispMod(calcDispMod(offset)) {}

			constexpr bool isByteOffset() const {
				return mByteOffset;
			}
			constexpr common::MOD_REG_RM::Mod getDispMod() const {
				return mDispMod;
			}
			constexpr int8_t getByteOffset() const {
				return mOffset & 0xFF;
			}
			constexpr const int32_t& getDWordOffset() const {
				return mOffset;
			}
			void writeSmallestOffset(Os::CodeBlock& block) const {
				block.writeRaw(mOffset);
				block.skipBytes(mOptimalBytes);
			}
			void writeOffset(Os::CodeBlock& block) const {
				block.pushRaw(mOffset);
			}
		};

		class Mem32_Base {
		private:
			Reg32 mBaseReg;
			bool mRspBase;

			constexpr static bool isRsp(Reg64 baseReg) {
				return baseReg == ESP;
			}
		public:
			explicit constexpr Mem32_Base(Reg32 baseReg) : mBaseReg(baseReg), mRspBase(baseReg == ESP) {}

			constexpr bool isRspBase() const {
				return mRspBase;
			}
			constexpr Reg32 getBaseReg() const {
				return mBaseReg;
			}
			void writeEspPostfix(Os::CodeBlock& block) const {
				block.writeRaw<uint8_t>(0x24);
				block.skipBytes(mRspBase ? sizeof(uint8_t) : 0);
			}
		};

		class Mem64_Base {
		private:
			Reg64 mBaseReg;
			bool mRspBase;

			constexpr static bool isRsp(Reg64 baseReg) {
				return baseReg == RSP;
			}
		public:
			explicit constexpr Mem64_Base(Reg64 baseReg) : mBaseReg(baseReg), mRspBase(baseReg == RSP) {}

			constexpr bool isRspBase() const {
				return mRspBase;
			}
			constexpr Reg64 getBaseReg() const {
				return mBaseReg;
			}
			void writeEspPostfix(Os::CodeBlock& block) const {
				block.writeRaw<uint8_t>(0x24);
				block.skipBytes(mRspBase ? sizeof(uint8_t) : 0);
			}
		};

		class Mem64_SIB : public common::SIB {
		private:
			Reg64 mBaseReg;
			Reg64 mIndexReg;
		public:
			explicit constexpr Mem64_SIB(Reg64 index) : mBaseReg(Reg64::RAX), mIndexReg(index), SIB(SCALE_1, removeExtRegBit(index), 0b101) {}
			constexpr Mem64_SIB(IndexScale scale, Reg64 index) : mBaseReg(Reg64::RAX), mIndexReg(index), SIB(scale, removeExtRegBit(index), 0b101) {}
			constexpr Mem64_SIB(Reg64 index, Reg64 base) : mBaseReg(base), mIndexReg(index), SIB(SCALE_1, removeExtRegBit(index), removeExtRegBit(base)) {}
			constexpr Mem64_SIB(IndexScale scale, Reg64 index, Reg64 base) : mBaseReg(base), mIndexReg(index), SIB(scale, removeExtRegBit(index), removeExtRegBit(base)) {}
		
			constexpr Reg64 getBaseReg() const {
				return mBaseReg;
			}

			constexpr Reg64 getIndexReg() const {
				return mIndexReg;
			}
		};

		class Mem32_SIB : public common::SIB {
		private:
			Reg32 mBaseReg;
			Reg32 mIndexReg;
		public:
			explicit constexpr Mem32_SIB(Reg32 index) : mBaseReg(Reg32::EAX), mIndexReg(index), SIB(SCALE_1, removeExtRegBit(index), 0b101) {}
			constexpr Mem32_SIB(IndexScale scale, Reg32 index) : mBaseReg(Reg32::EAX), mIndexReg(index), SIB(scale, removeExtRegBit(index), 0b101) {}
			constexpr Mem32_SIB(Reg32 index, Reg32 base) : mBaseReg(base), mIndexReg(index), SIB(SCALE_1, removeExtRegBit(index), removeExtRegBit(base)) {}
			constexpr Mem32_SIB(IndexScale scale, Reg32 index, Reg32 base) : mBaseReg(base), mIndexReg(index), SIB(scale, removeExtRegBit(index), removeExtRegBit(base)) {}

			constexpr Reg32 getBaseReg() const {
				return mBaseReg;
			}

			constexpr Reg32 getIndexReg() const {
				return mIndexReg;
			}
		};

		class ExtSizePrefix {
		public:
			constexpr static uint8_t FIRST_REG_BIT_OFFSET = 0;
			constexpr static uint8_t BASE_REG_BIT_OFFSET = 0;
			constexpr static uint8_t INDEX_REG_BIT_OFFSET = 1;
			constexpr static uint8_t SECOND_REG_BIT_OFFSET = 2;
			constexpr static uint8_t SIZE_BIT_OFFSET = 3;
		};

		template<MemSize SIZE, class T>
		struct ImmSizeExtend {
			typedef T type;
		};

		template<>
		struct ImmSizeExtend<WORD_PTR, int8_t> {
			typedef int16_t type;
		};

		template<>
		struct ImmSizeExtend<WORD_PTR, uint8_t> {
			typedef uint16_t type;
		};

		template<>
		struct ImmSizeExtend<DWORD_PTR, int8_t> {
			typedef int32_t type;
		};

		template<>
		struct ImmSizeExtend<DWORD_PTR, uint8_t> {
			typedef uint32_t type;
		};

		template<>
		struct ImmSizeExtend<DWORD_PTR, int16_t> {
			typedef int32_t type;
		};

		template<>
		struct ImmSizeExtend<DWORD_PTR, uint16_t> {
			typedef uint32_t type;
		};
	}

	template<AddressMode MODE = OFFSET>
	class Mem64;

	template<AddressMode MODE = OFFSET>
	class Mem32;

	template<>
	class Mem64<OFFSET> : public common::MOD_REG_INDEX, public detail::Mem64_Offset, public detail::Mem64_Seg {
	public:
		explicit constexpr Mem64(int32_t offset) : MOD_REG_INDEX(REG_IND_ADDR), Mem64_Offset(offset), Mem64_Seg() {}
		constexpr Mem64(RegSeg segReg, int32_t offset) : MOD_REG_INDEX(REG_IND_ADDR), Mem64_Offset(offset), Mem64_Seg(segReg) {}

		constexpr uint8_t getExtRegMask() const {
			return 0;
		}

		void write(Os::CodeBlock& block, uint8_t reg) const {
			writeMOD_REG_RM(block, reg);
			block.pushRaw<uint8_t>(0x25);
			writeOffset(block);
		}
	};

	template<>
	class Mem64<BASE> : public common::MOD_REG_RM, public detail::Mem64_Base, public detail::Mem64_Seg {
	public:
		explicit constexpr Mem64(Reg64 baseReg) : MOD_REG_RM(REG_IND_ADDR, detail::removeExtRegBit(baseReg)), 
			Mem64_Base(baseReg), Mem64_Seg() {}
		constexpr Mem64(RegSeg segReg, Reg64 baseReg) : MOD_REG_RM(REG_IND_ADDR, detail::removeExtRegBit(baseReg)), 
			Mem64_Base(baseReg), Mem64_Seg(segReg, baseReg) {}

		constexpr uint8_t getExtRegMask() const {
			return detail::getExtRegMask(getBaseReg(), detail::ExtSizePrefix::BASE_REG_BIT_OFFSET);
		}

		void write(Os::CodeBlock& block, uint8_t reg) const {
			writeMOD_REG_RM(block, reg);
			writeEspPostfix(block);
		}
	};

	template<>
	class Mem32<BASE> : public common::MOD_REG_RM, public detail::Mem32_Base, public detail::Mem32_Seg {
	public:
		explicit constexpr Mem32(Reg32 baseReg) : MOD_REG_RM(REG_IND_ADDR, detail::removeExtRegBit(baseReg)),
			Mem32_Base(baseReg), Mem32_Seg() {}
		constexpr Mem32(RegSeg segReg, Reg32 baseReg) : MOD_REG_RM(REG_IND_ADDR, detail::removeExtRegBit(baseReg)),
			Mem32_Base(baseReg), Mem32_Seg(segReg, baseReg) {}

		constexpr uint8_t getExtRegMask() const {
			return detail::getExtRegMask(getBaseReg(), detail::ExtSizePrefix::BASE_REG_BIT_OFFSET);
		}

		void writePrefix(Os::CodeBlock& block) const {
			block.pushRaw<uint8_t>(0x67);
		}

		void write(Os::CodeBlock& block, uint8_t reg) const {
			writeMOD_REG_RM(block, reg);
			writeEspPostfix(block);
		}
	};

	template<>
	class Mem64<BASE_OFFSET> : public detail::Mem64_Base, public detail::Mem64_Offset, public common::MOD_REG_RM, public detail::Mem64_Seg {
	public:
		constexpr Mem64(Reg64 baseReg, int32_t offset) : Mem64_Base(baseReg), Mem64_Offset(offset), 
			MOD_REG_RM(getDispMod(), detail::removeExtRegBit(baseReg)), Mem64_Seg() {}
		constexpr Mem64(RegSeg segReg, Reg64 baseReg, int32_t offset) : Mem64_Base(baseReg), Mem64_Offset(offset),
			MOD_REG_RM(getDispMod(), detail::removeExtRegBit(baseReg)), Mem64_Seg(segReg, baseReg) {}

		constexpr uint8_t getExtRegMask() const {
			return detail::getExtRegMask(getBaseReg(), detail::ExtSizePrefix::BASE_REG_BIT_OFFSET);
		}

		void write(Os::CodeBlock& block, uint8_t reg) const {
			writeMOD_REG_RM(block, reg);
			writeEspPostfix(block);
			writeSmallestOffset(block);
		}
	};

	template<>
	class Mem32<BASE_OFFSET> : public detail::Mem32_Base, public detail::Mem64_Offset, public common::MOD_REG_RM, public detail::Mem32_Seg {
	public:
		constexpr Mem32(Reg32 baseReg, int32_t offset) : Mem32_Base(baseReg), Mem64_Offset(offset),
			MOD_REG_RM(getDispMod(), detail::removeExtRegBit(baseReg)), Mem32_Seg() {}
		constexpr Mem32(RegSeg segReg, Reg32 baseReg, int32_t offset) : Mem32_Base(baseReg), Mem64_Offset(offset),
			MOD_REG_RM(getDispMod(), detail::removeExtRegBit(baseReg)), Mem32_Seg(segReg, baseReg) {}

		constexpr uint8_t getExtRegMask() const {
			return detail::getExtRegMask(getBaseReg(), detail::ExtSizePrefix::BASE_REG_BIT_OFFSET);
		}

		void writePrefix(Os::CodeBlock& block) const {
			block.pushRaw<uint8_t>(0x67);
		}

		void write(Os::CodeBlock& block, uint8_t reg) const {
			writeMOD_REG_RM(block, reg);
			writeEspPostfix(block);
			writeSmallestOffset(block);
		}
	};

	template<>
	class Mem64<INDEX_OFFSET> : public common::MOD_REG_INDEX, public detail::Mem64_SIB, public detail::Mem64_Offset, public detail::Mem64_Seg {
	public:
		explicit constexpr Mem64(Reg64 indexReg) : MOD_REG_INDEX(SIB_NO_DISP), Mem64_SIB(indexReg), Mem64_Offset(), Mem64_Seg() {}
		constexpr Mem64(Reg64 indexReg, IndexScale indexScale) : MOD_REG_INDEX(SIB_NO_DISP), Mem64_SIB(indexScale, indexReg), Mem64_Offset(), Mem64_Seg() {}
		constexpr Mem64(Reg64 indexReg, int32_t offset) : MOD_REG_INDEX(SIB_NO_DISP), Mem64_SIB(indexReg), Mem64_Offset(offset), Mem64_Seg() {}
		constexpr Mem64(Reg64 indexReg, IndexScale indexScale, int32_t offset) : MOD_REG_INDEX(SIB_NO_DISP), Mem64_SIB(indexScale, indexReg), Mem64_Offset(offset), Mem64_Seg() {}
		constexpr Mem64(RegSeg segReg, Reg64 indexReg) : MOD_REG_INDEX(SIB_NO_DISP), Mem64_SIB(indexReg), Mem64_Offset(), Mem64_Seg(segReg) {}
		constexpr Mem64(RegSeg segReg, Reg64 indexReg, IndexScale indexScale) : MOD_REG_INDEX(SIB_NO_DISP), Mem64_SIB(indexScale, indexReg), Mem64_Offset(), Mem64_Seg(segReg) {}
		constexpr Mem64(RegSeg segReg, Reg64 indexReg, int32_t offset) : MOD_REG_INDEX(SIB_NO_DISP), Mem64_SIB(indexReg), Mem64_Offset(offset), Mem64_Seg(segReg) {}
		constexpr Mem64(RegSeg segReg, Reg64 indexReg, IndexScale indexScale, int32_t offset) : MOD_REG_INDEX(SIB_NO_DISP), Mem64_SIB(indexScale, indexReg), Mem64_Offset(offset), Mem64_Seg(segReg) {}

		constexpr uint8_t getExtRegMask() const {
			return detail::getExtRegMask(getIndexReg(), detail::ExtSizePrefix::INDEX_REG_BIT_OFFSET);
		}

		void write(Os::CodeBlock& block, uint8_t reg) const {
			writeMOD_REG_RM(block, reg);
			writeSIB(block);
			writeOffset(block);
		}
	};

	template<>
	class Mem32<INDEX_OFFSET> : public common::MOD_REG_INDEX, public detail::Mem32_SIB, public detail::Mem64_Offset, public detail::Mem32_Seg {
	public:
		explicit constexpr Mem32(Reg32 indexReg) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexReg), Mem64_Offset(), Mem32_Seg() {}
		constexpr Mem32(Reg32 indexReg, IndexScale indexScale) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexScale, indexReg), Mem64_Offset(), Mem32_Seg() {}
		constexpr Mem32(Reg32 indexReg, int32_t offset) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexReg), Mem64_Offset(offset), Mem32_Seg() {}
		constexpr Mem32(Reg32 indexReg, IndexScale indexScale, int32_t offset) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexScale, indexReg), Mem64_Offset(offset), Mem32_Seg() {}
		constexpr Mem32(RegSeg segReg, Reg32 indexReg) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexReg), Mem64_Offset(), Mem32_Seg(segReg) {}
		constexpr Mem32(RegSeg segReg, Reg32 indexReg, IndexScale indexScale) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexScale, indexReg), Mem64_Offset(), Mem32_Seg(segReg) {}
		constexpr Mem32(RegSeg segReg, Reg32 indexReg, int32_t offset) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexReg), Mem64_Offset(offset), Mem32_Seg(segReg) {}
		constexpr Mem32(RegSeg segReg, Reg32 indexReg, IndexScale indexScale, int32_t offset) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexScale, indexReg), Mem64_Offset(offset), Mem32_Seg(segReg) {}

		constexpr uint8_t getExtRegMask() const {
			return detail::getExtRegMask(getIndexReg(), detail::ExtSizePrefix::INDEX_REG_BIT_OFFSET);
		}

		void writePrefix(Os::CodeBlock& block) const {
			block.pushRaw<uint8_t>(0x67);
		}

		void write(Os::CodeBlock& block, uint8_t reg) const {
			writeMOD_REG_RM(block, reg);
			writeSIB(block);
			writeOffset(block);
		}
	};

	template<>
	class Mem64<BASE_INDEX> : public common::MOD_REG_INDEX, public detail::Mem64_SIB, public detail::Mem64_Seg {
	public:
		constexpr Mem64(Reg64 baseReg, Reg64 indexReg) : MOD_REG_INDEX(SIB_NO_DISP), Mem64_SIB(indexReg, baseReg), Mem64_Seg() {}
		constexpr Mem64(Reg64 baseReg, Reg64 indexReg, IndexScale indexScale) : MOD_REG_INDEX(SIB_NO_DISP), Mem64_SIB(indexScale, indexReg, baseReg), Mem64_Seg() {}
		constexpr Mem64(RegSeg segReg, Reg64 baseReg, Reg64 indexReg) : MOD_REG_INDEX(SIB_NO_DISP), Mem64_SIB(indexReg, baseReg), Mem64_Seg(segReg, baseReg) {}
		constexpr Mem64(RegSeg segReg, Reg64 baseReg, Reg64 indexReg, IndexScale indexScale) : MOD_REG_INDEX(SIB_NO_DISP), Mem64_SIB(indexScale, indexReg, baseReg), Mem64_Seg(segReg, baseReg) {}

		constexpr uint8_t getExtRegMask() const {
			return detail::getExtRegMask(getBaseReg(), detail::ExtSizePrefix::BASE_REG_BIT_OFFSET) |
				detail::getExtRegMask(getIndexReg(), detail::ExtSizePrefix::INDEX_REG_BIT_OFFSET);
		}

		void write(Os::CodeBlock& block, uint8_t reg) const {
			writeMOD_REG_RM(block, reg);
			writeSIB(block);
		}
	};

	template<>
	class Mem32<BASE_INDEX> : public common::MOD_REG_INDEX, public detail::Mem32_SIB, public detail::Mem32_Seg {
	public:
		constexpr Mem32(Reg32 baseReg, Reg32 indexReg) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexReg, baseReg), Mem32_Seg() {}
		constexpr Mem32(Reg32 baseReg, Reg32 indexReg, IndexScale indexScale) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexScale, indexReg, baseReg), Mem32_Seg() {}
		constexpr Mem32(RegSeg segReg, Reg32 baseReg, Reg32 indexReg) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexReg, baseReg), Mem32_Seg(segReg, baseReg) {}
		constexpr Mem32(RegSeg segReg, Reg32 baseReg, Reg32 indexReg, IndexScale indexScale) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexScale, indexReg, baseReg), Mem32_Seg(segReg, baseReg) {}

		constexpr uint8_t getExtRegMask() const {
			return detail::getExtRegMask(getBaseReg(), detail::ExtSizePrefix::BASE_REG_BIT_OFFSET) |
				detail::getExtRegMask(getIndexReg(), detail::ExtSizePrefix::INDEX_REG_BIT_OFFSET);
		}

		void writePrefix(Os::CodeBlock& block) const {
			block.pushRaw<uint8_t>(0x67);
		}

		void write(Os::CodeBlock& block, uint8_t reg) const {
			writeMOD_REG_RM(block, reg);
			writeSIB(block);
		}
	};

	template<>
	class Mem64<BASE_INDEX_OFFSET> : public detail::Mem64_SIB, public detail::Mem64_Offset, public common::MOD_REG_INDEX, public detail::Mem64_Seg {
	public:
		constexpr Mem64(Reg64 baseReg, Reg64 indexReg, int32_t offset) : Mem64_SIB(indexReg, baseReg), Mem64_Offset(offset), MOD_REG_INDEX(getDispMod()), Mem64_Seg() {}
		constexpr Mem64(Reg64 baseReg, Reg64 indexReg, IndexScale indexScale, int32_t offset) : Mem64_SIB(indexScale, indexReg, baseReg), Mem64_Offset(offset), MOD_REG_INDEX(getDispMod()), Mem64_Seg() {}
		constexpr Mem64(RegSeg segReg, Reg64 baseReg, Reg64 indexReg, int32_t offset) : Mem64_SIB(indexReg, baseReg), Mem64_Offset(offset), MOD_REG_INDEX(getDispMod()), Mem64_Seg(segReg, baseReg) {}
		constexpr Mem64(RegSeg segReg, Reg64 baseReg, Reg64 indexReg, IndexScale indexScale, int32_t offset) : Mem64_SIB(indexScale, indexReg, baseReg), Mem64_Offset(offset), MOD_REG_INDEX(getDispMod()), Mem64_Seg(segReg, baseReg) {}

		constexpr uint8_t getExtRegMask() const {
			return detail::getExtRegMask(getBaseReg(), detail::ExtSizePrefix::BASE_REG_BIT_OFFSET) |
				detail::getExtRegMask(getIndexReg(), detail::ExtSizePrefix::INDEX_REG_BIT_OFFSET);
		}

		void write(Os::CodeBlock& block, uint8_t reg) const {
			writeMOD_REG_RM(block, reg);
			writeSIB(block);
			writeSmallestOffset(block);
		}
	};

	template<>
	class Mem32<BASE_INDEX_OFFSET> : public detail::Mem32_SIB, public detail::Mem64_Offset, public common::MOD_REG_INDEX, public detail::Mem32_Seg {
	public:
		constexpr Mem32(Reg32 baseReg, Reg32 indexReg, int32_t offset) : Mem32_SIB(indexReg, baseReg), Mem64_Offset(offset), MOD_REG_INDEX(getDispMod()), Mem32_Seg() {}
		constexpr Mem32(Reg32 baseReg, Reg32 indexReg, IndexScale indexScale, int32_t offset) : Mem32_SIB(indexScale, indexReg, baseReg), Mem64_Offset(offset), MOD_REG_INDEX(getDispMod()), Mem32_Seg() {}
		constexpr Mem32(RegSeg segReg, Reg32 baseReg, Reg32 indexReg, int32_t offset) : Mem32_SIB(indexReg, baseReg), Mem64_Offset(offset), MOD_REG_INDEX(getDispMod()), Mem32_Seg(segReg, baseReg) {}
		constexpr Mem32(RegSeg segReg, Reg32 baseReg, Reg32 indexReg, IndexScale indexScale, int32_t offset) : Mem32_SIB(indexScale, indexReg, baseReg), Mem64_Offset(offset), MOD_REG_INDEX(getDispMod()), Mem32_Seg(segReg, baseReg) {}

		constexpr uint8_t getExtRegMask() const {
			return detail::getExtRegMask(getBaseReg(), detail::ExtSizePrefix::BASE_REG_BIT_OFFSET) |
				detail::getExtRegMask(getIndexReg(), detail::ExtSizePrefix::INDEX_REG_BIT_OFFSET);
		}

		void writePrefix(Os::CodeBlock& block) const {
			block.pushRaw<uint8_t>(0x67);
		}

		void write(Os::CodeBlock& block, uint8_t reg) const {
			writeMOD_REG_RM(block, reg);
			writeSIB(block);
			writeSmallestOffset(block);
		}
	};

	/*class Mem64
	{
	public:
		
	private:
		AddressMode mAddressMode;
		Reg64 mBaseReg;
		Reg64 mIndexReg;
		uint64_t mOffset;
		uint8_t mMultiplier;
	public:
		explicit Mem64(uint64_t offset): mAddressMode(OFFSET), mOffset(offset) {}
		explicit Mem64(Reg64 baseReg) : mAddressMode(BASE), mBaseReg(baseReg) {}
		Mem64(Reg64 baseReg, uint64_t offset) : mAddressMode(BASE_OFFSET), mBaseReg(baseReg), mOffset(offset) {}
		void writeSegmPrefix(CodeBlock& block, X86::RegSeg defaultSegm = X86::DS) const {}
		AddressMode getAddressMode() const {
			return mAddressMode;
		}
		Reg64 getBaseReg() const {
			return mBaseReg;
		}
		uint64_t getOffset() const {
			return mOffset;
		}
	};

	class SegmMem64 : public Mem64 {
	private:
		X86::RegSeg mSegmReg;
	public:
		SegmMem64(X86::RegSeg segment, uint64_t offset) : Mem64(offset), mSegmReg(segment) {}
		SegmMem64(X86::RegSeg segment, Reg64 baseReg) : Mem64(baseReg), mSegmReg(segment) {}
		SegmMem64(X86::RegSeg segment, Reg64 baseReg, uint64_t offset) : Mem64(baseReg, offset), mSegmReg(segment) {}
		void writeSegmPrefix(CodeBlock& block, X86::RegSeg defaultSegm = X86::DS) const {
			if (mSegmReg != defaultSegm) {
				block.pushRaw<uint8_t>(mSegmReg);
			}
		}
		X86::RegSeg getSegmReg() const {
			return mSegmReg;
		}
	};*/
}