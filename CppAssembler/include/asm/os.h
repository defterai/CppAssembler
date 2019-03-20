#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>

#include "defs.h"

namespace CppAsm::Os
{
	class CodeBlock {
	private:
		uint8_t* mDataCurr;
		uint8_t* mDataStart;
		Size mSize;

	public:
		CodeBlock(void* data, Size size);
		CodeBlock(const CodeBlock&) = delete;
		CodeBlock(CodeBlock&&) = default;
		CodeBlock& operator=(const CodeBlock&) = delete;
		CodeBlock& operator=(CodeBlock&&) = default;

		void clear();
		bool clearAfter(Addr ptr);
		Addr getStartPtr() const;
		Addr getCurrentPtr() const;
		Offset getOffset() const;
		Size getSize() const;
		void skipBytes(Size size);
		CodeBlock subBlock(Size size);

		template<uint8_t ALIGN>
		Size getAlignSize() const {
			static_assert((ALIGN & (ALIGN - 1)) == 0, "getAlignBlock: align not power of two");
			Size alignRemainder = reinterpret_cast<Size>(const_cast<uint8_t*>(mDataCurr)) % ALIGN;
			return (ALIGN - alignRemainder) % ALIGN;
		}

		template<class T>
		void readRaw(T& val) {
			memcpy(&val, mDataCurr, sizeof(val));
		}

		template<class T>
		void readRaw(T& val, Offset offset) {
			memcpy(&val, mDataStart + offset, sizeof(val));
		}

		template<class T>
		void writeRaw(const T& val) {
			memcpy(mDataCurr, &val, sizeof(val));
		}

		template<class T>
		void writeRaw(const T& val, Offset offset) {
			memcpy(mDataStart + offset, &val, sizeof(val));
		}

		template<class T>
		void pushRaw(const T& val) {
			memcpy(mDataCurr, &val, sizeof(val));
			mDataCurr += sizeof(val);
		}
	};
}