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
			// TEST AL, imm8
			testArch::Test(block, X86::AL, S8(50));
			// TEST AX, imm16
			testArch::Test(block, X86::AX, S16(5000));
			// TEST EAX, imm32
			testArch::Test(block, X86::EAX, S32(5000000));
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
		}

		TEST_METHOD(TestPush) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// PUSH r/m16
			testArch::Push(block, X86::AX);
			testArch::Push<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// PUSH r/m32
			testArch::Push(block, X86::EAX);
			testArch::Push<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// PUSH imm8
			testArch::Push(block, S8(50));
			// PUSH imm16
			testArch::Push(block, S16(5000));
			// PUSH imm32
			testArch::Push(block, S32(5000000));
			// PUSH CS
			testArch::Push(block, X86::CS);
			// PUSH SS
			testArch::Push(block, X86::SS);
			// PUSH DS
			testArch::Push(block, X86::DS);
			// PUSH ES
			testArch::Push(block, X86::ES);
			// PUSH FS
			testArch::Push(block, X86::FS);
			// PUSH GS
			testArch::Push(block, X86::GS);
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

		TEST_METHOD(TestRor) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// ROR r/m8, 1
			testArch::Ror(block, X86::DL);
			testArch::Ror<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// ROR r/m8, CL
			testArch::Ror<X86::CL>(block, X86::DL);
			testArch::Ror<X86::BYTE_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// ROR r/m8, imm8
			testArch::Ror(block, X86::DL, U8(5));
			testArch::Ror<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(5));
			// ROR r/m16, 1
			testArch::Ror(block, X86::DX);
			testArch::Ror<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// ROR r/m16, CL
			testArch::Ror<X86::CL>(block, X86::DX);
			testArch::Ror<X86::WORD_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// ROR r/m16, imm8
			testArch::Ror(block, X86::DX, U8(5));
			testArch::Ror<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(5));
			// ROR r/m32, 1
			testArch::Ror(block, X86::EDX);
			testArch::Ror<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// ROR r/m32, CL
			testArch::Ror<X86::CL>(block, X86::EDX);
			testArch::Ror<X86::DWORD_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// ROR r/m32, imm8
			testArch::Ror(block, X86::EDX, U8(5));
			testArch::Ror<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(5));
		}

		TEST_METHOD(TestSetcc) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// SETA r/m8
			testArch::Seta(block, X86::AL);
			testArch::Seta(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETAE r/m8
			testArch::Setae(block, X86::AL);
			testArch::Setae(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETB r/m8
			testArch::Setb(block, X86::AL);
			testArch::Setb(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETBE r/m8
			testArch::Setbe(block, X86::AL);
			testArch::Setbe(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETC r/m8
			testArch::Setc(block, X86::AL);
			testArch::Setc(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETE r/m8
			testArch::Sete(block, X86::AL);
			testArch::Sete(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETG r/m8
			testArch::Setg(block, X86::AL);
			testArch::Setg(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETGE r/m8
			testArch::Setge(block, X86::AL);
			testArch::Setge(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETL r/m8
			testArch::Setl(block, X86::AL);
			testArch::Setl(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETLE r/m8
			testArch::Setle(block, X86::AL);
			testArch::Setle(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETNA r/m8
			testArch::Setna(block, X86::AL);
			testArch::Setna(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETNAE r/m8
			testArch::Setnae(block, X86::AL);
			testArch::Setnae(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETNB r/m8
			testArch::Setnb(block, X86::AL);
			testArch::Setnb(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETNBE r/m8
			testArch::Setnbe(block, X86::AL);
			testArch::Setnbe(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETNC r/m8
			testArch::Setnc(block, X86::AL);
			testArch::Setnc(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETNE r/m8
			testArch::Setne(block, X86::AL);
			testArch::Setne(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETNG r/m8
			testArch::Setng(block, X86::AL);
			testArch::Setng(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETNGE r/m8
			testArch::Setnge(block, X86::AL);
			testArch::Setnge(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETNL r/m8
			testArch::Setnl(block, X86::AL);
			testArch::Setnl(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETNLE r/m8
			testArch::Setnle(block, X86::AL);
			testArch::Setnle(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETNO r/m8
			testArch::Setno(block, X86::AL);
			testArch::Setno(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETNP r/m8
			testArch::Setnp(block, X86::AL);
			testArch::Setnp(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETNS r/m8
			testArch::Setns(block, X86::AL);
			testArch::Setns(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETNZ r/m8
			testArch::Setnz(block, X86::AL);
			testArch::Setnz(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETO r/m8
			testArch::Seto(block, X86::AL);
			testArch::Seto(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETP r/m8
			testArch::Setp(block, X86::AL);
			testArch::Setp(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETPE r/m8
			testArch::Setpe(block, X86::AL);
			testArch::Setpe(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETPO r/m8
			testArch::Setpo(block, X86::AL);
			testArch::Setpo(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETS r/m8
			testArch::Sets(block, X86::AL);
			testArch::Sets(block, X86::Mem32<X86::BASE>(X86::EDX));
			// SETZ r/m8
			testArch::Setz(block, X86::AL);
			testArch::Setz(block, X86::Mem32<X86::BASE>(X86::EDX));
		}
	};
}