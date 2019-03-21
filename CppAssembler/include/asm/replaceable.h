#pragma once

#include "defs.h"

namespace CppAsm
{
	class Replaceable {
	private:
		Offset mCbOffset;
	public:
		explicit constexpr Replaceable(Offset cbOffset) : mCbOffset(cbOffset) {}

		constexpr Offset getCbOffset() const {
			return mCbOffset;
		}
	};

	template<class T>
	class ReplaceableValue : public Replaceable {
	public:
		using Replaceable::Replaceable;

		template<class BLOCK>
		void replace(BLOCK& block, const T& value) const {
			block.writeRaw(value, getCbOffset());
		}

		template<class BLOCK>
		T read(BLOCK& block) const {
			T readValue;
			block.readRaw(readValue, getCbOffset());
			return readValue;
		}
	};

	template<class T>
	struct TypeBitsMask;

	template<class T, T MASK = TypeBitsMask<T>::value>
	class ReplaceableBits : public Replaceable {
	private:
		static_assert(sizeof(T) == 1, "ReplaceableBits not supported this type");
		uint8_t mBitOffset;
	public:
		constexpr ReplaceableBits(Offset cbOffset, uint8_t bitOffset) : Replaceable(cbOffset), mBitOffset(bitOffset) {}

		template<class BLOCK>
		void replace(BLOCK& block, const T& value) const {
			uint8_t replaceByte;
			block.readRaw(replaceByte, getCbOffset());
			replaceByte &= ~(MASK << mBitOffset);
			replaceByte |= value << mBitOffset;
			block.writeRaw(replaceByte, getCbOffset());
		}

		template<class BLOCK>
		T read(BLOCK& block) const {
			uint8_t readValue;
			block.readRaw(readValue, getCbOffset());
			return (readValue >> mBitOffset) & MASK;
		}
	};
}
