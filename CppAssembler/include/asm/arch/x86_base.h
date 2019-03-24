#pragma once


#include <cassert>

#include "x86_64_base.h"

namespace CppAsm::X86
{
	namespace
	{
		namespace common = CppAsm::X86_64::detail;
	}

	constexpr static endian byteOrder = common::byteOrder;

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
		EAX = 0b000,
		ECX = 0b001,
		EDX = 0b010,
		EBX = 0b011,
		ESP = 0b100,
		EBP = 0b101,
		ESI = 0b110,
		EDI = 0b111
	};

	enum RegSeg : uint8_t {
		ES = 0x26,
		CS = 0x2E,
		SS = 0x36,
		DS = 0x3E,
		FS = 0x64,
		GS = 0x65
	};

	enum RegDebug : uint8_t {
		DR0 = 0,
		DR1,
		DR2,
		DR3,
		DR4,
		DR5,
		DR6,
		DR7
	};

	enum RegControl : uint8_t {
		CR0 = 0,
		CR1, // reversed
		CR2,
		CR3,
		CR4
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
		FWORD_PTR,
		QWORD_PTR,
		TBYTE_PTR
	};

	enum JmpSize {
		SHORT,
		LONG
	};

	enum RepPrefix : uint8_t {
		NO_REP = 0x00,
		REP = 0xF3,
		REPE = 0xF3,
		REPZ = 0xF3,
		REPNE = 0xF2,
		REPNZ = 0xF2
	};

	enum LockPrefix : uint8_t {
		NO_LOCK = 0,
		LOCK = 0xF0
	};

	enum AddressMode {
		OFFSET,			// segment:[offset]
		BASE,			// segment:[base_reg]
		BASE_OFFSET,	// segment:[base_reg+offset]
		INDEX_OFFSET,	// segment:[scale*index_reg+offset]
		BASE_INDEX,		// segment:[base_reg+scale*index_reg]
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
			template<class BLOCK>
			void writeSegmPrefix(BLOCK& block) const {
				block.writeRaw<byteOrder>(mSegReg);
				block.skipBytes(mCustomSegReg ? sizeof(mSegReg) : 0);
			}
		};

		class Mem32_Offset {
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
			constexpr Mem32_Offset() : mOffset(0), mOptimalBytes(sizeof(uint8_t)), mByteOffset(true), mDispMod(common::MOD_REG_RM::DISP_ADDR_1BYTE) {}
			explicit constexpr Mem32_Offset(int8_t offset) : mOffset(offset), mOptimalBytes(sizeof(uint8_t)), mByteOffset(true), mDispMod(common::MOD_REG_RM::DISP_ADDR_1BYTE) {}
			explicit constexpr Mem32_Offset(int32_t offset) : mOffset(offset), mOptimalBytes(calcOptimalBytes(offset)), mByteOffset(mOptimalBytes == 1), mDispMod(calcDispMod(offset)) {}

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

			template<class BLOCK>
			void writeSmallestOffset(BLOCK& block) const {
				block.writeRaw<byteOrder>(mOffset);
				block.skipBytes(mOptimalBytes);
			}

			template<class BLOCK>
			void writeOffset(BLOCK& block) const {
				block.pushRaw<byteOrder>(mOffset);
			}
		};

		class Mem32_Base {
		private:
			Reg32 mBaseReg;
			bool mEspBase;

			constexpr static bool isEsp(Reg32 baseReg) {
				return baseReg == ESP;
			}
		public:
			explicit constexpr Mem32_Base(Reg32 baseReg) : mBaseReg(baseReg), mEspBase(baseReg == ESP) {}

			constexpr bool isEspBase() const {
				return mEspBase;
			}
			constexpr Reg32 getBaseReg() const {
				return mBaseReg;
			}

			template<class BLOCK>
			void writeEspPostfix(BLOCK& block) const {
				block.writeRaw<byteOrder, uint8_t>(0x24);
				block.skipBytes(mEspBase ? sizeof(uint8_t) : 0);
			}
		};

		class Mem32_SIB: public common::SIB {
		public:
			explicit constexpr Mem32_SIB(Reg32 index) : SIB(SCALE_1, index, 0b101) {}
			constexpr Mem32_SIB(IndexScale scale, Reg32 index) : SIB(scale, index, 0b101) {}
			constexpr Mem32_SIB(Reg32 index, Reg32 base) : SIB(SCALE_1, index, base) {}
			constexpr Mem32_SIB(IndexScale scale, Reg32 index, Reg32 base) : SIB(scale, index, base) {}
		};

		class OpcodeLarge {
		private:
			common::Opcode mOpcode;
			uint8_t mMode;
		public:
			constexpr OpcodeLarge(common::Opcode opcode, uint8_t mode) : mOpcode(opcode), mMode(mode) {}
			OpcodeLarge(const OpcodeLarge&) = default;
			void operator=(const OpcodeLarge&) = delete;
			constexpr common::Opcode getOpcode() const {
				return mOpcode;
			}
			constexpr uint8_t getMode() const {
				return mMode;
			}
		};

		class OpcodeSet {
		private:
			common::Opcode mMainOpcode;
			OpcodeLarge mSecondOpcode;
		public:
			constexpr OpcodeSet(common::Opcode mainOpcode, const OpcodeLarge& secondOpcode) :
				mMainOpcode(mainOpcode), mSecondOpcode(secondOpcode) {}
			OpcodeSet(const OpcodeSet&) = delete;
			void operator=(const OpcodeSet&) = delete;
			constexpr common::Opcode getMain() const {
				return mMainOpcode;
			}
			constexpr const OpcodeLarge& getSecond() const {
				return mSecondOpcode;
			}
		};

		template<MemSize SIZE, class T>
		struct ImmSizeOptimize {
			typedef T type;
		};

		template<>
		struct ImmSizeOptimize<DWORD_PTR, int16_t> {
			typedef int32_t type;
		};

		template<>
		struct ImmSizeOptimize<DWORD_PTR, uint16_t> {
			typedef int32_t type;
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
		constexpr static OpcodeSet opcode_ADD(0x00, OpcodeLarge(0x80, 0b000));
		constexpr static OpcodeSet opcode_OR(0x08, OpcodeLarge(0x80, 0b001));
		constexpr static OpcodeSet opcode_ADC(0x10, OpcodeLarge(0x80, 0b010));
		constexpr static OpcodeSet opcode_SBB(0x18, OpcodeLarge(0x80, 0b011));
		constexpr static OpcodeSet opcode_AND(0x20, OpcodeLarge(0x80, 0b100));
		constexpr static OpcodeSet opcode_SUB(0x28, OpcodeLarge(0x80, 0b101));
		constexpr static OpcodeSet opcode_XOR(0x30, OpcodeLarge(0x80, 0b110));
		constexpr static OpcodeSet opcode_CMP(0x38, OpcodeLarge(0x80, 0b111));

		constexpr static OpcodeSet opcode_MOV(0x88, OpcodeLarge(0xC6, 0b000));
		constexpr static OpcodeSet opcode_TEST(0x84, OpcodeLarge(0xF6, 0b000));

		constexpr static OpcodeSet opcode_BT(0xA3, OpcodeLarge(0xBA, 0b100));
		constexpr static OpcodeSet opcode_BTS(0xAB, OpcodeLarge(0xBA, 0b101));
		constexpr static OpcodeSet opcode_BTR(0xB3, OpcodeLarge(0xBA, 0b110));
		constexpr static OpcodeSet opcode_BTC(0xBB, OpcodeLarge(0xBA, 0b111));

		constexpr static OpcodeLarge opcode_SLDT(0x00, 0b000);
		constexpr static OpcodeLarge opcode_STR(0x00, 0b001);
		constexpr static OpcodeLarge opcode_LLDT(0x00, 0b010);
		constexpr static OpcodeLarge opcode_LTR(0x00, 0b011);
		constexpr static OpcodeLarge opcode_VERR(0x00, 0b100);
		constexpr static OpcodeLarge opcode_VERW(0x00, 0b101);
		constexpr static OpcodeLarge opcode_SGDT(0x01, 0b000);
		constexpr static OpcodeLarge opcode_SIDT(0x01, 0b001);
		constexpr static OpcodeLarge opcode_LGDT(0x01, 0b010);
		constexpr static OpcodeLarge opcode_LIDT(0x01, 0b011);
		constexpr static OpcodeLarge opcode_SMSW(0x01, 0b100);
		constexpr static OpcodeLarge opcode_LMSW(0x01, 0b110);

		constexpr static OpcodeLarge opcode_POP(0x8F, 0b000);

		constexpr static OpcodeLarge opcode_NOT(0xF6, 0b010);
		constexpr static OpcodeLarge opcode_NEG(0xF6, 0b011);
		constexpr static OpcodeLarge opcode_MUL(0xF6, 0b100);
		constexpr static OpcodeLarge opcode_IMUL(0xF6, 0b101);
		constexpr static OpcodeLarge opcode_DIV(0xF6, 0b110);
		constexpr static OpcodeLarge opcode_IDIV(0xF6, 0b111);

		constexpr static OpcodeLarge opcode_INC(0xFE, 0b000);
		constexpr static OpcodeLarge opcode_DEC(0xFE, 0b001);

		constexpr static OpcodeLarge opcode_CALL_NEAR(0xFF, 0b010);
		constexpr static OpcodeLarge opcode_CALL_FAR(0xFF, 0b011);
		constexpr static OpcodeLarge opcode_JMP_NEAR(0xFF, 0b100);
		constexpr static OpcodeLarge opcode_JMP_FAR(0xFF, 0b101);
		constexpr static OpcodeLarge opcode_PUSH(0xFF, 0b110);

		constexpr static OpcodeLarge opcode_ROL(0xC0, 0b000);
		constexpr static OpcodeLarge opcode_ROR(0xC0, 0b001);
		constexpr static OpcodeLarge opcode_RCL(0xC0, 0b010);
		constexpr static OpcodeLarge opcode_RCR(0xC0, 0b011);
		constexpr static OpcodeLarge opcode_SHL(0xC0, 0b100);
		constexpr static OpcodeLarge opcode_SHR(0xC0, 0b101);
		constexpr static OpcodeLarge opcode_SAL(0xC0, 0b110);
		constexpr static OpcodeLarge opcode_SAR(0xC0, 0b111);
	}

	template<class T>
	struct TypeBitsMask;

	template<>
	struct TypeBitsMask<Reg32> {
		constexpr static Reg32 value = Reg32(0b111);
	};

	template<>
	struct TypeBitsMask<Reg16> {
		constexpr static Reg16 value = Reg16(0b111);
	};

	template<>
	struct TypeBitsMask<Reg8> {
		constexpr static Reg8 value = Reg8(0b111);
	};

	template<>
	struct TypeBitsMask<IndexScale> {
		constexpr static IndexScale value = IndexScale(0b11);
	};

	template<class REG>
	class ReplaceableReg final : public ReplaceableBits<REG, byteOrder, TypeBitsMask<REG>::value> {
	public:
		using ReplaceableBits<REG, byteOrder, TypeBitsMask<REG>::value>::ReplaceableBits;
	};

	typedef ReplaceableBits<IndexScale, byteOrder, TypeBitsMask<IndexScale>::value> ReplaceableIndexScale;

	template<class T>
	using ReplaceableImmediate = ReplaceableValue<T, byteOrder>;

	template<AddressMode MODE = OFFSET>
	class ReplaceableMem32;

	template<AddressMode MODE = OFFSET>
	class Mem32;

	template<>
	class ReplaceableMem32<OFFSET> final : public Replaceable {
	public:
		using Replaceable::Replaceable;

		template<class REG>
		constexpr ReplaceableReg<REG> getOtherReg() const {
			return ReplaceableReg<REG>(getCbOffset(), common::MOD_REG_RM::REG_BIT_OFFSET);
		}

		constexpr ReplaceableImmediate<int32_t> getOffset() const {
			return ReplaceableImmediate<int32_t>(getCbOffset() + 1); // +1 skip MOD_REG_RM byte
		}
	};

	template<>
	class Mem32<OFFSET> final : public common::MOD_REG_OFFSET, public detail::Mem32_Offset, public detail::Mem32_Seg {
	public:
		explicit constexpr Mem32(int32_t offset) : MOD_REG_OFFSET(REG_IND_ADDR), Mem32_Offset(offset), Mem32_Seg() {}
		constexpr Mem32(RegSeg segReg, int32_t offset) : MOD_REG_OFFSET(REG_IND_ADDR), Mem32_Offset(offset), Mem32_Seg(segReg) {}

		template<class BLOCK>
		ReplaceableMem32<OFFSET> write(BLOCK& block, uint8_t reg) const {
			Offset offset = block.getOffset();
			writeMOD_REG_RM(block, reg);
			writeOffset(block);
			return ReplaceableMem32<OFFSET>(offset);
		}
	};

	template<>
	class ReplaceableMem32<BASE> final : public Replaceable {
	private:
		bool mEspBase;
	public:
		constexpr ReplaceableMem32(Offset offset, bool espBase) : Replaceable(offset), mEspBase(espBase) {}

		template<class REG>
		constexpr ReplaceableReg<REG> getOtherReg() const {
			return ReplaceableReg<REG>(getCbOffset(), common::MOD_REG_RM::REG_BIT_OFFSET);
		}

		constexpr bool canReplaceBaseReg() const {
			return !mEspBase;
		}

		template<class REG>
		constexpr ReplaceableReg<REG> getBaseReg() const {
			return assert(!mEspBase), ReplaceableReg<REG>(getCbOffset(), common::MOD_REG_RM::RM_BIT_OFFSET);
		}
	};

	template<>
	class Mem32<BASE> final : public common::MOD_REG_RM, public detail::Mem32_Base, public detail::Mem32_Seg {
	public:
		explicit constexpr Mem32(Reg32 baseReg) : MOD_REG_RM(REG_IND_ADDR, baseReg), Mem32_Base(baseReg), Mem32_Seg() {}
		constexpr Mem32(RegSeg segReg, Reg32 baseReg) : MOD_REG_RM(REG_IND_ADDR, baseReg), Mem32_Base(baseReg), Mem32_Seg(segReg, baseReg) {}

		template<class BLOCK>
		ReplaceableMem32<BASE> write(BLOCK& block, uint8_t reg) const {
			Offset offset = block.getOffset();
			writeMOD_REG_RM(block, reg);
			writeEspPostfix(block);
			return ReplaceableMem32<BASE>(offset, isEspBase());
		}
	};

	template<>
	class ReplaceableMem32<BASE_OFFSET> final : public Replaceable {
	private:
		bool mEspBase;
		bool mByteOffset;
	public:
		constexpr ReplaceableMem32(Offset offset, bool espBase, bool byteOffset) : Replaceable(offset), mEspBase(espBase), mByteOffset(byteOffset) {}

		template<class REG>
		constexpr ReplaceableReg<REG> getOtherReg() const {
			return ReplaceableReg<REG>(getCbOffset(), common::MOD_REG_RM::REG_BIT_OFFSET);
		}

		constexpr bool canReplaceBaseReg() const {
			return !mEspBase;
		}

		constexpr bool isByteOffset() const {
			return mByteOffset;
		}

		template<class REG>
		constexpr ReplaceableReg<REG> getBaseReg() const {
			return assert(!mEspBase), ReplaceableReg<REG>(getCbOffset(), common::MOD_REG_RM::RM_BIT_OFFSET);
		}

		constexpr ReplaceableImmediate<int32_t> getDWordOffset() const {
			return assert(!mByteOffset), ReplaceableImmediate<int32_t>(getCbOffset() + (mEspBase ? 1 : 0) + 1); // +1 skip MOD_REG_RM byte
		}

		constexpr ReplaceableImmediate<int8_t> getByteOffset() const {
			return assert(mByteOffset), ReplaceableImmediate<int8_t>(getCbOffset() + (mEspBase ? 1 : 0) + 1); // +1 skip MOD_REG_RM byte
		}
	};

	template<>
	class Mem32<BASE_OFFSET> final : public detail::Mem32_Base, public detail::Mem32_Offset, public common::MOD_REG_RM, public detail::Mem32_Seg {
	public:
		constexpr Mem32(Reg32 baseReg, int32_t offset) : Mem32_Base(baseReg), Mem32_Offset(offset), MOD_REG_RM(getDispMod(), baseReg), Mem32_Seg() {}
		constexpr Mem32(RegSeg segReg, Reg32 baseReg, int32_t offset) : Mem32_Base(baseReg), Mem32_Offset(offset), MOD_REG_RM(getDispMod(), baseReg), Mem32_Seg(segReg, baseReg) {}

		template<class BLOCK>
		ReplaceableMem32<BASE_OFFSET> write(BLOCK& block, uint8_t reg) const {
			Offset offset = block.getOffset();
			writeMOD_REG_RM(block, reg);
			writeEspPostfix(block);
			writeSmallestOffset(block);
			return ReplaceableMem32<BASE_OFFSET>(offset, isEspBase(), isByteOffset());
		}
	};

	template<>
	class ReplaceableMem32<INDEX_OFFSET> final : public Replaceable {
	public:
		using Replaceable::Replaceable;

		template<class REG>
		constexpr ReplaceableReg<REG> getOtherReg() const {
			return ReplaceableReg<REG>(getCbOffset(), common::MOD_REG_RM::REG_BIT_OFFSET);
		}

		template<class REG>
		constexpr ReplaceableReg<REG> getIndexReg() const {
			// +1 skip MOD_REG_RM byte
			return ReplaceableReg<REG>(getCbOffset() + 1, detail::Mem32_SIB::INDEX_BIT_OFFSET);
		}

		constexpr ReplaceableIndexScale getIndexScale() const {
			// +1 skip MOD_REG_RM byte
			return ReplaceableIndexScale(getCbOffset() + 1, detail::Mem32_SIB::SCALE_BIT_OFFSET);
		}

		constexpr ReplaceableImmediate<int32_t> getOffset() const {
			// +2 skip MOD_REG_RM and SIB bytes
			return ReplaceableImmediate<int32_t>(getCbOffset() + 2);
		}
	};

	template<>
	class Mem32<INDEX_OFFSET> final : public common::MOD_REG_INDEX, public detail::Mem32_SIB, public detail::Mem32_Offset, public detail::Mem32_Seg {
	public:
		explicit constexpr Mem32(Reg32 indexReg) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexReg), Mem32_Offset(), Mem32_Seg() {}
		constexpr Mem32(Reg32 indexReg, IndexScale indexScale) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexScale, indexReg), Mem32_Offset(), Mem32_Seg() {}
		constexpr Mem32(Reg32 indexReg, int32_t offset) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexReg), Mem32_Offset(offset), Mem32_Seg() {}
		constexpr Mem32(Reg32 indexReg, IndexScale indexScale, int32_t offset) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexScale, indexReg), Mem32_Offset(offset), Mem32_Seg() {}
		constexpr Mem32(RegSeg segReg, Reg32 indexReg) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexReg), Mem32_Offset(), Mem32_Seg(segReg) {}
		constexpr Mem32(RegSeg segReg, Reg32 indexReg, IndexScale indexScale) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexScale, indexReg), Mem32_Offset(), Mem32_Seg(segReg) {}
		constexpr Mem32(RegSeg segReg, Reg32 indexReg, int32_t offset) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexReg), Mem32_Offset(offset), Mem32_Seg(segReg) {}
		constexpr Mem32(RegSeg segReg, Reg32 indexReg, IndexScale indexScale, int32_t offset) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexScale, indexReg), Mem32_Offset(offset), Mem32_Seg(segReg) {}

		template<class BLOCK>
		ReplaceableMem32<INDEX_OFFSET> write(BLOCK& block, uint8_t reg) const {
			Offset offset = block.getOffset();
			writeMOD_REG_RM(block, reg);
			writeSIB(block);
			writeOffset(block);
			return ReplaceableMem32<INDEX_OFFSET>(offset);
		}
	};

	template<>
	class ReplaceableMem32<BASE_INDEX> final : public Replaceable {
	public:
		using Replaceable::Replaceable;

		template<class REG>
		constexpr ReplaceableReg<REG> getOtherReg() const {
			return ReplaceableReg<REG>(getCbOffset(), common::MOD_REG_RM::REG_BIT_OFFSET);
		}

		template<class REG>
		constexpr ReplaceableReg<REG> getBaseReg() const {
			// +1 skip MOD_REG_RM byte
			return ReplaceableReg<REG>(getCbOffset() + 1, common::Mem32_SIB::BASE_BIT_OFFSET);
		}

		template<class REG>
		constexpr ReplaceableReg<REG> getIndexReg() const {
			// +1 skip MOD_REG_RM byte
			return ReplaceableReg<REG>(getCbOffset() + 1, common::Mem32_SIB::INDEX_BIT_OFFSET);
		}

		constexpr ReplaceableIndexScale getIndexScale() const {
			// +1 skip MOD_REG_RM byte
			return ReplaceableIndexScale(getCbOffset() + 1, detail::Mem32_SIB::SCALE_BIT_OFFSET);
		}
	};

	template<>
	class Mem32<BASE_INDEX> final : public common::MOD_REG_INDEX, public detail::Mem32_SIB, public detail::Mem32_Seg {
	public:
		constexpr Mem32(Reg32 baseReg, Reg32 indexReg) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexReg, baseReg), Mem32_Seg() {}
		constexpr Mem32(Reg32 baseReg, Reg32 indexReg, IndexScale indexScale) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexScale, indexReg, baseReg), Mem32_Seg() {}
		constexpr Mem32(RegSeg segReg, Reg32 baseReg, Reg32 indexReg) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexReg, baseReg), Mem32_Seg(segReg, baseReg) {}
		constexpr Mem32(RegSeg segReg, Reg32 baseReg, Reg32 indexReg, IndexScale indexScale) : MOD_REG_INDEX(SIB_NO_DISP), Mem32_SIB(indexScale, indexReg, baseReg), Mem32_Seg(segReg, baseReg) {}

		template<class BLOCK>
		ReplaceableMem32<BASE_INDEX> write(BLOCK& block, uint8_t reg) const {
			Offset offset = block.getOffset();
			writeMOD_REG_RM(block, reg);
			writeSIB(block);
			return ReplaceableMem32<BASE_INDEX>(offset);
		}
	};

	template<>
	class ReplaceableMem32<BASE_INDEX_OFFSET> final : public Replaceable {
	private:
		bool mByteOffset;
	public:
		constexpr ReplaceableMem32(Offset offset, bool byteOffset) : Replaceable(offset), mByteOffset(byteOffset) {}

		template<class REG>
		constexpr ReplaceableReg<REG> getOtherReg() const {
			return ReplaceableReg<REG>(getCbOffset(), common::MOD_REG_RM::REG_BIT_OFFSET);
		}

		constexpr bool isByteOffset() const {
			return mByteOffset;
		}

		template<class REG>
		constexpr ReplaceableReg<REG> getBaseReg() const {
			// +1 skip MOD_REG_RM byte
			return ReplaceableReg<REG>(getCbOffset() + 1, detail::Mem32_SIB::BASE_BIT_OFFSET);
		}

		template<class REG>
		constexpr ReplaceableReg<REG> getIndexReg() const {
			// +1 skip MOD_REG_RM byte
			return ReplaceableReg<REG>(getCbOffset() + 1, detail::Mem32_SIB::INDEX_BIT_OFFSET);
		}

		constexpr ReplaceableIndexScale getIndexScale() const {
			// +1 skip MOD_REG_RM byte
			return ReplaceableIndexScale(getCbOffset() + 1, detail::Mem32_SIB::SCALE_BIT_OFFSET);
		}

		constexpr ReplaceableImmediate<int32_t> getDWordOffset() const {
			// +2 skip MOD_REG_RM and SIB bytes
			return assert(!mByteOffset), ReplaceableImmediate<int32_t>(getCbOffset() + 2);
		}

		constexpr ReplaceableImmediate<int8_t> getByteOffset() const {
			// +2 skip MOD_REG_RM and SIB bytes
			return assert(mByteOffset), ReplaceableImmediate<int8_t>(getCbOffset() + 2);
		}
	};

	template<>
	class Mem32<BASE_INDEX_OFFSET> final : public detail::Mem32_SIB, public detail::Mem32_Offset, public common::MOD_REG_INDEX, public detail::Mem32_Seg {
	public:
		constexpr Mem32(Reg32 baseReg, Reg32 indexReg, int32_t offset) : Mem32_SIB(indexReg, baseReg), Mem32_Offset(offset), MOD_REG_INDEX(getDispMod()), Mem32_Seg() {}
		constexpr Mem32(Reg32 baseReg, Reg32 indexReg, IndexScale indexScale, int32_t offset) : Mem32_SIB(indexScale, indexReg, baseReg), Mem32_Offset(offset), MOD_REG_INDEX(getDispMod()), Mem32_Seg() {}
		constexpr Mem32(RegSeg segReg, Reg32 baseReg, Reg32 indexReg, int32_t offset) : Mem32_SIB(indexReg, baseReg), Mem32_Offset(offset), MOD_REG_INDEX(getDispMod()), Mem32_Seg(segReg, baseReg) {}
		constexpr Mem32(RegSeg segReg, Reg32 baseReg, Reg32 indexReg, IndexScale indexScale, int32_t offset) : Mem32_SIB(indexScale, indexReg, baseReg), Mem32_Offset(offset), MOD_REG_INDEX(getDispMod()), Mem32_Seg(segReg, baseReg) {}

		template<class BLOCK>
		ReplaceableMem32<BASE_INDEX_OFFSET> write(BLOCK& block, uint8_t reg) const {
			Offset offset = block.getOffset();
			writeMOD_REG_RM(block, reg);
			writeSIB(block);
			writeSmallestOffset(block);
			return ReplaceableMem32<BASE_INDEX_OFFSET>(offset, isByteOffset());
		}
	};

	template<MemSize SIZE, class T>
	class ReplaceableOptimizedImm : public ReplaceableImmediate<typename detail::ImmSizeOptimize<SIZE, T>::type> {
	private:
		typedef ReplaceableImmediate<typename detail::ImmSizeOptimize<SIZE, T>::type> ParentType;
	public:
		using ParentType::ParentType;
	};

	template<MemSize SIZE, class T>
	class ReplaceableExtendedImm : public ReplaceableImmediate<typename detail::ImmSizeExtend<SIZE, T>::type> {
	private:
		typedef ReplaceableImmediate<typename detail::ImmSizeExtend<SIZE, T>::type> ParentType;
	public:
		using ParentType::ParentType;
	};

	template<JmpSize SIZE>
	class FwdLabel;

	template<>
	class FwdLabel<LONG> final : public Replaceable {
	public:
		using Replaceable::Replaceable;

		typedef Addr offset_type;
		static const int offset_size = sizeof(offset_type);

		template<class BLOCK>
		bool bind(BLOCK& block, Addr newAddr) const {
			Offset labelOffset = getCbOffset();
			auto jumpOffset = common::calc_Jump_Offset(block.getStartPtr() + labelOffset,
				newAddr, offset_size);
			block.writeRaw<byteOrder>(reinterpret_cast<offset_type>(jumpOffset), labelOffset);
			return true;
		}

		template<class BLOCK>
		bool bind(BLOCK& block) const {
			return bind(block, block.getCurrentPtr());
		}
	};

	template<>
	class FwdLabel<SHORT> final : public Replaceable {
	public:
		using Replaceable::Replaceable;

		typedef int8_t offset_type;
		static const int offset_size = sizeof(offset_type);

		template<class BLOCK>
		bool bind(BLOCK& block, Addr newAddr) const {
			Offset labelOffset = getCbOffset();
			auto jumpOffset = common::calc_Jump_Offset(block.getStartPtr() + labelOffset,
				newAddr, offset_size);
			if (common::is_Byte_Offset(jumpOffset)) {
				block.writeRaw<byteOrder>(static_cast<offset_type>(jumpOffset), labelOffset);
				return true;
			}
			return false;
		}

		template<class BLOCK>
		bool bind(BLOCK& block) const {
			return bind(block, block.getCurrentPtr());
		}
	};
}