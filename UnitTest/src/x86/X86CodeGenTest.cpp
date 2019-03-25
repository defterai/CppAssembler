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
			testArch::Adc(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
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