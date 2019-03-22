#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CppAsm;

namespace UnitTest
{
	/* Test to verify all x86 instruction variants can be compiled and generate some code */
	TEST_CLASS(X86CompileTest) 
	{
	private:
		constexpr static Size CODE_BLOCK_SIZE = 1024 * 1024;
		typedef Win32::CodeBlock testCodeBlock;
		typedef X86::i486 testArch;
	public:
		TEST_METHOD(TestNoParams) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			testArch::Aaa(block);
			testArch::Aad(block);
			testArch::Aam(block);
			testArch::Aas(block);
			testArch::Adc(block, X86::EAX, X86::EDX);
			testArch::Adc(block, X86::EAX, X86::Mem32<X86::BASE>(X86::EDX));
			testArch::Adc(block, X86::Mem32<X86::BASE>(X86::EDX), X86::EAX);
			testArch::Adc(block, X86::EAX, S8(1));
		}

		TEST_METHOD(TestXchg) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			testArch::Xchg(block, X86::EAX, X86::EDX);
			testArch::Xchg(block, X86::EDX, X86::EAX);
			testArch::Xchg(block, X86::EDX, X86::ESI);
			testArch::Xchg(block, X86::AX, X86::DX);
			testArch::Xchg(block, X86::DX, X86::AX);
			testArch::Xchg(block, X86::SI, X86::DX);
			testArch::Xchg(block, X86::AL, X86::AH);
			testArch::Xchg(block, X86::AH, X86::BH);
			testArch::Xchg(block, X86::DH, X86::CH);
			testArch::Xchg(block, X86::EAX, X86::Mem32<X86::BASE>(X86::EDX));
			testArch::Xchg(block, X86::Mem32<X86::BASE>(X86::EDX), X86::EAX);
			testArch::Xchg(block, X86::AX, X86::Mem32<X86::BASE>(X86::EDX));
			testArch::Xchg(block, X86::Mem32<X86::BASE>(X86::EDX), X86::AX);
			testArch::Xchg(block, X86::AL, X86::Mem32<X86::BASE>(X86::EDX));
			testArch::Xchg(block, X86::Mem32<X86::BASE>(X86::EDX), X86::AL);
		}

		TEST_METHOD(TestTest) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			testArch::Test(block, X86::EAX, X86::EDX);
			testArch::Test(block, X86::EDX, X86::EAX);
			testArch::Test(block, X86::EDX, X86::ESI);
			testArch::Test(block, X86::AX, X86::DX);
			testArch::Test(block, X86::DX, X86::AX);
			testArch::Test(block, X86::SI, X86::DX);
			testArch::Test(block, X86::AL, X86::AH);
			testArch::Test(block, X86::AH, X86::BH);
			testArch::Test(block, X86::DH, X86::CH);
			testArch::Test(block, X86::EDX, S32(50));
			testArch::Test(block, X86::DX, S16(50));
			testArch::Test(block, X86::DL, S8(50));
			testArch::Test(block, X86::Mem32<X86::BASE>(X86::EDX), X86::EAX);
			testArch::Test(block, X86::Mem32<X86::BASE>(X86::EDX), X86::AX);
			testArch::Test(block, X86::Mem32<X86::BASE>(X86::EDX), X86::AL);
			testArch::Test<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S32(50));
			testArch::Test<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S16(50));
			testArch::Test<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
		}

		TEST_METHOD(TestShrd) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			testArch::Shrd(block, X86::AX, X86::DX, U8(5));
			testArch::Shrd<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX, U8(5));
			testArch::Shrd<X86::CL>(block, X86::AX, X86::DX);
			testArch::Shrd<X86::WORD_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX);
			testArch::Shrd(block, X86::EAX, X86::EDX, U8(5));
			testArch::Shrd<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX, U8(5));
			testArch::Shrd<X86::CL>(block, X86::EAX, X86::EDX);
			testArch::Shrd<X86::DWORD_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::EDX);
		}

		TEST_METHOD(TestXadd) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// XADD r/m8, r8
			testArch::Xadd(block, X86::AL, X86::AH);
			testArch::Xadd(block, X86::Mem32<X86::BASE>(X86::EDX), X86::AH);
			// XADD r/m16, r16
			testArch::Xadd(block, X86::AX, X86::CX);
			testArch::Xadd(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			// XADD r/m32, r32
			testArch::Xadd(block, X86::EAX, X86::ECX);
			testArch::Xadd(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
		}

		TEST_METHOD(TestBsf) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// BSF r16, r/m16
			testArch::Bsf(block, X86::DX, X86::CX);
			testArch::Bsf(block, X86::DX, X86::Mem32<X86::BASE>(X86::EDX));
			// BSF r32, r/m32
			testArch::Bsf(block, X86::EDX, X86::ECX);
			testArch::Bsf(block, X86::EDX, X86::Mem32<X86::BASE>(X86::EDX));
		}
	};
}