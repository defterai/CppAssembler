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

#include "asm/os/host.h"

using namespace CppAsm;

#if CPP_ASM_HOST_ARCH_BITS == 64
	typedef Arch::CustomArch<X64::i386> testArch;
#else
	typedef Arch::CustomArch<X86::i686, X86::SEP, X86::CLFSH> testArch;
#endif

int main()
{
	Host::CodeBlock block(1024);

	{
#if CPP_ASM_HOST_ARCH_BITS == 64
		testArch::Xor(block, X64::RAX, X64::RAX);
		testArch::Lea<X64::BASE_INDEX_OFFSET>(block, X64::RAX, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::RAX, X64::RAX, X64::SCALE_8, 5000));
		testArch::Ret(block);
		
		Host::MeasureBlock measureBlock = block.subBlock<Host::MeasureBlock>(1024);
		testArch::Movsxd(measureBlock, X64::RDX, X64::EDX);
		
#else
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
		testArch::Nop(measureBlock);
		testArch::Lds(measureBlock, X86::EDX, X86::Mem32<X86::OFFSET>(5000));
		testArch::Les(measureBlock, X86::EDX, X86::Mem32<X86::OFFSET>(5000));
		testArch::Lgs(measureBlock, X86::EDX, X86::Mem32<X86::OFFSET>(5000));
		testArch::Lfs(measureBlock, X86::EDX, X86::Mem32<X86::OFFSET>(5000));
		testArch::Lss(measureBlock, X86::EDX, X86::Mem32<X86::OFFSET>(5000));
		testArch::Lds(measureBlock, X86::DX, X86::Mem32<X86::OFFSET>(5000));
		testArch::Les(measureBlock, X86::DX, X86::Mem32<X86::OFFSET>(5000));
		testArch::Lgs(measureBlock, X86::DX, X86::Mem32<X86::OFFSET>(5000));
		testArch::Lfs(measureBlock, X86::DX, X86::Mem32<X86::OFFSET>(5000));
		testArch::Lss(measureBlock, X86::DX, X86::Mem32<X86::OFFSET>(5000));

		testArch::Nop(block, measureBlock.getSize());

		/*Size alignSize = block.getAlignSize<16>();
		testArch::Nop(block.subBlock(alignSize), alignSize);
		block.skipBytes(alignSize);*/

		

		testArch::Bound(block, X86::EAX, X86::Mem32<X86::OFFSET>(5000));
		testArch::Bound(block, X86::AX, X86::Mem32<X86::OFFSET>(5000));
		testArch::Stosb<>(block);
		testArch::Stosw<>(block);
		testArch::Stosd<>(block);
		testArch::Stosb<X86::REPE>(block);
		testArch::Stosw<X86::REPE>(block);
		testArch::Stosd<X86::REPE>(block);
		testArch::Stosb<X86::REPNE>(block);
		testArch::Stosw<X86::REPNE>(block);
		testArch::Stosd<X86::REPNE>(block);

		testArch::Sysenter(block);
		testArch::Sysexit(block);

		testArch::Imul(block, X86::EDX, X86::EAX);
		testArch::Imul(block, X86::DX, X86::AX);
		testArch::Imul(block, X86::EDX, X86::EAX, U32(500000));
		testArch::Imul(block, X86::EDX, X86::EAX, U8(5));
		testArch::Imul(block, X86::DX, X86::AX, U8(50));

		testArch::Imul(block, X86::EDX, X86::Mem32<X86::BASE>(X86::EDX), U32(500000));
		testArch::Imul(block, X86::EDX, X86::Mem32<X86::BASE_OFFSET>(X86::EDX, 100000), U32(500000));

		testArch::Xchg(block, X86::Mem32<X86::OFFSET>(5000), X86::EDX);
		//testArch::Xchg(block, X86::Mem32<X86::BASE>(X86::ECX), X86::EDX);
		//testArch::Xchg(block, X86::Mem32<X86::BASE_OFFSET>(X86::ECX, 5000), X86::EDX);
		testArch::Xchg(block, X86::Mem32<X86::INDEX_OFFSET>(X86::ECX, X86::SCALE_8, 5000), X86::EDX);
		testArch::Xchg(block, X86::Mem32<X86::BASE_INDEX>(X86::EAX, X86::ECX, X86::SCALE_8), X86::EDX);
		testArch::Xchg(block, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::EAX, X86::ECX, X86::SCALE_8, 10), X86::EDX);
		testArch::Xchg(block, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::EAX, X86::ECX, X86::SCALE_8, 1000), X86::EDX);
		
		testArch::Lea(block, X86::EDX, X86::Mem32<X86::OFFSET>(5000));
		testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE>(X86::ECX));
		testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_OFFSET>(X86::ECX, 50));
		testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_OFFSET>(X86::ECX, 5000));
		testArch::Lea(block, X86::EDX, X86::Mem32<X86::INDEX_OFFSET>(X86::ECX, X86::SCALE_8, 5000));
		testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_INDEX>(X86::EAX, X86::ECX, X86::SCALE_8));
		testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::EAX, X86::ECX, X86::SCALE_8, 10));
		testArch::Lea(block, X86::EDX, X86::Mem32<X86::BASE_INDEX_OFFSET>(X86::EAX, X86::ECX, X86::SCALE_8, 1000));

		testArch::F2xm1(block);
		testArch::Fcom<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EAX));
		testArch::Fcom<X86::QWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EAX));
		testArch::Fadd(block, X86::ST, X86::ST6);
		testArch::Fadd(block, X86::ST6, X86::ST);
		testArch::Faddp(block, X86::ST6, X86::ST);
		testArch::Fiadd<X86::DWORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EAX));
		testArch::Fiadd<X86::WORD_PTR>(block, X86::Mem32<X86::BASE>(X86::EAX));
		testArch::Fcomi(block, X86::ST, X86::ST6);
		testArch::Fcomip(block, X86::ST, X86::ST6);
		testArch::Fucomi(block, X86::ST, X86::ST6);
		testArch::Fucomip(block, X86::ST, X86::ST6);
		testArch::Ret(block);
		testArch::CmpXchg8b<>(block, X86::Mem32<X86::BASE>(X86::EAX));
		testArch::Rdtsc(block);
		testArch::Rdmsr(block);
		testArch::Wrmsr(block);
		testArch::Rsm(block);
