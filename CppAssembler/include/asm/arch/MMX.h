#pragma once


#include <cstdint>

#include "x86_base.h"

namespace CppAsm::X86
{
	enum RegMMX : uint8_t {
		MM0 = 0b000,
		MM1 = 0b001,
		MM2 = 0b010,
		MM3 = 0b011,
		MM4 = 0b100,
		MM5 = 0b101,
		MM6 = 0b110,
		MM7 = 0b111
	};

	class MMX {
	protected:

	public:

	};
}