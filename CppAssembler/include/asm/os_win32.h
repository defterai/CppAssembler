#pragma once

#include "arch.h"
#include <Windows.h>

namespace CppAsm::Win32
{
	enum CallConv {
		CC_CDECL,
		CC_STDCALL,
		CC_FASTCALL,
		CC_PASCAL
	};

	template<CallConv C>
	struct CallConvProps;

	template<>
	struct CallConvProps<CC_CDECL> {
		template<class R, class... P>
		struct FuncBuilder {
			typedef R(__cdecl *funcType)(P...);
		};
		constexpr static bool clearParams = false;
	};

	template<>
	struct CallConvProps<CC_STDCALL> {
		template<class R, class... P>
		struct FuncBuilder {
			typedef R(__stdcall *funcType)(P...);
		};
		constexpr static bool clearParams = true;
	};

	template<>
	struct CallConvProps<CC_FASTCALL> {
		template<class R, class... P>
		struct FuncBuilder {
			typedef R(__fastcall *funcType)(P...);
		};
		constexpr static bool clearParams = true;
	};

	template<>
	struct CallConvProps<CC_PASCAL> {
		template<class R, class... P>
		struct FuncBuilder {
			typedef R(pascal *funcType)(P...);
		};
		constexpr static bool clearParams = true;
	};

	class CodeBlock : public Os::CodeBlock {
	private:
		template<class T>
		constexpr static T funcTypeCast(Addr addr) {
			return reinterpret_cast<T>(addr);
		}

		static LPVOID allocateCodeBlockMem(Size size) {
			return VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		}
		static void freeCodeBlockMem(LPVOID address) {
			VirtualFree(address, 0, MEM_RELEASE);
		}
	public:
		CodeBlock(Size size): Os::CodeBlock(allocateCodeBlockMem(size), size) {}
		~CodeBlock() {
			if (getStartPtr()) {
				freeCodeBlockMem((LPVOID)getStartPtr());
			}
		}

		template<CallConv CONV = CC_CDECL, class R = void, class... P>
		R invokeAddr(Addr addr, P... args) {
			typedef CallConvProps<CONV>::FuncBuilder<R, P...>::funcType funcType;
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

		template<class R = void, class... P>
		R invokeAddrStdcall(Addr addr, P... args) {
			return invokeAddr<CC_STDCALL, R>(addr, args...);
		}

		template<class R = void, class... P>
		R invokeStdcall(P... args) {
			return invoke<CC_STDCALL, R>(args...);
		}

		template<class R = void, class... P>
		R invokeAddrFastcall(Addr addr, P... args) {
			return invokeAddr<CC_FASTCALL, R>(addr, args...);
		}

		template<class R = void, class... P>
		R invokeFastcall(P... args) {
			return invoke<CC_FASTCALL, R>(args...);
		}

		template<class R = void, class... P>
		R invokeAddrPascal(Addr addr, P... args) {
			return invokeAddr<CC_PASCAL, R>(addr, args...);
		}

		template<class R = void, class... P>
		R invokePascal(P... args) {
			return invoke<CC_PASCAL, R>(args...);
		}
	};

	typedef X86::Mem32<X86::BASE_OFFSET> FunctionParam;
	typedef X86::Mem32<X86::BASE_OFFSET> FunctionVar;

	template<CallConv CONV = CC_CDECL, X86::Reg32 BASE_REG = X86::EBP>
	class FunctionGen: public X86::i386 {
	private:
		Os::CodeBlock& mBlock;
		uint32_t mLocalVarsSize;
		uint16_t mParamsSize;

	protected:
		void beginFunction(Os::CodeBlock& block, uint32_t localVarsSize) {
			Push(block, BASE_REG);
			Mov(block, BASE_REG, X86::ESP);
			if (localVarsSize) {
				Add(block, X86::ESP, U32(localVarsSize));
			}
		}

		void endFunction(Os::CodeBlock& block) {
			Pop(block, BASE_REG);
			if (CallConvProps<CONV>::clearParams && mParamsSize) {
				Ret(block, U16(mParamsSize));
			} else {
				Ret(block);
			}
		}
	public:
		FunctionGen(Os::CodeBlock& block, uint16_t paramsSize = 0, uint32_t localVarsSize = 0) : 
			mBlock(block), mLocalVarsSize(localVarsSize), mParamsSize(paramsSize)
		{
			beginFunction(block, localVarsSize);
		}
		FunctionGen(const FunctionGen&) = delete;
		void operator=(const FunctionGen&) = delete;

		~FunctionGen() {
			endFunction(mBlock);
		}

		Addr getAddress() const {
			return mBlock.getStartPtr();
		}

		FunctionVar getVariable(uint32_t offset) const {
			assert(offset < mLocalVarsSize);
			return FunctionVar(BASE_REG, -4 - offset);
		}

		FunctionParam getParameter(uint32_t offset) const {
			assert(offset < mParamsSize);
			return FunctionParam(BASE_REG, 8 + offset);
		}
	};
}