/*#else
		testArch::Lea(block, X64::EDX, X64::Mem64<X64::OFFSET>(0xFFFFFFFF));
		//testArch::Movsx(block, X64::DX, X64::Mem64<X64::BASE>(X64::RSP));

		testArch::Lea<X64::BASE_INDEX_OFFSET>(block, X64::RDX, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::R14, X64::R14, X64::SCALE_8, 5000));
		testArch::Lea<X64::BASE_INDEX_OFFSET>(block, X64::EDX, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::R14, X64::R14, X64::SCALE_8, 5000));
		testArch::Lea<X64::BASE_INDEX_OFFSET>(block, X64::DX, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::R14, X64::R14, X64::SCALE_8, 5000));
		testArch::Lea<X64::BASE_INDEX_OFFSET>(block, X64::RDX, X64::Mem32<X64::BASE_INDEX_OFFSET>(X64::R14d, X64::R14d, X64::SCALE_8, 5000));
		testArch::Lea<X64::BASE_INDEX_OFFSET>(block, X64::EDX, X64::Mem32<X64::BASE_INDEX_OFFSET>(X64::R14d, X64::R14d, X64::SCALE_8, 5000));
		testArch::Lea<X64::BASE_INDEX_OFFSET>(block, X64::DX, X64::Mem32<X64::BASE_INDEX_OFFSET>(X64::R14d, X64::R14d, X64::SCALE_8, 5000));

		testArch::Movsxd(block, X64::RDX, X64::EDX);
		testArch::Movsxd(block, X64::RDX, X64::Mem64<X64::BASE>(X64::RAX));
		testArch::Movsxd(block, X64::RDX, X64::Mem32<X64::BASE>(X64::EAX));

		testArch::Movzx(block, X64::EDX, X64::DX);
		testArch::Movzx(block, X64::DX, X64::DX);
		testArch::Movzx<X64::WORD_PTR>(block, X64::EDX, X64::Mem64<X64::BASE>(X64::RAX));
		testArch::Movzx<X64::BYTE_PTR>(block, X64::EDX, X64::Mem32<X64::BASE>(X64::EAX));
		testArch::Movzx(block, X64::DX, X64::DL);
		testArch::Movzx<X64::WORD_PTR>(block, X64::DX, X64::Mem64<X64::BASE>(X64::RAX));
		testArch::Movzx<X64::BYTE_PTR>(block, X64::DX, X64::Mem32<X64::BASE>(X64::EAX));

		testArch::Cmp<X64::BYTE_PTR>(block, X64::Mem64<X64::BASE>(X64::R14), S8(0x32));
		testArch::Cmp<X64::WORD_PTR>(block, X64::Mem64<X64::BASE>(X64::R14), S16(0x32));
		testArch::Cmp<X64::DWORD_PTR>(block, X64::Mem64<X64::BASE>(X64::R14), S32(0x32));
		testArch::Cmp<X64::QWORD_PTR>(block, X64::Mem64<X64::BASE>(X64::R14), S32(0x32));
		testArch::Cmp<X64::BYTE_PTR>(block, X64::Mem32<X64::BASE>(X64::R14d), S8(0x32));
		testArch::Cmp<X64::WORD_PTR>(block, X64::Mem32<X64::BASE>(X64::R14d), S16(0x32));
		testArch::Cmp<X64::DWORD_PTR>(block, X64::Mem32<X64::BASE>(X64::R14d), S32(0x32));
		testArch::Cmp<X64::QWORD_PTR>(block, X64::Mem32<X64::BASE>(X64::R14d), S32(0x32));

		testArch::Cmp<X64::BASE>(block, X64::Mem64<X64::BASE>(X64::R14), X64::EAX);
		testArch::Cmp<X64::BASE>(block, X64::Mem64<X64::BASE>(X64::R14), X64::AX);
		testArch::Cmp<X64::BASE>(block, X64::Mem64<X64::BASE>(X64::R14), X64::AL);
		testArch::Cmp<X64::BASE_OFFSET>(block, X64::Mem64<X64::BASE_OFFSET>(X64::RSP, 5000), X64::R14);
		testArch::Cmp<X64::BASE_OFFSET>(block, X64::Mem64<X64::BASE_OFFSET>(X64::RSP, 5000), X64::RSP);
		testArch::Cmp<X64::BASE_OFFSET>(block, X64::Mem64<X64::BASE_OFFSET>(X64::R14, 5000), X64::EAX);
		testArch::Cmp<X64::BASE_OFFSET>(block, X64::Mem64<X64::BASE_OFFSET>(X64::R14, 5000), X64::AX);
		testArch::Cmp<X64::BASE_OFFSET>(block, X64::Mem64<X64::BASE_OFFSET>(X64::R14, 5000), X64::AL);
		testArch::Cmp<X64::INDEX_OFFSET>(block, X64::Mem64<X64::INDEX_OFFSET>(X64::R14, X64::SCALE_8, 5000), X64::R14);
		testArch::Cmp<X64::INDEX_OFFSET>(block, X64::Mem64<X64::INDEX_OFFSET>(X64::R14, X64::SCALE_8, 5000), X64::RSP);
		testArch::Cmp<X64::INDEX_OFFSET>(block, X64::Mem64<X64::INDEX_OFFSET>(X64::R14, X64::SCALE_8, 5000), X64::EAX);
		testArch::Cmp<X64::INDEX_OFFSET>(block, X64::Mem64<X64::INDEX_OFFSET>(X64::R14, X64::SCALE_8, 5000), X64::AX);
		testArch::Cmp<X64::INDEX_OFFSET>(block, X64::Mem64<X64::INDEX_OFFSET>(X64::R14, X64::SCALE_8, 5000), X64::AL);
		testArch::Cmp<X64::BASE_INDEX>(block, X64::Mem64<X64::BASE_INDEX>(X64::R14, X64::RAX, X64::SCALE_8), X64::R14);
		testArch::Cmp<X64::BASE_INDEX>(block, X64::Mem64<X64::BASE_INDEX>(X64::R14, X64::R14, X64::SCALE_8), X64::RSP);
		testArch::Cmp<X64::BASE_INDEX>(block, X64::Mem64<X64::BASE_INDEX>(X64::R14, X64::R14, X64::SCALE_8), X64::EAX);
		testArch::Cmp<X64::BASE_INDEX>(block, X64::Mem64<X64::BASE_INDEX>(X64::R14, X64::R14, X64::SCALE_8), X64::AX);
		testArch::Cmp<X64::BASE_INDEX>(block, X64::Mem64<X64::BASE_INDEX>(X64::R14, X64::R14, X64::SCALE_8), X64::AL);
		testArch::Cmp<X64::BASE_INDEX_OFFSET>(block, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::RAX, X64::RAX, X64::SCALE_8, 5000), X64::R14);
		testArch::Cmp<X64::BASE_INDEX_OFFSET>(block, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::RDX, X64::R14, X64::SCALE_8, 5000), X64::RSP);
		testArch::Cmp<X64::BASE_INDEX_OFFSET>(block, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::R14, X64::R14, X64::SCALE_8, 5000), X64::EAX);
		testArch::Cmp<X64::BASE_INDEX_OFFSET>(block, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::R14, X64::R14, X64::SCALE_8, 5000), X64::AX);
		testArch::Cmp<X64::BASE_INDEX_OFFSET>(block, X64::Mem64<X64::BASE_INDEX_OFFSET>(X64::R14, X64::R14, X64::SCALE_8, 5000), X64::AL);

#endif*/
#endif
		
		int result = block.invokeCdecl<int>();
		std::cout << "Result: " << result << std::endl;
	}

	return 0;
}
