#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>

#include "defs.h"

namespace CppAsm::Os
{
	class MeasureBlock {
	protected:
		uint8_t* mDataCurr;
		uint8_t* mDataStart;
		Size mSize;
	public:
		MeasureBlock(void* data, Size size);
		MeasureBlock(const MeasureBlock&) = delete;
		MeasureBlock(MeasureBlock&&) = default;
		MeasureBlock& operator=(const MeasureBlock&) = delete;
		MeasureBlock& operator=(MeasureBlock&&) = default;

		void clear();
		bool clearAfter(Addr ptr);
		Addr getStartPtr() const;
		Addr getCurrentPtr() const;
		Offset getOffset() const;
		Size getTotalSize() const;
		Size getSize() const;
		void skipBytes(Size size);
		MeasureBlock subBlock(Size size) const;

		template<uint8_t ALIGN>
		Size getAlignSize() const {
			static_assert((ALIGN & (ALIGN - 1)) == 0, "getAlignBlock: align not power of two");
			Size alignRemainder = reinterpret_cast<Size>(const_cast<uint8_t*>(mDataCurr)) % ALIGN;
			return (ALIGN - alignRemainder) % ALIGN;
		}

		template<class T>
		void readRaw(T& val) {
			// do nothing
		}

		template<class T>
		void readRaw(T& val, Offset offset) {
			// do nothing
		}

		template<class T>
		void writeRaw(const T& val) {
			// do nothing
		}

		template<class T>
		void writeRaw(const T& val, Offset offset) {
			// do nothing
		}

		template<class T>
		void pushRaw(const T& val) {
			mDataCurr += sizeof(val);
		}
	};

	class CodeBlock : public MeasureBlock {
	public:
		using MeasureBlock::MeasureBlock;
		/*CodeBlock(void* data, Size size);
		CodeBlock(const CodeBlock&) = delete;
		CodeBlock(CodeBlock&&) = default;
		CodeBlock& operator=(const CodeBlock&) = delete;
		CodeBlock& operator=(CodeBlock&&) = default;*/

		CodeBlock subBlock(Size size) const;

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