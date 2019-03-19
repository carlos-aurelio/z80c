#include "z80_instructions.h"

void z80_call_A(struct z80 *z80)
{
	z80->sp -= 2;
	*((uint16_t *)&z80->mem[z80->sp]) = z80->pc + 3;
	z80->pc = z80_get_op16(z80, 1);
}

void z80_call_C_A(struct z80 *z80)
{
	if (z80_cond_ccc(z80))
		z80_call_A(z80);
	else
		INCPC(3);
}