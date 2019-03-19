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
		void operator=(const CodeBlock&) = delete;

		void clear();
		bool clearAfter(Addr ptr);
		Addr getStartPtr() const;
		Addr getCurrentPtr() const;
		Offset getOffset() const;
		void skipBytes(Size size);

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