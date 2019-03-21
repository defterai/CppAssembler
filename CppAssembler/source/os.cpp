#include "asm\os.h"

namespace CppAsm::Os
{
	BaseBlock::BaseBlock(void* data, Size size) :
		mDataStart(static_cast<uint8_t*>(data)),
		mDataCurr(static_cast<uint8_t*>(data)), mSize(size)
	{}
	
	Addr BaseBlock::getStartPtr() const {
		return mDataStart;
	}
	
	Addr BaseBlock::getCurrentPtr() const {
		return mDataCurr;
	}
	
	Offset BaseBlock::getOffset() const {
		return mDataCurr - mDataStart;
	}

	Size BaseBlock::getTotalSize() const {
		return mSize;
	}

	Size BaseBlock::getSize() const {
		return mDataCurr - mDataStart;
	}

	Size BaseBlock::getRemainingSize() const {
		return getTotalSize() - getSize();
	}

	void BaseBlock::reset() {
		mDataCurr = mDataStart;
	}

	bool BaseBlock::reset(Addr ptr) {
		if ((ptr >= mDataStart) && (ptr < mDataStart + mSize)) {
			mDataCurr = const_cast<uint8_t*>(ptr);
			return true;
		}
		return false;
	}

	void BaseBlock::skipBytes(Size size) {
		mDataCurr += size;
	}
}