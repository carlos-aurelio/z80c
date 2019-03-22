#include "z80_instructions.h"

void z80_push(struct z80 *z80, uint16_t *reg)
{
	z80->sp -= 2;
	*((uint16_t *)&z80->mem[z80->sp]) = *reg;
	if (z80->counters.push == UINT64_MAX)
		WARN("counters.push overflow");
	z80->counters.push++;
}

void z80_push_P(struct z80 *z80)
{
	z80_push(z80, z80_regs_pp(z80));
	INCPC(1);
}

void z80_push_I(struct z80 *z80)
{
	z80_push(z80, z80_regs_ixiy(z80));
	INCPC(2);
}