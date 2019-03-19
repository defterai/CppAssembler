#include <iostream>

#include "asm\arch\x86_i386.h"
#include "asm\arch\x86_i486.h"
#include "asm\arch\x86_i586.h"
#include "asm\arch\x86_i387.h"
#include "asm\arch\x86_i686.h"
#include "asm\arch\x64_i386.h"
#include "asm\os_win32.h"

using namespace CppAsm;


typedef Arch::CodeGen<X64::i386> GArch;
//typedef Arch::CodeGen<X86::i386> GArch;

int main()
{
	Win32::CodeBlock block(1024);

	{
		/*GArch::Xchg(block, X86::Mem32<X86::OFFSET>(5000), X86::EDX);
		//GArch::Xchg(block, X86::Mem32<X86::BASE>(X86::ECX), X86::EDX);*/
		//GArch::Xchg(block, X86::Mem32<X86::BASE_OFFSET>(X86::ECX, 5000), X86::EDX);
		/*GArch::Xchg(block, X86::Mem32<X86::INDEX_OFFSET>(X86::ECX, X86::SCALE_8, 5000), X86::EDX);
		GArch::Xchg(block, X86::Mem32<X86::BASE_INDEX>(X86::EAX, X86::ECX, X86::SCALE_8), X86::EDX);
		GArch::Xchg(block, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::EAX, X86::ECX, X86::SCALE_8, 10), X86::EDX);
		GArch::Xchg(block, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::EAX, X86::ECX, X86::SCALE_8, 1000), X86::EDX);
		
		GArch::Lea(block, X86::EDX, X86::Mem32<X86::OFFSET>(5000));
		GArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE>(X86::ECX));
		GArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_OFFSET>(X86::ECX, 50));
		GArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_OFFSET>(X86::ECX, 5000));
		GArch::Lea(block, X86::EDX, X86::Mem32<X86::INDEX_OFFSET>(X86::ECX, X86::SCALE_8, 5000));
		GArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_INDEX>(X86::EAX, X86::ECX, X86::SCALE_8));
		GArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::EAX, X86::ECX, X86::SCALE_8, 10));
		GArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::EAX, X86::ECX, X86::SCALE_8, 1000));

		GArch::F2xm1(block);
		GArch::Fcom<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EAX));
		GArch::Fcom<X86::QWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EAX));
		GArch::Fadd(block, X86::ST, X86::ST6);
		GArch::Fadd(block, X86::ST6, X86::ST);
		GArch::Faddp(block, X86::ST6, X86::ST);
		GArch::Fiadd<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EAX));
		GArch::Fiadd<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EAX));
		GArch::Fcomi(block, X86::ST, X86::ST6);
		GArch::Fcomip(block, X86::ST, X86::ST6);
		GArch::Fucomi(block, X86::ST, X86::ST6);
		GArch::Fucomip(block, X86::ST, X86::ST6);
		GArch::Ret(block);
		GArch::CmpXchg8b<>(block, X86::Mem32<X86::BASE>(X86::EAX));
		GArch::Rdtsc(block);
		GArch::Rdmsr(block);
		GArch::Wrmsr(block);
		GArch::Rsm(block);*/

		/*GArch::Mov(block, X64::RAX, X64::RCX);
		GArch::Mov(block, X64::EAX, X64::ECX);
		GArch::Mov(block, X64::AX, X64::CX);
		GArch::Mov(block, X64::AL, X64::CL);

		GArch::Mov(block, X64::RAX, X64::R9);
		GArch::Mov(block, X64::EAX, X64::R9d);
		GArch::Mov(block, X64::R9, X64::RAX);
		GArch::Mov(block, X64::R9d, X64::EAX);
		GArch::Mov(block, X64::R9, X64::R9);
		GArch::Mov(block, X64::R9d, X64::R9d);

		GArch::Mov(block, X64::R15, X64::R14);
		GArch::Mov(block, X64::R15d, X64::R14d);*/

		//GArch::Mov(block, X64::RDX, S32(500));
		//GArch::Mov(block, X64::R14, S32(500));

		/*GArch::Mov<X64::OFFSET>(block, X64::Mem64<>(5), X64::R14);
		GArch::Mov<X64::OFFSET>(block, X64::Mem64<>(5), X64::RAX);
		GArch::Mov<X64::OFFSET>(block, X64::Mem64<>(5), X64::EAX);
		GArch::Mov<X64::OFFSET>(block, X64::Mem64<>(5), X64::AX);
		GArch::Mov<X64::OFFSET>(block, X64::Mem64<>(5), X64::AL);

		GArch::Mov<X64::OFFSET>(block, X64::R14, X64::Mem64<>(5));
		GArch::Mov<X64::OFFSET>(block, X64::RAX, X64::Mem64<>(5));
		GArch::Mov<X64::OFFSET>(block, X64::EAX, X64::Mem64<>(5));
		GArch::Mov<X64::OFFSET>(block, X64::AX, X64::Mem64<>(5));
		GArch::Mov<X64::OFFSET>(block, X64::AL, X64::Mem64<>(5));*/

		GArch::Lea(block, X64::EDX, X64::Mem64<X64::OFFSET>(0xFFFFFFFF));
		//GArch::Movsx(block, X64::DX, X64::Mem64<X64::BASE>(X64::RSP));

		GArch::Lea<X64::BASE_INDEX_OFFSET>(block, X64::RDX, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::R14, X64::R14, X64::SCALE_8, 5000));
		GArch::Lea<X64::BASE_INDEX_OFFSET>(block, X64::EDX, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::R14, X64::R14, X64::SCALE_8, 5000));
		GArch::Lea<X64::BASE_INDEX_OFFSET>(block, X64::DX, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::R14, X64::R14, X64::SCALE_8, 5000));
		GArch::Lea<X64::BASE_INDEX_OFFSET>(block, X64::RDX, X64::Mem32<X64::BASE_INDEX_OFFSET>(X64::R14d, X64::R14d, X64::SCALE_8, 5000));
		GArch::Lea<X64::BASE_INDEX_OFFSET>(block, X64::EDX, X64::Mem32<X64::BASE_INDEX_OFFSET>(X64::R14d, X64::R14d, X64::SCALE_8, 5000));
		GArch::Lea<X64::BASE_INDEX_OFFSET>(block, X64::DX, X64::Mem32<X64::BASE_INDEX_OFFSET>(X64::R14d, X64::R14d, X64::SCALE_8, 5000));

		GArch::Movsxd(block, X64::RDX, X64::EDX);
		GArch::Movsxd(block, X64::RDX, X64::Mem64<X64::BASE>(X64::RAX));
		GArch::Movsxd(block, X64::RDX, X64::Mem32<X64::BASE>(X64::EAX));

		GArch::Movzx(block, X64::EDX, X64::DX);
		GArch::Movzx(block, X64::DX, X64::DX);
		GArch::Movzx<X64::WORD_PTR>(block, X64::EDX, X64::Mem64<X64::BASE>(X64::RAX));
		GArch::Movzx<X64::BYTE_PTR>(block, X64::EDX, X64::Mem32<X64::BASE>(X64::EAX));
		GArch::Movzx(block, X64::DX, X64::DL);
		GArch::Movzx<X64::WORD_PTR>(block, X64::DX, X64::Mem64<X64::BASE>(X64::RAX));
		GArch::Movzx<X64::BYTE_PTR>(block, X64::DX, X64::Mem32<X64::BASE>(X64::EAX));

		GArch::Cmp<X64::BYTE_PTR>(block, X64::Mem64<X64::BASE>(X64::R14), S8(0x32));
		GArch::Cmp<X64::WORD_PTR>(block, X64::Mem64<X64::BASE>(X64::R14), S16(0x32));
		GArch::Cmp<X64::DWORD_PTR>(block, X64::Mem64<X64::BASE>(X64::R14), S32(0x32));
		GArch::Cmp<X64::QWORD_PTR>(block, X64::Mem64<X64::BASE>(X64::R14), S32(0x32));
		GArch::Cmp<X64::BYTE_PTR>(block, X64::Mem32<X64::BASE>(X64::R14d), S8(0x32));
		GArch::Cmp<X64::WORD_PTR>(block, X64::Mem32<X64::BASE>(X64::R14d), S16(0x32));
		GArch::Cmp<X64::DWORD_PTR>(block, X64::Mem32<X64::BASE>(X64::R14d), S32(0x32));
		GArch::Cmp<X64::QWORD_PTR>(block, X64::Mem32<X64::BASE>(X64::R14d), S32(0x32));

		GArch::Cmp<X64::BASE>(block, X64::Mem64<X64::BASE>(X64::R14), X64::EAX);
		GArch::Cmp<X64::BASE>(block, X64::Mem64<X64::BASE>(X64::R14), X64::AX);
		GArch::Cmp<X64::BASE>(block, X64::Mem64<X64::BASE>(X64::R14), X64::AL);
		GArch::Cmp<X64::BASE_OFFSET>(block, X64::Mem64<X64::BASE_OFFSET>(X64::RSP, 5000), X64::R14);
		GArch::Cmp<X64::BASE_OFFSET>(block, X64::Mem64<X64::BASE_OFFSET>(X64::RSP, 5000), X64::RSP);
		GArch::Cmp<X64::BASE_OFFSET>(block, X64::Mem64<X64::BASE_OFFSET>(X64::R14, 5000), X64::EAX);
		GArch::Cmp<X64::BASE_OFFSET>(block, X64::Mem64<X64::BASE_OFFSET>(X64::R14, 5000), X64::AX);
		GArch::Cmp<X64::BASE_OFFSET>(block, X64::Mem64<X64::BASE_OFFSET>(X64::R14, 5000), X64::AL);
		GArch::Cmp<X64::INDEX_OFFSET>(block, X64::Mem64<X64::INDEX_OFFSET>(X64::R14, X64::SCALE_8, 5000), X64::R14);
		GArch::Cmp<X64::INDEX_OFFSET>(block, X64::Mem64<X64::INDEX_OFFSET>(X64::R14, X64::SCALE_8, 5000), X64::RSP);
		GArch::Cmp<X64::INDEX_OFFSET>(block, X64::Mem64<X64::INDEX_OFFSET>(X64::R14, X64::SCALE_8, 5000), X64::EAX);
		GArch::Cmp<X64::INDEX_OFFSET>(block, X64::Mem64<X64::INDEX_OFFSET>(X64::R14, X64::SCALE_8, 5000), X64::AX);
		GArch::Cmp<X64::INDEX_OFFSET>(block, X64::Mem64<X64::INDEX_OFFSET>(X64::R14, X64::SCALE_8, 5000), X64::AL);
		GArch::Cmp<X64::BASE_INDEX>(block, X64::Mem64<X64::BASE_INDEX>(X64::R14, X64::RAX, X64::SCALE_8), X64::R14);
		GArch::Cmp<X64::BASE_INDEX>(block, X64::Mem64<X64::BASE_INDEX>(X64::R14, X64::R14, X64::SCALE_8), X64::RSP);
		GArch::Cmp<X64::BASE_INDEX>(block, X64::Mem64<X64::BASE_INDEX>(X64::R14, X64::R14, X64::SCALE_8), X64::EAX);
		GArch::Cmp<X64::BASE_INDEX>(block, X64::Mem64<X64::BASE_INDEX>(X64::R14, X64::R14, X64::SCALE_8), X64::AX);
		GArch::Cmp<X64::BASE_INDEX>(block, X64::Mem64<X64::BASE_INDEX>(X64::R14, X64::R14, X64::SCALE_8), X64::AL);
		GArch::Cmp<X64::BASE_INDEX_OFFSET>(block, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::RAX, X64::RAX, X64::SCALE_8, 5000), X64::R14);
		GArch::Cmp<X64::BASE_INDEX_OFFSET>(block, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::RDX, X64::R14, X64::SCALE_8, 5000), X64::RSP);
		GArch::Cmp<X64::BASE_INDEX_OFFSET>(block, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::R14, X64::R14, X64::SCALE_8, 5000), X64::EAX);
		GArch::Cmp<X64::BASE_INDEX_OFFSET>(block, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::R14, X64::R14, X64::SCALE_8, 5000), X64::AX);
		GArch::Cmp<X64::BASE_INDEX_OFFSET>(block, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::R14, X64::R14, X64::SCALE_8, 5000), X64::AL);

		block.invokeStdcall<int>();
	}

	return 0;
}