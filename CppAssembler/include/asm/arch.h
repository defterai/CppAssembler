#pragma once

namespace CppAsm::Arch
{
	template<class... Archs>
	class CustomArch : public Archs... {
	private:
		CustomArch() = delete;
		CustomArch(const CustomArch&) = delete;
		void operator=(const CustomArch&) = delete;
	};
}
