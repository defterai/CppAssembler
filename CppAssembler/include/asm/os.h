#pragma once

#include <cstdint>

#include "defs.h"
#include "os/byte_order.h"

namespace CppAsm::Os
{
	class BaseBlock {
	private:
		uint8_t* mDataCurr;
		uint8_t* mDataStart;
		Size mSize;

	protected:
		void verifyCurrent(Size size);
		void verifyOffset(Size offset, Size size);
		void* write(const void* data, Size size);
		void* writeOffset(Size offset, const void* data, Size size);
		void read(void* data, Size size);
		void readOffset(Size offset, void* data, Size size);
	public:
		BaseBlock(void* data, Size size);

		/* Get pointer to block start */
		Addr getStartPtr() const;

		/* Get pointer to next write data */
		Addr getCurrentPtr() const;

		/* Get current write offset */
		Offset getOffset() const;

		/* Get total size of this block in bytes */
		Size getTotalSize() const;

		/* Get written size in bytes */
		Size getSize() const;

		/* Get remaining size in bytes available to write */
		Size getRemainingSize() const;

		/* Reset current write position to start */
		void reset();

		/* Reset current write position to specified position
		 * in case address not in block range return false
		 */
		bool reset(Addr ptr);

		/* Move forward current write position by specified size */
		void skipBytes(Size size);

		/* Create sub block from current write position to end of parent block */
		template<class BLOCK>
		BLOCK subBlock() const {
			return BLOCK(mDataCurr, getRemainingSize());
		}

		/* Create sub block from current write position with specified size */
		template<class BLOCK>
		BLOCK subBlock(Size size) const {
			return BLOCK(mDataCurr, size);
		}

		/* Get size in bytes needed to align current write position */
		template<uint8_t ALIGN>
		Size getAlignSize() const {
			static_assert((ALIGN & (ALIGN - 1)) == 0, "MeasureBlock::getAlignSize(): align not power of two");
			Size alignRemainder = reinterpret_cast<Size>(const_cast<uint8_t*>(mDataCurr)) % ALIGN;
			return (ALIGN - alignRemainder) % ALIGN;
		}
	};

	class MeasureBlock : public BaseBlock {
	public:
		MeasureBlock(void* data, Size size) : BaseBlock(data, size) {}
		MeasureBlock(const MeasureBlock&) = delete;
		MeasureBlock(MeasureBlock&&) = default;
		MeasureBlock& operator=(const MeasureBlock&) = delete;
		MeasureBlock& operator=(MeasureBlock&&) = default;

		template<endian ORDER, class T>
		void readRaw(T& val) {
			// do nothing
		}

		template<endian ORDER, class T>
		void readRaw(T& val, Offset offset) {
			// do nothing
		}

		template<endian ORDER, class T>
		void writeRaw(const T& val) {
			// do nothing
		}

		template<endian ORDER, class T>
		void writeRaw(const T& val, Offset offset) {
			// do nothing
		}

		template<endian ORDER, class T>
		void pushRaw(const T& val) {
			skipBytes(sizeof(val));
		}
	};

	class CodeBlock : public BaseBlock {
	public:
		CodeBlock(void* data, Size size) : BaseBlock(data, size) {}
		CodeBlock(const CodeBlock&) = delete;
		CodeBlock(CodeBlock&&) = default;
		CodeBlock& operator=(const CodeBlock&) = delete;
		CodeBlock& operator=(CodeBlock&&) = default;

		template<endian ORDER, class T>
		void readRaw(T& val) {
			read(&val, sizeof(val));
			change_byte_order<T, endian::native, ORDER>(&val, 1);
		}

		template<endian ORDER, class T>
		void readRaw(T& val, Offset offset) {
			readOffset(offset, &val, sizeof(val));
			change_byte_order<T, endian::native, ORDER>(&val, 1);
		}

		template<endian ORDER, class T>
		void writeRaw(const T& val) {
			void* ptr = write(&val, sizeof(val));
			change_byte_order<T, endian::native, ORDER>(ptr, 1);
		}

		template<endian ORDER, class T>
		void writeRaw(const T& val, Offset offset) {
			void* ptr = writeOffset(offset, &val, sizeof(val));
			change_byte_order<T, endian::native, ORDER>(ptr, 1);
		}

		template<endian ORDER, class T>
		void pushRaw(const T& val) {
			void* ptr = write(&val, sizeof(val));
			change_byte_order<T, endian::native, ORDER>(ptr, 1);
			skipBytes(sizeof(val));
		}
	};
}