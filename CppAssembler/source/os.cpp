#include "asm\os.h"

namespace CppAsm::Os
{
	MeasureBlock::MeasureBlock(void* data, Size size) :
		mDataStart(static_cast<uint8_t*>(data)),
		mDataCurr(static_cast<uint8_t*>(data)), mSize(size)
	{}

	void MeasureBlock::clear() {
		mDataCurr = mDataStart;
	}
	
	bool MeasureBlock::clearAfter(Addr ptr) {
		if ((ptr >= mDataStart) && (ptr < mDataStart + mSize)) {
			mDataCurr = const_cast<uint8_t*>(ptr);
			return true;
		}
		return false;
	}
	
	Addr MeasureBlock::getStartPtr() const {
		return mDataStart;
	}
	
	Addr MeasureBlock::getCurrentPtr() const {
		return mDataCurr;
	}
	
	Offset MeasureBlock::getOffset() const {
		return mDataCurr - mDataStart;
	}

	Size MeasureBlock::getTotalSize() const {
		return mSize;
	}

	Size MeasureBlock::getSize() const {
		return mDataCurr - mDataStart;
	}

	void MeasureBlock::skipBytes(Size size) {
		mDataCurr += size;
	}

	MeasureBlock MeasureBlock::subBlock(Size size) const {
		return MeasureBlock(mDataCurr, size);
	}

	CodeBlock CodeBlock::subBlock(Size size) const {
		return CodeBlock(mDataCurr, size);
	}
}