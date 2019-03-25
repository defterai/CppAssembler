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
		typedef X86::i386 testArch;
		typedef X86::i486 testArch4;
		typedef X86::i586 testArch5;
		typedef X86::i686 testArch6;
		typedef X86::i387 testArchFpu;
	public:
		TEST_METHOD(TestNoParams) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			testArch::Aaa(block);
			testArch::Aad(block);
			testArch::Aas(block);
			testArch::Aam(block);
			testArch::Cbw(block);
			testArch::Cwde(block);
			testArch::Clc(block);
			testArch::Cld(block);
			testArch::Cli(block);
			testArch::Clts(block);
			testArch::Cmc(block);
			testArch::Cmpsb(block);
			testArch::Cmpsw(block);
			testArch::Cmpsd(block);
			testArch::Cwd(block);
			testArch::Cdq(block);
			testArch::Daa(block);
			testArch::Das(block);
			testArch::Hlt(block);
			testArch::Insb(block);
			testArch::Insw(block);
			testArch::Insd(block);
			testArch::Int3(block);
			testArch::IntO(block);
			testArch::Iret(block);
			testArch::Lahf(block);
			testArch::Leave(block);
			testArch::Lodsb(block);
			testArch::Lodsw(block);
			testArch::Lodsd(block);
			testArch::Movsb(block);
			testArch::Movsw(block);
			testArch::Movsd(block);
			testArch::Nop(block);
			testArch::Outsb(block);
			testArch::Outsw(block);
			testArch::Outsd(block);
			testArch::Pusha(block);
			testArch::Pushad(block);
			testArch::Pushf(block);
			testArch::Pushfd(block);
			testArch::Popa(block);
			testArch::Popad(block);
			testArch::Ret(block);
			testArch::Retf(block);
			testArch::Retn(block);
			testArch::Sahf(block);
			testArch::Scasb(block);
			testArch::Scasw(block);
			testArch::Scasd(block);
			testArch::Stc(block);
			testArch::Std(block);
			testArch::Sti(block);
			testArch::Stosb(block);
			testArch::Stosw(block);
			testArch::Stosd(block);
			testArch::Xlat(block);
			testArch4::Cpuid(block);
			testArch4::Invd(block);
			testArch4::Wbinvd(block);
			testArch5::Rsm(block);
			testArch5::Rdmsr(block);
			testArch5::Wrmsr(block);
			testArch5::Rdtsc(block);
			testArch6::Rdpmc(block);
			testArch6::Ud2(block);
		}

		TEST_METHOD(TestWithRepPrefix) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			testArch::Cmpsb<X86::REPE>(block);
			testArch::Cmpsw<X86::REPE>(block);
			testArch::Cmpsd<X86::REPE>(block);
			testArch::Insb<X86::REPE>(block);
			testArch::Insw<X86::REPE>(block);
			testArch::Insd<X86::REPE>(block);
			testArch::Lodsb<X86::REPE>(block);
			testArch::Lodsw<X86::REPE>(block);
			testArch::Lodsd<X86::REPE>(block);
			testArch::Movsb<X86::REPE>(block);
			testArch::Movsw<X86::REPE>(block);
			testArch::Movsd<X86::REPE>(block);
			testArch::Outsb<X86::REPE>(block);
			testArch::Outsw<X86::REPE>(block);
			testArch::Outsd<X86::REPE>(block);
			testArch::Scasb<X86::REPE>(block);
			testArch::Scasw<X86::REPE>(block);
			testArch::Scasd<X86::REPE>(block);
			testArch::Stosb<X86::REPE>(block);
			testArch::Stosw<X86::REPE>(block);
			testArch::Stosd<X86::REPE>(block);
		}

		TEST_METHOD(TestWithLockPrefix) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// ADD
			testArch::Add<X86::BYTE_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Add<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			testArch::Add<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			testArch::Add<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Add<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Add<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			testArch::Add<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			testArch::Add<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// ADC
			testArch::Adc<X86::BYTE_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Adc<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			testArch::Adc<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			testArch::Adc<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Adc<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Adc<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			testArch::Adc<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			testArch::Adc<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// AND
			testArch::And<X86::BYTE_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::And<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			testArch::And<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			testArch::And<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::And<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::And<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			testArch::And<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			testArch::And<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// BTC
			testArch::Btc<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX);
			testArch::Btc<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::EDX);
			testArch::Btc<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(50));
			testArch::Btc<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(50));
			// BTR
			testArch::Btr<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX);
			testArch::Btr<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::EDX);
			testArch::Btr<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(50));
			testArch::Btr<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(50));
			// BTS
			testArch::Bts<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX);
			testArch::Bts<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::EDX);
			testArch::Bts<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(50));
			testArch::Bts<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(50));
			// CMPXCHG
			testArch4::CmpXchg<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			testArch4::CmpXchg<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			testArch4::CmpXchg<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// CMPXCH8B
			// CMPXCHG16B
			// DEC
			testArch::Dec<X86::BYTE_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX));
			testArch::Dec<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX));
			testArch::Dec<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// INC
			testArch::Inc<X86::BYTE_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX));
			testArch::Inc<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX));
			testArch::Inc<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// NEG
			testArch::Neg<X86::BYTE_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX));
			testArch::Neg<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX));
			testArch::Neg<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// NOT
			testArch::Not<X86::BYTE_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX));
			testArch::Not<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX));
			testArch::Not<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// OR
			testArch::Or<X86::BYTE_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Or<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			testArch::Or<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			testArch::Or<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Or<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Or<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			testArch::Or<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			testArch::Or<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// SBB
			testArch::Sbb<X86::BYTE_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Sbb<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			testArch::Sbb<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			testArch::Sbb<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Sbb<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Sbb<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			testArch::Sbb<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			testArch::Sbb<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// SUB
			testArch::Sub<X86::BYTE_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Sub<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			testArch::Sub<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			testArch::Sub<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Sub<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Sub<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			testArch::Sub<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			testArch::Sub<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// XOR
			testArch::Xor<X86::BYTE_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Xor<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			testArch::Xor<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			testArch::Xor<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Xor<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Xor<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			testArch::Xor<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			testArch::Xor<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// XADD
			testArch4::Xadd<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			testArch4::Xadd<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			testArch4::Xadd<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// XCHG (not required)
		}

		TEST_METHOD(TestNeg) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// NEG r/m8
			testArch::Neg(block, X86::DL);
			testArch::Neg<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// NEG r/m16
			testArch::Neg(block, X86::DX);
			testArch::Neg<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// NEG r/m32
			testArch::Neg(block, X86::EDX);
			testArch::Neg<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestNot) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// NOT r/m8
			testArch::Not(block, X86::DL);
			testArch::Not<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// NOT r/m16
			testArch::Not(block, X86::DX);
			testArch::Not<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// NOT r/m32
			testArch::Not(block, X86::EDX);
			testArch::Not<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestInc) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// INC r/m8
			testArch::Inc(block, X86::DL);
			testArch::Inc<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// INC r/m16
			testArch::Inc(block, X86::DX);
			testArch::Inc<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// INC r/m32
			testArch::Inc(block, X86::EDX);
			testArch::Inc<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestDec) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// DEC r/m8
			testArch::Dec(block, X86::DL);
			testArch::Dec<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// DEC r/m16
			testArch::Dec(block, X86::DX);
			testArch::Dec<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// DEC r/m32
			testArch::Dec(block, X86::EDX);
			testArch::Dec<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestMul) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// MUL r/m8
			testArch::Mul(block, X86::DL);
			testArch::Mul<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// MUL r/m16
			testArch::Mul(block, X86::DX);
			testArch::Mul<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// MUL r/m32
			testArch::Mul(block, X86::EDX);
			testArch::Mul<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestDiv) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// DIV r/m8
			testArch::Div(block, X86::DL);
			testArch::Div<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// DIV r/m16
			testArch::Div(block, X86::DX);
			testArch::Div<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// DIV r/m32
			testArch::Div(block, X86::EDX);
			testArch::Div<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestIdiv) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// IDIV r/m8
			testArch::Idiv(block, X86::DL);
			testArch::Idiv<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// IDIV r/m16
			testArch::Idiv(block, X86::DX);
			testArch::Idiv<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// IDIV r/m32
			testArch::Idiv(block, X86::EDX);
			testArch::Idiv<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestXchg) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// XCHG AX, r16
			// XCHG r16, AX
			testArch::Xchg(block, X86::DX);
			// XCHG EAX, r32
			// XCHG r32, EAX
			testArch::Xchg(block, X86::EDX);
			// XCHG r/m8, r8
			testArch::Xchg(block, X86::DH, X86::CH);
			testArch::Xchg(block, X86::Mem32<X86::BASE>(X86::EDX), X86::AL);
			// XCHG r8, r/m8
			testArch::Xchg(block, X86::AX, X86::Mem32<X86::BASE>(X86::EDX));
			// XCHG r/m16, r16
			testArch::Xchg(block, X86::SI, X86::DX);
			testArch::Xchg(block, X86::Mem32<X86::BASE>(X86::EDX), X86::AX);
			// XCHG r16, r/m16
			testArch::Xchg(block, X86::AX, X86::Mem32<X86::BASE>(X86::EDX));
			// XCHG r/m32, r32
			testArch::Xchg(block, X86::EDX, X86::ESI);
			testArch::Xchg(block, X86::Mem32<X86::BASE>(X86::EDX), X86::EAX);
			// XCHG r32, r/m32
			testArch::Xchg(block, X86::EAX, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestCmp) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// CMP AL, imm8
			testArch::Cmp<X86::BYTE_PTR>(block, S8(50));
			// CMP AX, imm16
			testArch::Cmp<X86::WORD_PTR>(block, S16(5000));
			// CMP EAX, imm32
			testArch::Cmp<X86::DWORD_PTR>(block, S32(5000000));
			// CMP AL, imm8
			testArch::Cmp(block, X86::AL, S8(50));
			// CMP AX, imm16
			testArch::Cmp(block, X86::AX, S16(5000));
			// CMP EAX, imm32
			testArch::Cmp(block, X86::EAX, S32(5000000));
			// CMP r/m8, imm8
			testArch::Cmp(block, X86::DL, S8(50));
			testArch::Cmp(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// CMP r/m16, imm16
			testArch::Cmp(block, X86::DX, S16(5000));
			testArch::Cmp<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Cmp(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			// CMP r/m32, imm32
			testArch::Cmp(block, X86::EDX, S32(5000000));
			testArch::Cmp<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			testArch::Cmp<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			testArch::Cmp(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			// CMP r/m8, r8
			testArch::Cmp(block, X86::DL, X86::DH);
			testArch::Cmp(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DH);
			// CMP r/m16, r16
			testArch::Cmp(block, X86::DX, X86::CX);
			testArch::Cmp(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			// CMP r/m32, r32
			testArch::Cmp(block, X86::EDX, X86::ECX);
			testArch::Cmp(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// CMP r8, r/m8
			testArch::Cmp(block, X86::DH, X86::Mem32<X86::BASE>(X86::EDX));
			// CMP r16,r/m16
			testArch::Cmp(block, X86::CX, X86::Mem32<X86::BASE>(X86::EDX));
			// CMP r32,r/m32
			testArch::Cmp(block, X86::ECX, X86::Mem32<X86::BASE>(X86::EDX));
		}

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
		}

		TEST_METHOD(TestShrd) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// SHRD r/m16, r16, imm8
			testArch::Shrd(block, X86::AX, X86::DX, U8(5));
			testArch::Shrd<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX, U8(5));
			// SHRD r/m16, r16, CL
			testArch::Shrd<X86::CL>(block, X86::AX, X86::DX);
			testArch::Shrd<X86::WORD_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX);
			// SHRD r/m32, r32, imm8
			testArch::Shrd(block, X86::EAX, X86::EDX, U8(5));
			testArch::Shrd<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX, U8(5));
			// SHRD r/m32, r32, CL
			testArch::Shrd<X86::CL>(block, X86::EAX, X86::EDX);
			testArch::Shrd<X86::DWORD_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::EDX);
		}

		TEST_METHOD(TestShld) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// SHLD r/m16, r16, imm8
			testArch::Shld(block, X86::AX, X86::DX, U8(5));
			testArch::Shld<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX, U8(5));
			// SHLD r/m16, r16, CL
			testArch::Shld<X86::CL>(block, X86::AX, X86::DX);
			testArch::Shld<X86::WORD_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX);
			// SHLD r/m32, r32, imm8
			testArch::Shld(block, X86::EAX, X86::EDX, U8(5));
			testArch::Shld<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX, U8(5));
			// SHLD r/m32, r32, CL
			testArch::Shld<X86::CL>(block, X86::EAX, X86::EDX);
			testArch::Shld<X86::DWORD_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::EDX);
		}

		TEST_METHOD(TestLea) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// LEA r32, [imm]
			testArch::Lea(block, X86::EDX, X86::Mem32<X86::OFFSET>(50));
			// LEA r32, [r32]
			testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE>(X86::EDX));
			// LEA r32, [r32+imm]
			testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_OFFSET>(X86::EDX, 50));
			// LEA r32, [r32*S+imm]
			testArch::Lea(block, X86::EDX, X86::Mem32<X86::INDEX_OFFSET>(X86::EDX, X86::SCALE_2, 50));
			// LEA r32, [r32+r32]
			testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_INDEX>(X86::EDX, X86::ECX));
			// LEA r32, [r32+r32+imm]
			testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::EDX, X86::ECX, 50));
			// LEA r32, [r32+r32*S+imm]
			testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::EDX, X86::ECX, X86::SCALE_2, 50));
			// LEA r32, fs:[imm]
			testArch::Lea(block, X86::EDX, X86::Mem32<X86::OFFSET>(X86::FS, 50));
			// LEA r32, fs:[r32]
			testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE>(X86::FS, X86::EDX));
			// LEA r32, fs:[r32+imm]
			testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_OFFSET>(X86::FS, X86::EDX, 50));
			// LEA r32, fs:[r32*S+imm]
			testArch::Lea(block, X86::EDX, X86::Mem32<X86::INDEX_OFFSET>(X86::FS, X86::EDX, X86::SCALE_2, 50));
			// LEA r32, fs:[r32+r32]
			testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_INDEX>(X86::FS, X86::EDX, X86::ECX));
			// LEA r32, fs:[r32+r32+imm]
			testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::FS, X86::EDX, X86::ECX, 50));
			// LEA r32, fs:[r32+r32*S+imm]
			testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::FS, X86::EDX, X86::ECX, X86::SCALE_2, 50));
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
		}

		TEST_METHOD(TestSub) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// SUB AL, imm8
			testArch::Sub<X86::BYTE_PTR>(block, S8(50));
			// SUB AX, imm16
			testArch::Sub<X86::WORD_PTR>(block, S16(5000));
			// SUB EAX, imm32
			testArch::Sub<X86::DWORD_PTR>(block, S32(5000000));
			// SUB r/m8, imm8
			testArch::Sub(block, X86::DL, S8(50));
			testArch::Sub(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// SUB r/m16, imm16
			testArch::Sub(block, X86::DX, S16(5000));
			testArch::Sub(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			// SUB r/m32, imm32
			testArch::Sub(block, X86::EDX, S32(5000000));
			testArch::Sub(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			// SUB r/m16, imm8
			testArch::Sub(block, X86::DX, S8(50));
			testArch::Sub<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// SUB r/m32, imm8
			testArch::Sub(block, X86::EDX, S8(50));
			testArch::Sub<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// SUB r/m8, r8
			testArch::Sub(block, X86::DL, X86::CL);
			testArch::Sub(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			// SUB r/m16, r16
			testArch::Sub(block, X86::DX, X86::CX);
			testArch::Sub(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			// SUB r/m32, r32
			testArch::Sub(block, X86::EDX, X86::ECX);
			testArch::Sub(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// SUB r8, r/m8
			testArch::Sub(block, X86::CL, X86::Mem32<X86::BASE>(X86::EDX));
			// SUB r16, r/m16
			testArch::Sub(block, X86::CX, X86::Mem32<X86::BASE>(X86::EDX));
			// SUB r32, r/m32
			testArch::Sub(block, X86::ECX, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestSbb) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// SBB AL, imm8
			testArch::Sbb<X86::BYTE_PTR>(block, S8(50));
			// SBB AX, imm16
			testArch::Sbb<X86::WORD_PTR>(block, S16(5000));
			// SBB EAX, imm32
			testArch::Sbb<X86::DWORD_PTR>(block, S32(5000000));
			// SBB r/m8, imm8
			testArch::Sbb(block, X86::DL, S8(50));
			testArch::Sbb(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// SBB r/m16, imm16
			testArch::Sbb(block, X86::DX, S16(5000));
			testArch::Sbb(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			// SBB r/m32, imm32
			testArch::Sbb(block, X86::EDX, S32(5000000));
			testArch::Sbb(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			// SBB r/m16, imm8
			testArch::Sbb(block, X86::DX, S8(50));
			testArch::Sbb<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// SBB r/m32, imm8
			testArch::Sbb(block, X86::EDX, S8(50));
			testArch::Sbb<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// SBB r/m8, r8
			testArch::Sbb(block, X86::DL, X86::CL);
			testArch::Sbb(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			// SBB r/m16, r16
			testArch::Sbb(block, X86::DX, X86::CX);
			testArch::Sbb(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			// SBB r/m32, r32
			testArch::Sbb(block, X86::EDX, X86::ECX);
			testArch::Sbb(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// SBB r8, r/m8
			testArch::Sbb(block, X86::CL, X86::Mem32<X86::BASE>(X86::EDX));
			// SBB r16, r/m16
			testArch::Sbb(block, X86::CX, X86::Mem32<X86::BASE>(X86::EDX));
			// SBB r32, r/m32
			testArch::Sbb(block, X86::ECX, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestAnd) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// AND AL, imm8
			testArch::And<X86::BYTE_PTR>(block, S8(50));
			// AND AX, imm16
			testArch::And<X86::WORD_PTR>(block, S16(5000));
			// AND EAX, imm32
			testArch::And<X86::DWORD_PTR>(block, S32(5000000));
			// AND AL, imm8
			testArch::And(block, X86::AL, S8(50));
			// AND AX, imm16
			testArch::And(block, X86::AX, S16(5000));
			// AND EAX, imm32
			testArch::And(block, X86::EAX, S32(5000000));
			// AND r/m8, imm8
			testArch::And(block, X86::DL, S8(50));
			testArch::And(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// AND r/m16, imm16
			testArch::And(block, X86::DX, S16(5000));
			testArch::And(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			// AND r/m32, imm32
			testArch::And(block, X86::EDX, S32(5000000));
			testArch::And(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			// AND r/m16, imm8
			testArch::And(block, X86::DX, S8(50));
			testArch::And<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// AND r/m32, imm8
			testArch::And(block, X86::EDX, S8(50));
			testArch::And<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// AND r/m8, r8
			testArch::And(block, X86::DL, X86::CL);
			testArch::And(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			// AND r/m16, r16
			testArch::And(block, X86::DX, X86::CX);
			testArch::And(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			// AND r/m32, r32
			testArch::And(block, X86::EDX, X86::ECX);
			testArch::And(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// AND r8, r/m8
			testArch::And(block, X86::CL, X86::Mem32<X86::BASE>(X86::EDX));
			// AND r16, r/m16
			testArch::And(block, X86::CX, X86::Mem32<X86::BASE>(X86::EDX));
			// AND r32, r/m32
			testArch::And(block, X86::ECX, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestOr) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// OR AL, imm8
			testArch::Or<X86::BYTE_PTR>(block, S8(50));
			// OR AX, imm16
			testArch::Or<X86::WORD_PTR>(block, S16(5000));
			// OR EAX, imm32
			testArch::Or<X86::DWORD_PTR>(block, S32(5000000));
			// OR r/m8, imm8
			testArch::Or(block, X86::DL, S8(50));
			testArch::Or(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// OR r/m16, imm16
			testArch::Or(block, X86::DX, S16(5000));
			testArch::Or(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			// OR r/m32, imm32
			testArch::Or(block, X86::EDX, S32(5000000));
			testArch::Or(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			// OR r/m16, imm8
			testArch::Or(block, X86::DX, S8(50));
			testArch::Or<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// OR r/m32, imm8
			testArch::Or(block, X86::EDX, S8(50));
			testArch::Or<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// OR r/m8, r8
			testArch::Or(block, X86::DL, X86::CL);
			testArch::Or(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			// OR r/m16, r16
			testArch::Or(block, X86::DX, X86::CX);
			testArch::Or(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			// OR r/m32, r32
			testArch::Or(block, X86::EDX, X86::ECX);
			testArch::Or(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// OR r8, r/m8
			testArch::Or(block, X86::CL, X86::Mem32<X86::BASE>(X86::EDX));
			// OR r16, r/m16
			testArch::Or(block, X86::CX, X86::Mem32<X86::BASE>(X86::EDX));
			// OR r32, r/m32
			testArch::Or(block, X86::ECX, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestXor) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// XOR AL, imm8
			testArch::Xor<X86::BYTE_PTR>(block, S8(50));
			// XOR AX, imm16
			testArch::Xor<X86::WORD_PTR>(block, S16(5000));
			// XOR EAX, imm32
			testArch::Xor<X86::DWORD_PTR>(block, S32(5000000));
			// XOR r/m8, imm8
			testArch::Xor(block, X86::DL, S8(50));
			testArch::Xor(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// XOR r/m16, imm16
			testArch::Xor(block, X86::DX, S16(5000));
			testArch::Xor(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
			// XOR r/m32, imm32
			testArch::Xor(block, X86::EDX, S32(5000000));
			testArch::Xor(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
			// XOR r/m16, imm8
			testArch::Xor(block, X86::DX, S8(50));
			testArch::Xor<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// XOR r/m32, imm8
			testArch::Xor(block, X86::EDX, S8(50));
			testArch::Xor<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// XOR r/m8, r8
			testArch::Xor(block, X86::DL, X86::CL);
			testArch::Xor(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			// XOR r/m16, r16
			testArch::Xor(block, X86::DX, X86::CX);
			testArch::Xor(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			// XOR r/m32, r32
			testArch::Xor(block, X86::EDX, X86::ECX);
			testArch::Xor(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
			// XOR r8, r/m8
			testArch::Xor(block, X86::CL, X86::Mem32<X86::BASE>(X86::EDX));
			// XOR r16, r/m16
			testArch::Xor(block, X86::CX, X86::Mem32<X86::BASE>(X86::EDX));
			// XOR r32, r/m32
			testArch::Xor(block, X86::ECX, X86::Mem32<X86::BASE>(X86::EDX));
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

		TEST_METHOD(TestPop) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// POP r/m16
			testArch::Pop(block, X86::AX);
			testArch::Pop<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// POP r/m32
			testArch::Pop(block, X86::EAX);
			testArch::Pop<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// POP DS
			testArch::Pop(block, X86::DS);
			// POP ES
			testArch::Pop(block, X86::ES);
			// POP SS
			testArch::Pop(block, X86::SS);
			// POP FS
			testArch::Pop(block, X86::FS);
			// POP GS
			testArch::Pop(block, X86::GS);
		}

		TEST_METHOD(TestCall) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// CALL rel32 (near)
			Addr addr = block.getCurrentPtr();
			testArch::Call(block).bind(block);
			testArch::Call(block, addr);
			// CALL r/m32 (near)
			testArch::Call(block, X86::EDX);
			testArch::Call(block, X86::Mem32<X86::BASE>(X86::EDX));
			// CALL ptr16:32 (far)
			testArch::Call(block, S16(5000), S32(5000000));
			// CALL m16:32 (far)
			testArch::Call<X86::FWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestJmp) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// JMP rel8 (short)
			testArch::Jmp<X86::SHORT>(block).bind(block);
			// JMP rel32 (near)
			testArch::Jmp<X86::LONG>(block).bind(block);
			// JMP r/m32
			testArch::Jmp(block, X86::EDX);
			testArch::Jmp(block, X86::Mem32<X86::BASE>(X86::EDX));
			// JMP ptr16:32 (far)
			testArch::Jmp(block, S16(5000), S32(5000000));
			// JMP m16:32 (far)
			testArch::Jmp<X86::FWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestInt) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// INT imm8
			testArch::Int(block, U8(1));
			testArch::Int(block, U8(15));
			testArch::Int(block, U8(127));
		}

		TEST_METHOD(TestRet) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// RET (near)
			testArch::Retn(block);
			// RET (far)
			testArch::Retf(block);
			// RET imm16 (near)
			testArch::Retn(block, U16(4));
			// RET imm16 (far)
			testArch::Retf(block, U16(4));
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
		}

		TEST_METHOD(TestImul) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// IMUL r/m8
			testArch::Imul(block, X86::CH);
			testArch::Imul<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// IMUL r/m16
			testArch::Imul(block, X86::CX);
			testArch::Imul<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// IMUL r/m32
			testArch::Imul(block, X86::ECX);
			testArch::Imul<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// IMUL r16, r/m16
			testArch::Imul(block, X86::CX, X86::SI);
			testArch::Imul(block, X86::CX, X86::Mem32<X86::BASE>(X86::EDX));
			// IMUL r32, r/m32
			testArch::Imul(block, X86::ECX, X86::ESI);
			testArch::Imul(block, X86::ECX, X86::Mem32<X86::BASE>(X86::EDX));
			// IMUL r16, r/m16, imm8
			testArch::Imul(block, X86::CX, X86::SI, S8(50));
			testArch::Imul(block, X86::CX, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// IMUL r32, r/m32, imm8
			testArch::Imul(block, X86::ECX, X86::ESI, S8(50));
			testArch::Imul(block, X86::ECX, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
			// IMUL r16, imm8
			testArch::Imul(block, X86::CX, S8(50));
			// IMUL r32, imm8
			testArch::Imul(block, X86::ECX, S8(50));
			// IMUL r16, r/m16, imm16
			testArch::Imul(block, X86::CX, X86::DX, S16(50));
			testArch::Imul(block, X86::CX, X86::Mem32<X86::BASE>(X86::EDX), S16(50));
			// IMUL r32, r/m32, imm32
			testArch::Imul(block, X86::ECX, X86::EDX, S32(50));
			testArch::Imul(block, X86::ECX, X86::Mem32<X86::BASE>(X86::EDX), S32(50));
			// IMUL r16, imm16
			testArch::Imul(block, X86::CX, S16(50));
			// IMUL r32, imm32
			testArch::Imul(block, X86::ECX, S32(50));
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

		TEST_METHOD(TestBsr) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// BSR r16, r/m16
			testArch::Bsr(block, X86::DX, X86::CX);
			testArch::Bsr(block, X86::DX, X86::Mem32<X86::BASE>(X86::EDX));
			// BSR r32, r/m32
			testArch::Bsr(block, X86::EDX, X86::ECX);
			testArch::Bsr(block, X86::EDX, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestBtc) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// BTC r/m16, r16
			testArch::Btc(block, X86::DX, X86::CX);
			testArch::Btc(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX);
			// BTC r/m32, r32
			testArch::Btc(block, X86::EDX, X86::ECX);
			testArch::Btc(block, X86::Mem32<X86::BASE>(X86::EDX), X86::EDX);
			// BTC r/m16, imm8
			testArch::Btc(block, X86::DX, U8(50));
			testArch::Btc<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(50));
			// BTC r/m32, imm8
			testArch::Btc(block, X86::EDX, U8(50));
			testArch::Btc<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(50));
		}

		TEST_METHOD(TestBtr) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// BTR r/m16, r16
			testArch::Btr(block, X86::DX, X86::CX);
			testArch::Btr(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX);
			// BTR r/m32, r32
			testArch::Btr(block, X86::EDX, X86::ECX);
			testArch::Btr(block, X86::Mem32<X86::BASE>(X86::EDX), X86::EDX);
			// BTR r/m16, imm8
			testArch::Btr(block, X86::DX, U8(50));
			testArch::Btr<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(50));
			// BTR r/m32, imm8
			testArch::Btr(block, X86::EDX, U8(50));
			testArch::Btr<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(50));
		}

		TEST_METHOD(TestBts) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// BTS r/m16, r16
			testArch::Bts(block, X86::DX, X86::CX);
			testArch::Bts(block, X86::Mem32<X86::BASE>(X86::EDX), X86::DX);
			// BTS r/m32, r32
			testArch::Bts(block, X86::EDX, X86::ECX);
			testArch::Bts(block, X86::Mem32<X86::BASE>(X86::EDX), X86::EDX);
			// BTS r/m16, imm8
			testArch::Bts(block, X86::DX, U8(50));
			testArch::Bts<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(50));
			// BTS r/m32, imm8
			testArch::Bts(block, X86::EDX, U8(50));
			testArch::Bts<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(50));
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

		TEST_METHOD(TestRol) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// ROL r/m8, 1
			testArch::Rol(block, X86::DL);
			testArch::Rol<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// ROL r/m8, CL
			testArch::Rol<X86::CL>(block, X86::DL);
			testArch::Rol<X86::BYTE_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// ROL r/m8, imm8
			testArch::Rol(block, X86::DL, U8(5));
			testArch::Rol<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(5));
			// ROL r/m16, 1
			testArch::Rol(block, X86::DX);
			testArch::Rol<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// ROL r/m16, CL
			testArch::Rol<X86::CL>(block, X86::DX);
			testArch::Rol<X86::WORD_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// ROL r/m16, imm8
			testArch::Rol(block, X86::DX, U8(5));
			testArch::Rol<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(5));
			// ROL r/m32, 1
			testArch::Rol(block, X86::EDX);
			testArch::Rol<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// ROL r/m32, CL
			testArch::Rol<X86::CL>(block, X86::EDX);
			testArch::Rol<X86::DWORD_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// ROL r/m32, imm8
			testArch::Rol(block, X86::EDX, U8(5));
			testArch::Rol<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(5));
		}

		TEST_METHOD(TestRcr) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// RCR r/m8, 1
			testArch::Rcr(block, X86::DL);
			testArch::Rcr<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// RCR r/m8, CL
			testArch::Rcr<X86::CL>(block, X86::DL);
			testArch::Rcr<X86::BYTE_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// RCR r/m8, imm8
			testArch::Rcr(block, X86::DL, U8(5));
			testArch::Rcr<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(5));
			// RCR r/m16, 1
			testArch::Rcr(block, X86::DX);
			testArch::Rcr<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// RCR r/m16, CL
			testArch::Rcr<X86::CL>(block, X86::DX);
			testArch::Rcr<X86::WORD_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// RCR r/m16, imm8
			testArch::Rcr(block, X86::DX, U8(5));
			testArch::Rcr<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(5));
			// RCR r/m32, 1
			testArch::Rcr(block, X86::EDX);
			testArch::Rcr<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// RCR r/m32, CL
			testArch::Rcr<X86::CL>(block, X86::EDX);
			testArch::Rcr<X86::DWORD_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// RCR r/m32, imm8
			testArch::Rcr(block, X86::EDX, U8(5));
			testArch::Rcr<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(5));
		}

		TEST_METHOD(TestRcl) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// RCL r/m8, 1
			testArch::Rcl(block, X86::DL);
			testArch::Rcl<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// RCL r/m8, CL
			testArch::Rcl<X86::CL>(block, X86::DL);
			testArch::Rcl<X86::BYTE_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// RCL r/m8, imm8
			testArch::Rcl(block, X86::DL, U8(5));
			testArch::Rcl<X86::BYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(5));
			// RCL r/m16, 1
			testArch::Rcl(block, X86::DX);
			testArch::Rcl<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// RCL r/m16, CL
			testArch::Rcl<X86::CL>(block, X86::DX);
			testArch::Rcl<X86::WORD_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// RCL r/m16, imm8
			testArch::Rcl(block, X86::DX, U8(5));
			testArch::Rcl<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(5));
			// RCL r/m32, 1
			testArch::Rcl(block, X86::EDX);
			testArch::Rcl<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// RCL r/m32, CL
			testArch::Rcl<X86::CL>(block, X86::EDX);
			testArch::Rcl<X86::DWORD_PTR, X86::CL>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// RCL r/m32, imm8
			testArch::Rcl(block, X86::EDX, U8(5));
			testArch::Rcl<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX), U8(5));
		}

		TEST_METHOD(TestMovsx) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// MOVSX r16, r/m8
			testArch::Movsx(block, X86::DX, X86::CL);
			testArch::Movsx<X86::BYTE_PTR>(block, X86::DX, X86::Mem32<X86::BASE>(X86::EDX));
			// MOVSX r32, r/m8
			testArch::Movsx(block, X86::EDX, X86::CL);
			testArch::Movsx<X86::BYTE_PTR>(block, X86::EDX, X86::Mem32<X86::BASE>(X86::EDX));
			// MOVSX r32, r/m16
			testArch::Movsx(block, X86::EDX, X86::CX);
			testArch::Movsx<X86::WORD_PTR>(block, X86::EDX, X86::Mem32<X86::BASE>(X86::EDX));
			// MOVSX r16, m16 (undocumented)
			testArch::Movsx<X86::WORD_PTR>(block, X86::DX, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestMovzx) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// MOVZX r16, r/m8
			testArch::Movzx(block, X86::DX, X86::CL);
			testArch::Movzx<X86::BYTE_PTR>(block, X86::DX, X86::Mem32<X86::BASE>(X86::EDX));
			// MOVZX r32, r/m8
			testArch::Movzx(block, X86::EDX, X86::CL);
			testArch::Movzx<X86::BYTE_PTR>(block, X86::EDX, X86::Mem32<X86::BASE>(X86::EDX));
			// MOVZX r32, r/m16
			testArch::Movzx(block, X86::EDX, X86::CX);
			testArch::Movzx<X86::WORD_PTR>(block, X86::EDX, X86::Mem32<X86::BASE>(X86::EDX));
			// MOVZX r16, m16 (undocumented)
			testArch::Movzx<X86::WORD_PTR>(block, X86::DX, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestJcc) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// JA rel8
			testArch::Ja<X86::SHORT>(block).bind(block);
			// JAE rel8
			testArch::Jae<X86::SHORT>(block).bind(block);
			// JB rel8
			testArch::Jb<X86::SHORT>(block).bind(block);
			// JBE rel8
			testArch::Jbe<X86::SHORT>(block).bind(block);
			// JC rel8
			testArch::Jc<X86::SHORT>(block).bind(block);
			// JCXZ rel8
			testArch::Jcxz(block).bind(block);
			// JECXZ rel8
			testArch::Jecxz(block).bind(block);
			// JE rel8
			testArch::Je<X86::SHORT>(block).bind(block);
			// JG rel8
			testArch::Jg<X86::SHORT>(block).bind(block);
			// JGE rel8
			testArch::Jge<X86::SHORT>(block).bind(block);
			// JL rel8
			testArch::Jl<X86::SHORT>(block).bind(block);
			// JLE rel8
			testArch::Jle<X86::SHORT>(block).bind(block);
			// JNA rel8
			testArch::Jna<X86::SHORT>(block).bind(block);
			// JNAE rel8
			testArch::Jnae<X86::SHORT>(block).bind(block);
			// JNB rel8
			testArch::Jnb<X86::SHORT>(block).bind(block);
			// JNBE rel8
			testArch::Jnbe<X86::SHORT>(block).bind(block);
			// JNC rel8
			testArch::Jnc<X86::SHORT>(block).bind(block);
			// JNE rel8
			testArch::Jne<X86::SHORT>(block).bind(block);
			// JNG rel8
			testArch::Jng<X86::SHORT>(block).bind(block);
			// JNGE rel8
			testArch::Jnge<X86::SHORT>(block).bind(block);
			// JNL rel8
			testArch::Jnl<X86::SHORT>(block).bind(block);
			// JNLE rel8
			testArch::Jnle<X86::SHORT>(block).bind(block);
			// JNO rel8
			testArch::Jno<X86::SHORT>(block).bind(block);
			// JNP rel8
			testArch::Jnp<X86::SHORT>(block).bind(block);
			// JNS rel8
			testArch::Jns<X86::SHORT>(block).bind(block);
			// JNZ rel8
			testArch::Jnz<X86::SHORT>(block).bind(block);
			// JO rel8
			testArch::Jo<X86::SHORT>(block).bind(block);
			// JP rel8
			testArch::Jp<X86::SHORT>(block).bind(block);
			// JPE rel8
			testArch::Jpe<X86::SHORT>(block).bind(block);
			// JPO rel8
			testArch::Jpo<X86::SHORT>(block).bind(block);
			// JS rel8
			testArch::Js<X86::SHORT>(block).bind(block);
			// JZ rel8
			testArch::Jz<X86::SHORT>(block).bind(block);
			// JA rel32
			testArch::Ja<X86::LONG>(block).bind(block);
			// JAE rel32
			testArch::Jae<X86::LONG>(block).bind(block);
			// JB rel32
			testArch::Jb<X86::LONG>(block).bind(block);
			// JBE rel32
			testArch::Jbe<X86::LONG>(block).bind(block);
			// JC rel32
			testArch::Jc<X86::LONG>(block).bind(block);
			// JE rel32
			testArch::Je<X86::LONG>(block).bind(block);
			// JG rel32
			testArch::Jg<X86::LONG>(block).bind(block);
			// JGE rel32
			testArch::Jge<X86::LONG>(block).bind(block);
			// JL rel32
			testArch::Jl<X86::LONG>(block).bind(block);
			// JLE rel32
			testArch::Jle<X86::LONG>(block).bind(block);
			// JNA rel32
			testArch::Jna<X86::LONG>(block).bind(block);
			// JNAE rel32
			testArch::Jnae<X86::LONG>(block).bind(block);
			// JNB rel32
			testArch::Jnb<X86::LONG>(block).bind(block);
			// JNBE rel32
			testArch::Jnbe<X86::LONG>(block).bind(block);
			// JNC rel32
			testArch::Jnc<X86::LONG>(block).bind(block);
			// JNE rel32
			testArch::Jne<X86::LONG>(block).bind(block);
			// JNG rel32
			testArch::Jng<X86::LONG>(block).bind(block);
			// JNGE rel32
			testArch::Jnge<X86::LONG>(block).bind(block);
			// JNL rel32
			testArch::Jnl<X86::LONG>(block).bind(block);
			// JNLE rel32
			testArch::Jnle<X86::LONG>(block).bind(block);
			// JNO rel32
			testArch::Jno<X86::LONG>(block).bind(block);
			// JNP rel32
			testArch::Jnp<X86::LONG>(block).bind(block);
			// JNS rel32
			testArch::Jns<X86::LONG>(block).bind(block);
			// JNZ rel32
			testArch::Jnz<X86::LONG>(block).bind(block);
			// JO rel32
			testArch::Jo<X86::LONG>(block).bind(block);
			// JP rel32
			testArch::Jp<X86::LONG>(block).bind(block);
			// JPE rel32
			testArch::Jpe<X86::LONG>(block).bind(block);
			// JPO rel32
			testArch::Jpo<X86::LONG>(block).bind(block);
			// JS rel32
			testArch::Js<X86::LONG>(block).bind(block);
			// JZ rel32
			testArch::Jz<X86::LONG>(block).bind(block);
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
	
		TEST_METHOD(TestLcS) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// LDS r16, m16:16
			testArch::Lds(block, X86::AX, X86::Mem32<X86::BASE>(X86::EDX));
			// LDS r32, m16:32
			testArch::Lds(block, X86::EAX, X86::Mem32<X86::BASE>(X86::EDX));
			// LSS r16, m16:16
			testArch::Lss(block, X86::AX, X86::Mem32<X86::BASE>(X86::EDX));
			// LDS r32, m16:32
			testArch::Lss(block, X86::EAX, X86::Mem32<X86::BASE>(X86::EDX));
			// LES r16, m16:16
			testArch::Les(block, X86::AX, X86::Mem32<X86::BASE>(X86::EDX));
			// LES r32, m16:32
			testArch::Les(block, X86::EAX, X86::Mem32<X86::BASE>(X86::EDX));
			// LFS r16, m16:16
			testArch::Lfs(block, X86::AX, X86::Mem32<X86::BASE>(X86::EDX));
			// LFS r32, m16:32
			testArch::Lfs(block, X86::EAX, X86::Mem32<X86::BASE>(X86::EDX));
			// LGS r16, m16:16
			testArch::Lgs(block, X86::AX, X86::Mem32<X86::BASE>(X86::EDX));
			// LGS r32, m16:32
			testArch::Lgs(block, X86::EAX, X86::Mem32<X86::BASE>(X86::EDX));
		}

		/* i486 specific */

		TEST_METHOD(TestBswap) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// BSWAP r32
			testArch4::Bswap(block, X86::ECX);
		}

		TEST_METHOD(TestCmpXchg) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// CMPXCHG r/m8, r8
			testArch4::CmpXchg(block, X86::DL, X86::DH);
			testArch4::CmpXchg(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
			// CMPXCHG r/m16, r16
			testArch4::CmpXchg(block, X86::DX, X86::CX);
			testArch4::CmpXchg(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			// CMPXCHG r/m32, r32
			testArch4::CmpXchg(block, X86::EDX, X86::ECX);
			testArch4::CmpXchg(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
		}

		TEST_METHOD(TestXadd) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// XADD r/m8, r8
			testArch4::Xadd(block, X86::AL, X86::AH);
			testArch4::Xadd(block, X86::Mem32<X86::BASE>(X86::EDX), X86::AH);
			// XADD r/m16, r16
			testArch4::Xadd(block, X86::AX, X86::CX);
			testArch4::Xadd(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
			// XADD r/m32, r32
			testArch4::Xadd(block, X86::EAX, X86::ECX);
			testArch4::Xadd(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);
		}

		TEST_METHOD(TestInvlpg) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// INVLPG m
			testArch4::Invlpg(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		/* i586 specific */

		TEST_METHOD(TestCmpXchg8b) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// CMPXCHG8B m64
			testArch5::CmpXchg8b(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		/* i686 specific */

		TEST_METHOD(TestFpuNoParams) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			testArchFpu::F2xm1(block);
			testArchFpu::Fabs(block);
			testArchFpu::Fchs(block);
			testArchFpu::Fclex(block);
			testArchFpu::Fnclex(block);
			testArchFpu::Fcompp(block);
			testArchFpu::Fdecstp(block);
			testArchFpu::Fincstp(block);
			testArchFpu::Finit(block);
			testArchFpu::Fninit(block);
			testArchFpu::Fld1(block);
			testArchFpu::FldL2E(block);
			testArchFpu::FldL2T(block);
			testArchFpu::FldLG2(block);
			testArchFpu::FldLN2(block);
			testArchFpu::FldPI(block);
			testArchFpu::FldZ(block);
			testArchFpu::Fnop(block);
			testArchFpu::Fpatan(block);
			testArchFpu::Fprem(block);
			testArchFpu::Fptan(block);
			testArchFpu::Frndint(block);
			testArchFpu::Fscale(block);
			testArchFpu::Fsqrt(block);
			testArchFpu::Ftst(block);
			testArchFpu::Wait(block);
			testArchFpu::Fwait(block);
			testArchFpu::Fxam(block);
			testArchFpu::Fxtract(block);
			testArchFpu::FyL2X(block);
			testArchFpu::FyL2XP1(block);
			testArchFpu::Fcos(block);
			testArchFpu::Fprem1(block);
			testArchFpu::Fsin(block);
			testArchFpu::Fsincos(block);
			testArchFpu::Fucompp(block);

		}

		TEST_METHOD(TestFadd) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FADD m32fp
			testArchFpu::Fadd<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FADD m64fp
			testArchFpu::Fadd<X86::QWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FADD ST(0), ST(i)
			testArchFpu::Fadd(block, X86::ST, X86::ST1);
			// FADD ST(i), ST(0)
			testArchFpu::Fadd(block, X86::ST1, X86::ST);
		}

		TEST_METHOD(TestFaddp) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FADDP ST(i), ST(0)
			testArchFpu::Faddp(block, X86::ST1, X86::ST);
			// FADDP
			testArchFpu::Faddp(block);
		}

		TEST_METHOD(TestFbld) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FBLD m80
			testArchFpu::Fbld(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFbstp) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FBSTP m80bcd
			testArchFpu::Fbstp(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFcom) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FCOM m32fp
			testArchFpu::Fcom<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FCOM m64fp
			testArchFpu::Fcom<X86::QWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FCOM ST(i)
			testArchFpu::Fcom(block, X86::ST1);
			// FCOM
			testArchFpu::Fcom(block);
		}

		TEST_METHOD(TestFcomp) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FCOMP m32fp
			testArchFpu::Fcomp<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FCOMP m64fp
			testArchFpu::Fcomp<X86::QWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FCOMP ST(i)
			testArchFpu::Fcomp(block, X86::ST1);
			// FCOMP
			testArchFpu::Fcomp(block);
		}

		TEST_METHOD(TestFdiv) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FDIV m32fp
			testArchFpu::Fdiv<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FDIV m64fp
			testArchFpu::Fdiv<X86::QWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FDIV ST(0), ST(i)
			testArchFpu::Fdiv(block, X86::ST, X86::ST1);
			// FDIV ST(i), ST(0)
			testArchFpu::Fdiv(block, X86::ST1, X86::ST);
		}

		TEST_METHOD(TestFdivp) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FDIVP ST(i), ST(0)
			testArchFpu::Fdivp(block, X86::ST1, X86::ST);
			// FDIVP
			testArchFpu::Fdivp(block);
		}

		TEST_METHOD(TestFdivr) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FDIVR m32fp
			testArchFpu::Fdivr<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FDIVR m64fp
			testArchFpu::Fdivr<X86::QWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FDIVR ST(0), ST(i)
			testArchFpu::Fdivr(block, X86::ST, X86::ST1);
			// FDIVR ST(i), ST(0)
			testArchFpu::Fdivr(block, X86::ST1, X86::ST);
		}

		TEST_METHOD(TestFdivrp) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FDIVRP ST(i), ST(0)
			testArchFpu::Fdivrp(block, X86::ST1, X86::ST);
			// FDIVRP
			testArchFpu::Fdivrp(block);
		}

		TEST_METHOD(TestFfree) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FFREE ST(i)
			testArchFpu::Ffree(block, X86::ST1);
		}

		TEST_METHOD(TestFiadd) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FIADD m32int
			testArchFpu::Fiadd<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FIADD m16int
			testArchFpu::Fiadd<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFicom) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FICOM m32int
			testArchFpu::Ficom<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FICOM m16int
			testArchFpu::Ficom<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFicomp) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FICOMP m32int
			testArchFpu::Ficomp<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FICOMP m16int
			testArchFpu::Ficomp<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFidiv) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FIDIV m32int
			testArchFpu::Fidiv<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FIDIV m16int
			testArchFpu::Fidiv<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFidivr) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FIDIVR m32int
			testArchFpu::Fidivr<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FIDIVR m16int
			testArchFpu::Fidivr<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFild) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FILD m16int
			testArchFpu::Fild<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FILD m32int
			testArchFpu::Fild<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FILD m64int
			testArchFpu::Fild<X86::QWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFimul) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FIMUL m32int
			testArchFpu::Fimul<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FIMUL m16int
			testArchFpu::Fimul<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFist) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FIST m16int
			testArchFpu::Fist<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FIST m32int
			testArchFpu::Fist<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFistp) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FISTP m16int
			testArchFpu::Fistp<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FISTP m32int
			testArchFpu::Fistp<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FISTP m64int
			testArchFpu::Fistp<X86::QWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFisub) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FISUB m16int
			testArchFpu::Fisub<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FISUB m32int
			testArchFpu::Fisub<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFisubr) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FISUBR m16int
			testArchFpu::Fisubr<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FISUBR m32int
			testArchFpu::Fisubr<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFld) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FLD m32fp
			testArchFpu::Fld<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FLD m64fp
			testArchFpu::Fld<X86::QWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FLD m80fp
			testArchFpu::Fld<X86::TBYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FLD ST(i)
			testArchFpu::Fld(block, X86::ST1);
		}

		TEST_METHOD(TestFldcw) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FLDCW m2byte
			testArchFpu::Fldcw(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFldenv) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FLDENV m14/28byte
			testArchFpu::Fldenv(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFmul) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FMUL m32fp
			testArchFpu::Fmul<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FMUL m64fp
			testArchFpu::Fmul<X86::QWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FMUL ST(0), ST(i)
			testArchFpu::Fmul(block, X86::ST, X86::ST1);
			// FMUL ST(i), ST(0)
			testArchFpu::Fmul(block, X86::ST1, X86::ST);
		}

		TEST_METHOD(TestFmulp) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FMULP ST(i), ST(0)
			testArchFpu::Fmulp(block, X86::ST1, X86::ST);
			// FMULP
			testArchFpu::Fmulp(block);
		}

		TEST_METHOD(TestFsave) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FSAVE m94/108byte
			testArchFpu::Fsave(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFnsave) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FNSAVE m94/108byte
			testArchFpu::Fnsave(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFstcw) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FNSTCW m2byte
			testArchFpu::Fstcw(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFnstcw) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FNSTCW m2byte
			testArchFpu::Fnstcw(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFstenv) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FSTENV m14/28byte
			testArchFpu::Fstenv(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFnstenv) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FNSTENV m14/28byte
			testArchFpu::Fnstenv(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFstsw) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FSTSW m2byte
			testArchFpu::Fstsw(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FSTSW AX
			testArchFpu::Fstsw(block);
		}

		TEST_METHOD(TestFnstsw) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FNSTSW m2byte
			testArchFpu::Fnstsw(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FNSTSW AX
			testArchFpu::Fnstsw(block);
		}

		TEST_METHOD(TestRstor) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FRSTOR m94/108byte
			testArchFpu::Frstor(block, X86::Mem32<X86::BASE>(X86::EDX));
		}

		TEST_METHOD(TestFst) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FST m32fp
			testArchFpu::Fst<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FST m64fp
			testArchFpu::Fst<X86::QWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FST ST(i)
			testArchFpu::Fst(block, X86::ST1);
		}

		TEST_METHOD(TestFstp) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FSTP m32fp
			testArchFpu::Fstp<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FSTP m64fp
			testArchFpu::Fstp<X86::QWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FSTP m80fp
			testArchFpu::Fstp<X86::TBYTE_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FSTP ST(i)
			testArchFpu::Fstp(block, X86::ST1);
		}

		TEST_METHOD(TestFsub) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FSUB m32fp
			testArchFpu::Fsub<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FSUB m64fp
			testArchFpu::Fsub<X86::QWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FSUB ST(0), ST(i)
			testArchFpu::Fsub(block, X86::ST, X86::ST1);
			// FSUB ST(i), ST(0)
			testArchFpu::Fsub(block, X86::ST1, X86::ST);
		}

		TEST_METHOD(TestFsubp) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FSUBP ST(i), ST(0)
			testArchFpu::Fsubp(block, X86::ST1, X86::ST);
			// FSUBP
			testArchFpu::Fsubp(block);
		}

		TEST_METHOD(TestFsubr) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FSUBR m32fp
			testArchFpu::Fsubr<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FSUBR m64fp
			testArchFpu::Fsubr<X86::QWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EDX));
			// FSUBR ST(0), ST(i)
			testArchFpu::Fsubr(block, X86::ST, X86::ST1);
			// FSUBR ST(i), ST(0)
			testArchFpu::Fsubr(block, X86::ST1, X86::ST);
		}		
		
		TEST_METHOD(TestFsubrp) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FSUBRP ST(i), ST(0)
			testArchFpu::Fsubrp(block, X86::ST1, X86::ST);
			// FSUBRP
			testArchFpu::Fsubrp(block);
		}

		TEST_METHOD(TestFxch) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FXCH ST(i)
			testArchFpu::Fxch(block, X86::ST1);
			// FXCH
			testArchFpu::Fxch(block);
		}

		TEST_METHOD(TestFucom) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FUCOM ST(i)
			testArchFpu::Fucom(block, X86::ST1);
			// FUCOM
			testArchFpu::Fucom(block);
		}

		TEST_METHOD(TestFucomp) {
			testCodeBlock block(CODE_BLOCK_SIZE);
			// FUCOMP ST(i)
			testArchFpu::Fucomp(block, X86::ST1);
			// FUCOMP
			testArchFpu::Fucomp(block);
		}
	};
}