#pragma once

#include <cstdint>
#include <cstddef>

namespace CppAsm
{
	typedef uint8_t* MutableAddr;
	typedef const uint8_t* Addr;
	typedef std::size_t Offset;
	typedef std::size_t Size;

	template<class T>
	class Imm final {
	private:
		T mValue;
	public:
		explicit constexpr Imm(T val) : mValue(val) {}
		constexpr operator T() const {
			return mValue;
		}

		typedef T type;
	};

	typedef Imm<int8_t> S8;
	typedef Imm<int16_t> S16;
	typedef Imm<int32_t> S32;
	typedef Imm<int64_t> S64;
	typedef Imm<uint8_t> U8;
	typedef Imm<uint16_t> U16;
	typedef Imm<uint32_t> U32;
	typedef Imm<uint64_t> U64;

	enum class endian {
#ifdef _WIN32
		little = 0,
		big = 1,
		native = little
#else
		little = __ORDER_LITTLE_ENDIAN__,
		big = __ORDER_BIG_ENDIAN__,
		native = __BYTE_ORDER__
#endif
	};
}
