#pragma once

#include "asm/os.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

namespace CppAsm::Linux
{
	enum CallConv {
		CC_CDECL,
	};
	
	template<CallConv C>
	struct CallConvProps;
	
	template<>
	struct CallConvProps<CC_CDECL> {
		template<class R, class... P>
		struct FuncBuilder {
			typedef R( *funcType)(P...);
		};
		constexpr static bool clearParams = false;
	};
	
	class CodeBlock : public Os::CodeBlock {
	private:
		template<class T>
		constexpr static T funcTypeCast(Addr addr) {
			return reinterpret_cast<T>(addr);
		}

		static void* allocateCodeBlockMem(Size size) {
			int fd = open("/dev/zero", O_RDONLY);
			void* memory = mmap(nullptr, size, PROT_READ | PROT_WRITE | PROT_EXEC, 
				MAP_PRIVATE, fd, 0);
			close(fd);
			return memory;
		}
		
		static void freeCodeBlockMem(void* address, Size size) {
			munmap(address, size);
		}
	public:
		CodeBlock(Size size): Os::CodeBlock(allocateCodeBlockMem(size), size) {}
		~CodeBlock() {
			if (getStartPtr()) {
				freeCodeBlockMem((void*)getStartPtr(), getSize());
			}
		}

		template<CallConv CONV = CC_CDECL, class R = void, class... P>
		R invokeAddr(Addr addr, P... args) {
			typedef typename CallConvProps<CONV>::template FuncBuilder<R, P...>::funcType funcType;
			return funcTypeCast<funcType>(addr)(args...);
		}

		template<CallConv CONV = CC_CDECL, class R = void, class... P>
		R invoke(P... args) {
			return invokeAddr<CONV, R>(getStartPtr(), args...);
		}

		template<class R = void, class... P>
		R invokeAddrCdecl(Addr addr, P... args) {
			return invokeAddr<CC_CDECL, R>(addr, args...);
		}

		template<class R = void, class... P>
		R invokeCdecl(P... args) {
			return invoke<CC_CDECL, R>(args...);
		}
	};
}
