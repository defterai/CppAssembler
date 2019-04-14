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
		static constexpr Size CODE_BLOCK_SIZE = 1024 * 1024;

		typedef X86::i386 testArch;

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

			common_regs_t() {
				eax = 0;
				ebx = 0;
				ecx = 0;
				edx = 0;
				esi = 0;
				edi = 0;
				flags = 0;
				flag.reserved = true;
			}
		
			bool operator==(const common_regs_t& rhs) const {
				return eax == rhs.eax && ebx == rhs.ebx &&
					ecx == rhs.ecx && edx == rhs.edx &&
					esi == rhs.esi && edi == rhs.edi &&
					flags == rhs.flags;
			}

			void randomize() {
				srand(0);
				eax = rand();
				ebx = rand();
				ecx = rand();
				edx = rand();
				esi = rand();
				edi = rand();
			}
		};

		typedef std::array<uint8_t, 64> memory_state_t;

		void initRandomMemory(memory_state_t& mem) {
			srand(0);
			for (size_t i = 0; i < mem.size(); i++) {
				mem[i] = rand();
			}
		}

		struct TestInstructionCodeBlock : public Win32::CodeBlock {
			common_regs_t input_regs;
			common_regs_t output_regs;
			memory_state_t input_memory;
			memory_state_t output_memory;

			TestInstructionCodeBlock() : Win32::CodeBlock(CODE_BLOCK_SIZE) {
				input_regs.randomize();
			}

			void execute() {
				invokeAddr<Win32::CC_CDECL>(getStartPtr(), &input_regs, &input_memory,
					&output_regs, &output_memory);
			}
		};

		class TestInstructionFunctionGen : private Win32::FunctionGen<Win32::CC_CDECL> {
		private:
			TestInstructionCodeBlock& mBlock;

		public:
			TestInstructionFunctionGen(TestInstructionCodeBlock& block) :
				mBlock(block), Win32::FunctionGen<Win32::CC_CDECL>(block, 16, 0) 
			{
				testArch::Pushad(block);
				testArch::Mov(block, X86::EAX, getParameter(0));
				testArch::Mov(block, X86::EBX, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 4));
				testArch::Mov(block, X86::ECX, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 8));
				testArch::Mov(block, X86::EDX, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 12));
				testArch::Mov(block, X86::ESI, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 16));
				testArch::Mov(block, X86::EDI, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 20));
				testArch::Mov(block, X86::AH, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 24));
				testArch::Sahf(block);
				testArch::Mov(block, X86::EAX, getParameter(0));
				testArch::Mov(block, X86::EAX, X86::Mem32<X86::BASE>(X86::EAX));
			}

			~TestInstructionFunctionGen() {
				testArch::Push(mBlock, X86::ECX);
				testArch::Mov(mBlock, X86::ECX, getParameter(8));
				testArch::Mov(mBlock, X86::Mem32<X86::BASE>(X86::ECX), X86::EAX);
				testArch::Lahf(mBlock);
				testArch::Mov(mBlock, X86::Mem32<X86::BASE_OFFSET>(X86::ECX, 24), X86::AH);
				testArch::Pop(mBlock, X86::ECX);
				testArch::Mov(mBlock, X86::EAX, getParameter(8));
				testArch::Mov(mBlock, X86::EAX, getParameter(8));
				testArch::Mov(mBlock, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 4), X86::EBX);
				testArch::Mov(mBlock, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 8), X86::ECX);
				testArch::Mov(mBlock, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 12), X86::EDX);
				testArch::Mov(mBlock, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 16), X86::ESI);
				testArch::Mov(mBlock, X86::Mem32<X86::BASE_OFFSET>(X86::EAX, 20), X86::EDI);
				testArch::Popad(mBlock);
			}

			Win32::FunctionParam getInputMemory() const {
				return getParameter(4);
			}

			Win32::FunctionParam getOutputMemory() const {
				return getParameter(12);
			}
		};
	public:
		TEST_METHOD(TestSumOfParams)
		{
			Win32::CodeBlock block(CODE_BLOCK_SIZE);
			Addr functionAddr = nullptr;
			{
				Win32::FunctionGen<Win32::CC_CDECL> funcGen(block, 8, 0);
				functionAddr = funcGen.getAddress();

				testArch::Xor(block, X86::EAX, X86::EAX);
				testArch::Mov(block, X86::ECX, funcGen.getParameter(0));
				testArch::Mov(block, X86::EDX, funcGen.getParameter(4));
				testArch::Lea(block, X86::EAX, X86::Mem32<X86::BASE_INDEX>(X86::ECX, X86::EDX, X86::SCALE_2));
				Addr loopAddr = block.getCurrentPtr();
				testArch::Add(block, X86::EAX, X86::EDX);
				testArch::Dec(block, X86::ECX);
				testArch::Jnz<X86::SHORT>(block).bind(block, loopAddr);
			}
			int32_t ret = block.invokeAddr<Win32::CC_CDECL, int32_t>(functionAddr, 5, 6);
			Assert::AreEqual(5 + 6 * 2 + 5 * 6, ret);
		}

		TEST_METHOD(TestSumOfBytes) {
			Win32::CodeBlock block(CODE_BLOCK_SIZE);
			Addr functionAddr = nullptr;
			{
				Win32::FunctionGen<Win32::CC_CDECL> funcGen(block, 8, 0);
				functionAddr = funcGen.getAddress();
				testArch::Xor(block, X86::ECX, X86::ECX);
				testArch::Mov(block, X86::EDX, funcGen.getParameter(4));
				testArch::Test(block, X86::EDX, funcGen.getParameter(4));
				X86::FwdLabel<X86::SHORT> zeroItemsAddr = testArch::Jz<X86::SHORT>(block);
				testArch::Mov(block, X86::EAX, funcGen.getParameter(0));
				Addr loopAddr = block.getCurrentPtr();
				testArch::Add(block, X86::CL, X86::Mem32<X86::BASE>(X86::EAX));
				//testArch.Adc(X86::CH, X86::Imm8(0));
				testArch::Inc(block, X86::EAX);
				testArch::Dec(block, X86::EDX);
				testArch::Jnz<X86::LONG>(block).bind(block, loopAddr);
				zeroItemsAddr.bind(block);
				testArch::Mov(block, X86::EAX, X86::ECX);
			}
			int8_t test_data[] = { 1, 5, 6, 8 };
			int32_t ret = block.invokeAddr<Win32::CC_CDECL, int32_t>(functionAddr, test_data, 4);
			Assert::AreEqual(1 + 5 + 6 + 8, ret);
		}

		TEST_METHOD(TestCompileAddressing) {
			
		}

		TEST_METHOD(TestMovRR32) {
			TestInstructionCodeBlock block;
			{
				TestInstructionFunctionGen funcGen(block);
				testArch::Mov(block, X86::EDX, X86::EAX);
			}
			block.execute();
			Assert::AreEqual(block.input_regs.eax, block.output_regs.edx);
		}

		TEST_METHOD(TestMovRM32) {
			TestInstructionCodeBlock block;
			{
				TestInstructionFunctionGen funcGen(block);
				testArch::Xor(block, X86::EDX, X86::EDX);
				testArch::Mov(block, X86::EAX, funcGen.getInputMemory());
				testArch::Mov(block, X86::DL, X86::Mem32<X86::BASE>(X86::EAX));
			}
			block.input_memory[0] = 45;
			block.execute();
			Assert::AreEqual((uint32_t) block.input_memory[0], block.output_regs.edx);
		}

		TEST_METHOD(TestMovMR32) {
			TestInstructionCodeBlock block;
			{
				TestInstructionFunctionGen funcGen(block);
				testArch::Mov(block, X86::EAX, funcGen.getInputMemory());
				testArch::Mov(block, X86::AL, X86::Mem32<X86::BASE>(X86::EAX));
				testArch::Mov(block, X86::EDX, funcGen.getOutputMemory());
				testArch::Mov(block, X86::Mem32<X86::BASE>(X86::EDX), X86::AL);
			}
			block.input_memory[0] = 45;
			block.execute();
			Assert::AreEqual(block.input_memory[0], block.output_memory[0]);
		}

		TEST_METHOD(TestShl) {
			TestInstructionCodeBlock block;
			{
				TestInstructionFunctionGen funcGen(block);
				testArch::Shl(block, X86::EDX, U8(1));
			}
			block.execute();
			Assert::AreEqual(block.input_regs.edx << 1, block.output_regs.edx);
		}
	};
}