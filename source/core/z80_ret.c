#include "z80_instructions.h"

void z80_ret(struct z80 *z80)
{
	
	z80->pc = *(uint16_t *)&z80->mem[z80->sp];
	z80->sp += 2;
	if (z80->counters.ret == UINT64_MAX)
		WARN("counters.ret overflow");
	z80->counters.ret++;
}

void z80_ret_C(struct z80 *z80)
{
	if (z80_cond_ccc(z80))
		z80_ret(z80);
	else
		INCPC(1);
}

void z80_reti(struct z80 *z80)
{
	z80_ret(z80);
	z80->iff1 = z80->iff2;
	WARN("FIXME: check interrupts");
}

void z80_retn(struct z80 *z80)
{
	z80_reti(z80);
}