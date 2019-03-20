#include "asm\arch.h"

namespace CppAsm::Os
{
	CodeBlock::CodeBlock(void* data, Size size) :
		mDataStart(static_cast<uint8_t*>(data)),
		mDataCurr(static_cast<uint8_t*>(data)), mSize(size)
	{}

	void CodeBlock::clear() {
		mDataCurr = mDataStart;
	}
	
	bool CodeBlock::clearAfter(Addr ptr) {
		if ((ptr >= mDataStart) && (ptr < mDataStart + mSize)) {
			mDataCurr = const_cast<uint8_t*>(ptr);
			return true;
		}
		return false;
	}
	
	Addr CodeBlock::getStartPtr() const {
		return mDataStart;
	}
	
	Addr CodeBlock::getCurrentPtr() const {
		return mDataCurr;
	}
	
	Offset CodeBlock::getOffset() const {
		return mDataCurr - mDataStart;
	}

	Size CodeBlock::getSize() const {
		return mSize;
	}

	void CodeBlock::skipBytes(Size size) {
		mDataCurr += size;
	}

	CodeBlock CodeBlock::subBlock(Size size) {
		return CodeBlock(mDataCurr, size);
	}
}