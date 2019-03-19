#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CppAsm;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	private:
		constexpr static Size CODE_BLOCK_SIZE = 1024 * 1024;
	public:
		TEST_METHOD(TestSumOfParams)
		{
			Win32::CodeBlock block(CODE_BLOCK_SIZE);
			typedef X86::i386 codeGen;
			Addr functionAddr = nullptr;
			{
				Win32::FunctionGen<Win32::CC_CDECL> funcGen(block, 8, 0);
				functionAddr = funcGen.getAddress();

				codeGen::Xor(block, X86::EAX, X86::EAX);
				codeGen::Mov(block, X86::ECX, funcGen.getParameter(0));
				codeGen::Mov(block, X86::EDX, funcGen.getParameter(4));
				codeGen::Lea(block, X86::EAX, X86::Mem32<X86::BASE_INDEX>(X86::ECX, X86::EDX, X86::SCALE_2));
				Addr loopAddr = block.getCurrentPtr();
				codeGen::Add(block, X86::EAX, X86::EDX);
				codeGen::Dec(block, X86::ECX);
				codeGen::Jnz<X86::SHORT>(block).bind(block, loopAddr);
			}
			int32_t ret = block.invokeAddr<Win32::CC_CDECL, int32_t>(functionAddr, 5, 6);
			Assert::AreEqual(5 + 6 * 2 + 5 * 6, ret);
		}

		TEST_METHOD(TestSumOfBytes) {
			Win32::CodeBlock block(CODE_BLOCK_SIZE);
			typedef X86::i386 codeGen;
			Addr functionAddr = nullptr;
			{
				Win32::FunctionGen<Win32::CC_CDECL> funcGen(block, 8, 0);
				functionAddr = funcGen.getAddress();
				codeGen::Xor(block, X86::ECX, X86::ECX);
				codeGen::Mov(block, X86::EDX, funcGen.getParameter(4));
				codeGen::Test(block, X86::EDX, funcGen.getParameter(4));
				X86::FwdLabel<X86::SHORT> zeroItemsAddr = codeGen::Jz<X86::SHORT>(block);
				codeGen::Mov(block, X86::EAX, funcGen.getParameter(0));
				Addr loopAddr = block.getCurrentPtr();
				codeGen::Add(block, X86::CL, X86::Mem32<X86::BASE>(X86::EAX));
				//codeGen.Adc(X86::CH, X86::Imm8(0));
				codeGen::Inc(block, X86::EAX);
				codeGen::Dec(block, X86::EDX);
				codeGen::Jnz<X86::LONG>(block).bind(block, loopAddr);
				zeroItemsAddr.bind(block);
				codeGen::Mov(block, X86::EAX, X86::ECX);
			}
			int8_t test_data[] = { 1, 5, 6, 8 };
			int32_t ret = block.invokeAddr<Win32::CC_CDECL, int32_t>(functionAddr, test_data, 4);
			Assert::AreEqual(1 + 5 + 6 + 8, ret);
		}

		TEST_METHOD(TestNew) {
			Win32::CodeBlock block(CODE_BLOCK_SIZE);
			typedef X86::i386 codeGen;
			codeGen::Adc(block, X86::EAX, U16(5));
		}
	};
}