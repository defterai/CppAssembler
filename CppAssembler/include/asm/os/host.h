#pragma once

#include "asm/os.h"

namespace CppAsm::Host
{

#ifdef __unix__
	#define CPP_ASM_HOST_OS LINUX
	#ifdef __amd64__
		#include "asm/os/unix64.h"
		#define CPP_ASM_HOST_ARCH_BITS 64
		using MeasureBlock = CppAsm::Unix64::MeasureBlock;
		using CodeBlock = CppAsm::Unix64::CodeBlock;
	#else
		#include "asm/os/unix32.h"
		#define CPP_ASM_HOST_ARCH_BITS 32
		using MeasureBlock = CppAsm::Unix32::MeasureBlock;
		using CodeBlock = CppAsm::Unix32::CodeBlock;
	#endif
#else
	#define CPP_ASM_HOST_OS WINDOWS
	#ifdef WIN32
		#include "asm/os/win32.h"
		#define CPP_ASM_HOST_ARCH_BITS 32
		using MeasureBlock = CppAsm::Win32::MeasureBlock;
		using CodeBlock = CppAsm::Win32::CodeBlock;
	#else // WIN32
		#define CPP_ASM_HOST_ARCH_BITS 64
	#endif
#endif

}
