#include "asm\os.h"
#include <cstring>
#include <cassert>

namespace CppAsm::Os
{
	void BaseBlock::verifyCurrent(Size size) {
		assert(mDataCurr + size <= mDataStart + mSize);
	}

	void BaseBlock::verifyOffset(Size offset, Size size) {
		assert(mDataCurr + size <= mDataStart + mSize);
	}

	void BaseBlock::write(const void* data, Size size) {
		verifyCurrent(size);
		std::memcpy(mDataCurr, data, size);
	}

	void BaseBlock::writeOffset(Size offset, const void* data, Size size) {
		verifyOffset(offset, size);
		std::memcpy(mDataStart + offset, data, size);
	}

	void BaseBlock::read(void* data, Size size) {
		verifyCurrent(size);
		std::memcpy(data, mDataCurr, size);
	}

	void BaseBlock::readOffset(Size offset, void* data, Size size) {
		verifyOffset(offset, size);
		std::memcpy(data, mDataStart + offset, size);
	}

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