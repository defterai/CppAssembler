#include "stdafx.h"
#include "CppUnitTest.h"
#include <fstream>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CppAsm;

namespace UnitTest
{
	TEST_CLASS(X86CodeGenTest) 
	{
	private:
		constexpr static Size CODE_BLOCK_SIZE = 1024 * 1024;
		typedef Win32::CodeBlock testCodeBlock;
		typedef X86::i386 testArch;

		static bool hexStringToBytes(std::vector<uint8_t>& data, const std::string& str) {
			uint8_t byte;
			data.clear();
			if ((str.length() % 2) == 0) {
				data.reserve(str.length() / 2);
				for (size_t i = 0; i < str.length() / 2; i++) {
					sscanf_s(str.data() + i * 2, "%2hhx", &byte);
					data.push_back(byte);
				}
				return true;
			}
			return false;
		}

		static std::wstring posToHexString(size_t pos) {
			std::wostringstream os;
			os << std::hex << pos;
			return os.str();
		}

		static std::vector<uint8_t> LoadExpectedData(const std::string& filename) {
			std::vector<uint8_t> expectedData;
			std::string expectedHexLine;
			std::ifstream expectedDataFile("../../UnitTest/data/" + filename);
			Assert::IsTrue(expectedDataFile.is_open(), TEXT("Failed open expected data file"));
			Assert::IsTrue((bool)std::getline(expectedDataFile, expectedHexLine), TEXT("Failed read expected data file line"));
			Assert::IsTrue(hexStringToBytes(expectedData, expectedHexLine), TEXT("Failed convert expected data file line to bytes"));
			return expectedData;
		}

		static void AssertBlockEqual(const testCodeBlock& block, const std::vector<uint8_t>& expectedData) {
			Assert::AreEqual(expectedData.size(), block.getSize(), TEXT("Bad generated code size"));
			for (size_t pos = 0; pos < expectedData.size(); pos++) {
				Assert::AreEqual(expectedData[pos], block.getStartPtr()[pos], posToHexString(pos).c_str());
			}
		}
	public:
		TEST_METHOD(TestTest) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// TEST AL, imm8
			testArch::Test<X86::BYTE_PTR>(block, S8(50));
			// TEST AX, imm16
			testArch::Test<X86::WORD_PTR>(block, S16(5000));
			// TEST EAX, imm32
			testArch::Test<X86::DWORD_PTR>(block, S32(5000000));
			// TEST r/m8, imm8
			testArch::Test(block, X86::DL, S8(50));
			testArch::Test(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// TEST r/m16, imm16
			testArch::Test(block, X86::DX, S16(5000));
			testArch::Test<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Test(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			// TEST r/m32, imm32
			testArch::Test(block, X86::EDX, S32(5000000));
			testArch::Test<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Test<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			testArch::Test(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			// TEST r/m8, r8
			testArch::Test(block, X86::DL, X86::DH);
			testArch::Test(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DH);
			// TEST r/m16, r16
			testArch::Test(block, X86::DX, X86::CX);
			testArch::Test(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			// TEST r/m32, r32
			testArch::Test(block, X86::EDX, X86::ECX);
			testArch::Test(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			AssertBlockEqual(block, LoadExpectedData("test.txt"));
		}

		TEST_METHOD(TestLea) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// LEA r32, [imm]
			testArch::Lea(block, X86::EBX, X86::Mem32<X86::OFFSET>(50));
			// LEA r32, [r32]
			testArch::Lea(block, X86::EBX, X86::Mem32<X86::BASE>(X86::EDX));
			// LEA r32, [r32+imm]
			testArch::Lea(block, X86::EBX, X86::Mem32<X86::BASE_OFFSET>(X86::EDX, 50));
			// LEA r32, [r32*S+imm]
			testArch::Lea(block, X86::EBX, X86::Mem32<X86::INDEX_OFFSET>(X86::EDX, X86::SCALE_2, 50));
			// LEA r32, [r32+r32]
			testArch::Lea(block, X86::EBX, X86::Mem32<X86::BASE_INDEX>(X86::EDX, X86::ECX));
			// LEA r32, [r32+r32+imm]
			testArch::Lea(block, X86::EBX, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::EDX, X86::ECX, 50));
			// LEA r32, [r32+r32*S+imm]
			testArch::Lea(block, X86::EBX, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::EDX, X86::ECX, X86::SCALE_2, 50));
			// LEA r32, fs:[imm]
			testArch::Lea(block, X86::EBX, X86::Mem32<X86::OFFSET>(X86::FS, 50));
			// LEA r32, fs:[r32]
			testArch::Lea(block, X86::EBX, X86::Mem32<X86::BASE>(X86::FS, X86::EDX));
			// LEA r32, fs:[r32+imm]
			testArch::Lea(block, X86::EBX, X86::Mem32<X86::BASE_OFFSET>(X86::FS, X86::EDX, 50));
			// LEA r32, fs:[r32*S+imm]
			testArch::Lea(block, X86::EBX, X86::Mem32<X86::INDEX_OFFSET>(X86::FS, X86::EDX, X86::SCALE_2, 50));
			// LEA r32, fs:[r32+r32]
			testArch::Lea(block, X86::EBX, X86::Mem32<X86::BASE_INDEX>(X86::FS, X86::EDX, X86::ECX));
			// LEA r32, fs:[r32+r32+imm]
			testArch::Lea(block, X86::EBX, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::FS, X86::EDX, X86::ECX, 50));
			// LEA r32, fs:[r32+r32*S+imm]
			testArch::Lea(block, X86::EBX, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::FS, X86::EDX, X86::ECX, X86::SCALE_2, 50));
			AssertBlockEqual(block, LoadExpectedData("lea.txt"));
		}

