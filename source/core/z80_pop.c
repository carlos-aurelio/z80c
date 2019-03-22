#include "z80_instructions.h"

void z80_pop(struct z80 *z80, uint16_t *reg)
{
	*reg = *((uint16_t *)&z80->mem[z80->sp]);
	z80->sp += 2;
	if (z80->counters.pop == UINT64_MAX)
		WARN("counters.pop overflow");
	z80->counters.pop++;
}

void z80_pop_P(struct z80 *z80)
{
	z80_pop(z80, z80_regs_pp(z80));
	INCPC(1);
}

void z80_pop_I(struct z80 *z80)
{
	z80_pop(z80, z80_regs_ixiy(z80));
	INCPC(2);
}