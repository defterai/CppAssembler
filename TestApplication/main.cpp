#include <iostream>

#include "asm/arch.h"
#include "asm/arch/x86_i386.h"
#include "asm/arch/x86_i486.h"
#include "asm/arch/x86_i586.h"
#include "asm/arch/x86_i387.h"
#include "asm/arch/x86_i686.h"
#include "asm/arch/x64_i386.h"
#include "asm/arch/x86_SEP.h"
#include "asm/arch/x86_MSR.h"
#include "asm/arch/x86_CLFSH.h"
#include "asm/arch/x86_CX8.h"

#ifdef __linux__
#ifdef __amd64__
#include "asm/os/linux64.h"
#else
#include "asm/os/linux32.h"
#endif
#else
#ifdef WIN32
#include "asm/os/win32.h"
#else // WIN32

#endif
#endif

using namespace CppAsm;

#ifdef WIN32
	typedef Arch::CustomArch<X86::i686, X86::SEP, X86::CLFSH> GArch;
	typedef GArch testArch;
	typedef Win32::CodeBlock testCodeBlock;
#else // WIN32
	typedef Arch::CustomArch<X64::i386> GArch;
	typedef Linux::CodeBlock testCodeBlock;
#endif 

int main()
{
	testCodeBlock block(1024);

	{
#ifdef WIN32
		testArch::Mov(block, X86::DL, S8(50));

		//testArch::Add<X86::BYTE_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
		//testArch::Add<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S16(5000));
		//testArch::Add<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S32(5000000));
		testArch::Inc<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX));
		//testArch::Add<X86::WORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
		testArch::Nop(block);
		testArch::Nop(block);
		testArch::Nop(block);
		testArch::Nop(block);
		testArch::Nop(block);
		testArch::Nop(block);
		testArch::Nop(block);
		testArch::Nop(block);
		//testArch::Add<X86::DWORD_PTR, X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), S8(50));
		//testArch::Add<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CL);
		//testArch::Add<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::CX);
		//testArch::Add<X86::LOCK>(block, X86::Mem32<X86::BASE>(X86::EDX), X86::ECX);

		Os::MeasureBlock measureBlock = block.subBlock<Os::MeasureBlock>(1024);
		GArch::Nop(measureBlock);
		GArch::Lds(measureBlock, X86::EDX, X86::Mem32<X86::OFFSET>(5000));
		GArch::Les(measureBlock, X86::EDX, X86::Mem32<X86::OFFSET>(5000));
		GArch::Lgs(measureBlock, X86::EDX, X86::Mem32<X86::OFFSET>(5000));
		GArch::Lfs(measureBlock, X86::EDX, X86::Mem32<X86::OFFSET>(5000));
		GArch::Lss(measureBlock, X86::EDX, X86::Mem32<X86::OFFSET>(5000));
		GArch::Lds(measureBlock, X86::DX, X86::Mem32<X86::OFFSET>(5000));
		GArch::Les(measureBlock, X86::DX, X86::Mem32<X86::OFFSET>(5000));
		GArch::Lgs(measureBlock, X86::DX, X86::Mem32<X86::OFFSET>(5000));
		GArch::Lfs(measureBlock, X86::DX, X86::Mem32<X86::OFFSET>(5000));
		GArch::Lss(measureBlock, X86::DX, X86::Mem32<X86::OFFSET>(5000));

		GArch::Nop(block, measureBlock.getSize());

		/*Size alignSize = block.getAlignSize<16>();
		GArch::Nop(block.subBlock(alignSize), alignSize);
		block.skipBytes(alignSize);*/

		

		GArch::Bound(block, X86::EAX, X86::Mem32<X86::OFFSET>(5000));
		GArch::Bound(block, X86::AX, X86::Mem32<X86::OFFSET>(5000));
		GArch::Stosb<>(block);
		GArch::Stosw<>(block);
		GArch::Stosd<>(block);
		GArch::Stosb<X86::REPE>(block);
		GArch::Stosw<X86::REPE>(block);
		GArch::Stosd<X86::REPE>(block);
		GArch::Stosb<X86::REPNE>(block);
		GArch::Stosw<X86::REPNE>(block);
		GArch::Stosd<X86::REPNE>(block);

		GArch::Sysenter(block);
		GArch::Sysexit(block);

		GArch::Imul(block, X86::EDX, X86::EAX);
		GArch::Imul(block, X86::DX, X86::AX);
		GArch::Imul(block, X86::EDX, X86::EAX, U32(500000));
		GArch::Imul(block, X86::EDX, X86::EAX, U8(5));
		GArch::Imul(block, X86::DX, X86::AX, U8(50));

		GArch::Imul(block, X86::EDX, X86::Mem32<X86::BASE>(X86::EDX), U32(500000));
		GArch::Imul(block, X86::EDX, X86::Mem32<X86::BASE_OFFSET>(X86::EDX, 100000), U32(500000));

		GArch::Xchg(block, X86::Mem32<X86::OFFSET>(5000), X86::EDX);
		//GArch::Xchg(block, X86::Mem32<X86::BASE>(X86::ECX), X86::EDX);
		//GArch::Xchg(block, X86::Mem32<X86::BASE_OFFSET>(X86::ECX, 5000), X86::EDX);
		GArch::Xchg(block, X86::Mem32<X86::INDEX_OFFSET>(X86::ECX, X86::SCALE_8, 5000), X86::EDX);
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
		GArch::Rsm(block);
#else
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

#endif
		block.invokeCdecl<int>();
	}

	return 0;
}