		TEST_METHOD(TestMov) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// MOV r/m8, r8
			testArch::Mov(block, X86::AL, X86::DL);
			testArch::Mov(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DL);
			// MOV r/m16, r16
			testArch::Mov(block, X86::AX, X86::DX);
			testArch::Mov(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX);
			// MOV r/m32, r32
			testArch::Mov(block, X86::EAX, X86::EDX);
			testArch::Mov(block, X86::Mem32<X86::BASE>(X86::EDX), X86::EDX);
			// MOV r8, r/m8
			testArch::Mov(block, X86::DL, X86::AL);
			testArch::Mov(block, X86::DL, X86::Mem32<X86::BASE>(X86::EDX));
			// MOV r16, r/m16
			testArch::Mov(block, X86::DX, X86::AX);
			testArch::Mov(block, X86::DX, X86::Mem32<X86::BASE>(X86::EDX));
			// MOV r32, r/m32
			testArch::Mov(block, X86::EDX, X86::EAX);
			testArch::Mov(block, X86::EDX, X86::Mem32<X86::BASE>(X86::EDX));
			// MOV r/m16, Sreg
			testArch::Mov(block, X86::AX, X86::DS);
			testArch::Mov(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DS);
			// MOV Sreg, r/m16
			testArch::Mov(block, X86::DS, X86::AX);
			testArch::Mov(block, X86::DS, X86::Mem32<X86::BASE>(X86::EDX));
			/// TODO: short variants
			// MOV r/m88, imm8
			testArch::Mov(block, X86::DL, S8(50));
			testArch::Mov(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// MOV r/m16, imm16
			testArch::Mov(block, X86::DX, S8(50));
			testArch::Mov(block, X86::DX, S16(5000));
			testArch::Mov<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Mov(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			// MOV r/m32, imm32
			testArch::Mov(block, X86::EDX, S8(50));
			testArch::Mov(block, X86::EDX, S16(5000));
			testArch::Mov(block, X86::EDX, S32(50000000));
			testArch::Mov<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Mov<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			testArch::Mov(block, X86::Mem32<X86::BASE>(X86::EDX), S32(50000000));
			// MOV CR0, r32
			testArch::Mov(block, X86::CR0, X86::EDX);
			// MOV CR2, r32
			testArch::Mov(block, X86::CR2, X86::EDX);
			// MOV CR3, r32
			testArch::Mov(block, X86::CR3, X86::EDX);
			// MOV CR4, r32
			testArch::Mov(block, X86::CR4, X86::EDX);
			// MOV r32, CR0
			testArch::Mov(block, X86::EDX, X86::CR0);
			// MOV r32, CR2
			testArch::Mov(block, X86::EDX, X86::CR2);
			// MOV r32, CR3
			testArch::Mov(block, X86::EDX, X86::CR3);
			// MOV r32, CR4
			testArch::Mov(block, X86::EDX, X86::CR4);
			// MOV r32, DR0-DR7
			testArch::Mov(block, X86::EDX, X86::DR0);
			testArch::Mov(block, X86::EDX, X86::DR5);
			testArch::Mov(block, X86::EDX, X86::DR7);
			// MOV DR0-DR7, r32 
			testArch::Mov(block, X86::DR0, X86::EDX);
			testArch::Mov(block, X86::DR5, X86::EDX);
			testArch::Mov(block, X86::DR7, X86::EDX);
			AssertBlockEqual(block, LoadExpectedData("mov.txt"));
		}

		TEST_METHOD(TestAdd) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// ADD AL, imm8
			testArch::Add<X86::BYTE_PTR>(block, S8(50));
			// ADD AX, imm16
			testArch::Add<X86::WORD_PTR>(block, S16(5000));
			// ADD EAX, imm32
			testArch::Add<X86::DWORD_PTR>(block, S32(5000000));
			// ADD r/m8, imm8
			testArch::Add(block, X86::DL, S8(50));
			testArch::Add(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// ADD r/m16, imm16
			testArch::Add(block, X86::DX, S16(5000));
			testArch::Add(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			// ADD r/m32, imm32
			testArch::Add(block, X86::EDX, S32(5000000));
			testArch::Add(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			// ADD r/m16, imm8
			testArch::Add(block, X86::DX, S8(50));
			testArch::Add<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// ADD r/m32, imm8
			testArch::Add(block, X86::EDX, S8(50));
			testArch::Add<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// ADD r/m8, r8
			testArch::Add(block, X86::DL, X86::CL);
			testArch::Add(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			// ADD r/m16, r16
			testArch::Add(block, X86::DX, X86::CX);
			testArch::Add(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			// ADD r/m32, r32
			testArch::Add(block, X86::EDX, X86::ECX);
			testArch::Add(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// ADD r8, r/m8
			testArch::Add(block, X86::CL, X86::Mem32<X86::BASE>(X86::EDX));
			// ADD r16, r/m16
			testArch::Add(block, X86::CX, X86::Mem32<X86::BASE>(X86::EDX));
			// ADD r32, r/m32
			testArch::Add(block, X86::ECX, X86::Mem32<X86::BASE>(X86::EDX));
			AssertBlockEqual(block, LoadExpectedData("add.txt"));
		}

		TEST_METHOD(TestAdc) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// ADC AL, imm8
			testArch::Adc<X86::BYTE_PTR>(block, S8(50));
			// ADC AX, imm16
			testArch::Adc<X86::WORD_PTR>(block, S16(5000));
			// ADC EAX, imm32
			testArch::Adc<X86::DWORD_PTR>(block, S32(5000000));
			// ADC r/m8, imm8
			testArch::Adc(block, X86::DL, S8(50));
			testArch::Adc(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// ADC r/m16, imm16
			testArch::Adc(block, X86::DX, S16(5000));
			testArch::Adc(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			// ADC r/m32, imm32
			testArch::Adc(block, X86::EDX, S32(5000000));
			testArch::Adc(block, X86::EDX, S32(5000));
			testArch::Adc(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			testArch::Adc(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000));
			// ADC r/m16, imm8
			testArch::Adc(block, X86::DX, S8(50));
			testArch::Adc<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// ADC r/m32, imm8
			testArch::Adc(block, X86::EDX, S8(50));
			testArch::Adc<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// ADC r/m8, r8
			testArch::Adc(block, X86::DL, X86::CL);
			testArch::Adc(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			// ADC r/m16, r16
			testArch::Adc(block, X86::DX, X86::CX);
			testArch::Adc(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			// ADC r/m32, r32
			testArch::Adc(block, X86::EDX, X86::ECX);
			testArch::Adc(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// ADC r8, r/m8
			testArch::Adc(block, X86::CL, X86::Mem32<X86::BASE>(X86::EDX));
			// ADC r16, r/m16
			testArch::Adc(block, X86::CX, X86::Mem32<X86::BASE>(X86::EDX));
			// ADC r32, r/m32
			testArch::Adc(block, X86::ECX, X86::Mem32<X86::BASE>(X86::EDX));
			AssertBlockEqual(block, LoadExpectedData("adc.txt"));
		}

		TEST_METHOD(TestRet) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// RET (near)
			testArch::Retn(block);
			// RET (far)
			testArch::Retf(block);
			// RET imm16 (near)
			testArch::Retn(block, U16(0));
			testArch::Retn(block, U16(1));
			testArch::Retn(block, U16(4));
			testArch::Retn(block, U16(10000));
			testArch::Retn(block, U16(65535));
			// RET imm16 (far)
			testArch::Retf(block, U16(0));
			testArch::Retf(block, U16(1));
			testArch::Retf(block, U16(4));
			testArch::Retf(block, U16(10000));
			testArch::Retf(block, U16(65535));
			AssertBlockEqual(block, LoadExpectedData("ret.txt"));
		}

		TEST_METHOD(TestIn) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// IN AL, imm8
			testArch::In<X86::BYTE_PTR>(block, U8(50));
			// IN AX, imm8
			testArch::In<X86::WORD_PTR>(block, U8(50));
			// IN EAX, imm8
			testArch::In<X86::DWORD_PTR>(block, U8(50));
			// IN AL, DX
			testArch::In<X86::BYTE_PTR, X86::DX>(block);
			// IN AX, DX
			testArch::In<X86::WORD_PTR, X86::DX>(block);
			// IN EAX, DX
			testArch::In<X86::DWORD_PTR, X86::DX>(block);
			AssertBlockEqual(block, LoadExpectedData("in.txt"));
		}

		TEST_METHOD(TestOut) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// OUT imm8, AL
			testArch::Out<X86::BYTE_PTR>(block, U8(50));
			// OUT imm8, AX
			testArch::Out<X86::WORD_PTR>(block, U8(50));
			// OUT imm8, EAX
			testArch::Out<X86::DWORD_PTR>(block, U8(50));
			// OUT DX, AL
			testArch::Out<X86::BYTE_PTR, X86::DX>(block);
			// OUT DX, AX
			testArch::Out<X86::WORD_PTR, X86::DX>(block);
			// OUT DX, EAX 
			testArch::Out<X86::DWORD_PTR, X86::DX>(block);
			AssertBlockEqual(block, LoadExpectedData("out.txt"));
		}

		TEST_METHOD(TestBt) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// BT r/m16, r16
			testArch::Bt(block, X86::AX, X86::CX);
			testArch::Bt(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			// BT r/m32, r32
			testArch::Bt(block, X86::EAX, X86::ECX);
			testArch::Bt(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// BT r/m16, imm8
			testArch::Bt(block, X86::AX, U8(50));
			testArch::Bt<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(50));
			// BT r/m32, imm8
			testArch::Bt(block, X86::EAX, U8(50));
			testArch::Bt<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(50));
			AssertBlockEqual(block, LoadExpectedData("bt.txt"));
		}

		TEST_METHOD(TestBsf) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// BSF r16, r/m16
			testArch::Bsf(block, X86::DX, X86::CX);
			testArch::Bsf(block, X86::DX, X86::Mem32<X86::BASE>(X86::EDX));
			// BSF r32, r/m32
			testArch::Bsf(block, X86::EDX, X86::ECX);
			testArch::Bsf(block, X86::EDX, X86::Mem32<X86::BASE>(X86::EDX));
			AssertBlockEqual(block, LoadExpectedData("bsf.txt"));
		}
	};
}