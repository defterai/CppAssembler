#pragma once

#include <cstdint>
#include <cstddef>

#include "replaceable.h"

namespace CppAsm::Arch
{
	template<class... Feature>
	class CodeGen : public Feature... {
	private:
		CodeGen() = delete;
		CodeGen(const CodeGen&) = delete;
		void operator=(const CodeGen&) = delete;
	};
}
