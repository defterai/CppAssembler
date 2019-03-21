#include "stdafx.h"
#include "CppUnitTest.h"

#include <array>
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CppAsm;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	private:
		constexpr static Size CODE_BLOCK_SIZE = 1024 * 1024;

		struct common_flags_t {
			bool cf: 1;
			bool reserved : 1;
			bool pf : 1;
			bool unused3 : 1;
			bool af : 1;
			bool unused5 : 1;
			bool zf : 1;
			bool sf : 1;
		};

		struct common_regs_t {
			uint32_t eax;
			uint32_t ebx;
			uint32_t ecx;
			uint32_t edx;
			uint32_t esi;
			uint32_t edi;
			union {
				common_flags_t flag;
				uint8_t flags;
			};
		};

		typedef std::array<uint8_t, 64> memory_state_t;

		void initRandomRegisters(common_regs_t& regs) {
			srand(0);
			regs.eax = rand();
			regs.ebx = rand();
			regs.ecx = rand();
			regs.edx = rand();
			regs.esi = rand();
			regs.edi = rand();
			regs.flags = 0;
			regs.flag.reserved = true;
		}

		void clearRegisters(common_regs_t& regs) {
			regs.eax = 0;
			regs.ebx = 0;
			regs.ecx = 0;
			regs.edx = 0;
			regs.esi = 0;
			regs.edi = 0;
			regs.flags = 0;
			regs.flag.reserved = true;
		}

		void initRandomMemory(memory_state_t& mem) {
			srand(0);
			for (size_t i = 0; i < mem.size(); i++) {
				mem[i] = rand();
			}
		}

		void commonInstructionTest(const common_regs_t& inputRegisters, common_regs_t& outputRegisters,
			const memory_state_t* inputMemory, memory_state_t* outputMemory,
			std::function<void(Win32::CodeBlock& block, Win32::FunctionParam inputMemory,
				Win32::FunctionParam outputMemory)> testOpcodeGenerator) {
			typedef X86::i386 codeGen;
			Win32::CodeBlock block(CODE_BLOCK_SIZE);
			{
				Win32::FunctionGen<Win32::CC_CDECL> funcGen(block, 16, 0);
				codeGen::Pushad(block);
				codeGen::Mov(block, X86::EAX, funcGen.getParameter(0));
				codeGen::Mov(block, X86::EBX, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 4));
				codeGen::Mov(block, X86::ECX, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 8));
				codeGen::Mov(block, X86::EDX, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 12));
				codeGen::Mov(block, X86::ESI, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 16));
				codeGen::Mov(block, X86::EDI, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 20));
				codeGen::Mov(block, X86::AH, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 24));
				codeGen::Sahf(block);
				codeGen::Mov(block, X86::EAX, funcGen.getParameter(0));
				codeGen::Mov(block, X86::EAX, X86::Mem32<X86::BASE>(X86::EAX));
				testOpcodeGenerator(block, funcGen.getParameter(4), funcGen.getParameter(12));
				codeGen::Push(block, X86::ECX);
				codeGen::Mov(block, X86::ECX, funcGen.getParameter(8));
				codeGen::Mov(block, X86::Mem32<X86::BASE>(X86::ECX), X86::EAX);
				codeGen::Lahf(block);
				codeGen::Mov(block, X86::Mem32<X86::BASE_OFFSET>(X86::ECX, 24), X86::AH);
				codeGen::Pop(block, X86::ECX);
				codeGen::Mov(block, X86::EAX, funcGen.getParameter(8));
				codeGen::Mov(block, X86::EAX, funcGen.getParameter(8));
				codeGen::Mov(block, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 4), X86::EBX);
				codeGen::Mov(block, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 8), X86::ECX);
				codeGen::Mov(block, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 12), X86::EDX);
				codeGen::Mov(block, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 16), X86::ESI);
				codeGen::Mov(block, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 20), X86::EDI);
				
				codeGen::Popad(block);
			}
			block.invokeAddr<Win32::CC_CDECL>(block.getStartPtr(), &inputRegisters, &inputMemory,
				&outputRegisters, &outputMemory);
		}

		void commonInstructionTest(const common_regs_t& inputRegisters, common_regs_t& outputRegisters,
			std::function<void(Win32::CodeBlock& block)> testOpcodeGenerator) {
			commonInstructionTest(inputRegisters, outputRegisters, nullptr, nullptr,
				[testOpcodeGenerator](CppAsm::Win32::CodeBlock& block,
					Win32::FunctionParam inputMemory, Win32::FunctionParam outputMemory) {
				testOpcodeGenerator(block);
			});
		}
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

		TEST_METHOD(TestInstruction) {
			common_regs_t input_regs, output_regs;
			initRandomRegisters(input_regs);
			clearRegisters(output_regs);
			commonInstructionTest(input_regs, output_regs, [](CppAsm::Win32::CodeBlock& block) {
				X86::i386::Mov(block, X86::EDX, X86::EAX);
			});
			Assert::AreEqual(output_regs.edx, input_regs.eax);
		}

		TEST_METHOD(TestNew) {
			Win32::CodeBlock block(CODE_BLOCK_SIZE);
			typedef X86::i386 codeGen;
			codeGen::Adc(block, X86::EAX, U16(5));
		}
	};
}