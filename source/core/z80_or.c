#include "z80_instructions.h"

void z80_or(struct z80 *z80, uint8_t n)
{
	uint8_t *a = z80->regs.a;
	*a |= n;
	z80_set_flags_default(z80, *a, 0, 0, 0, FLAG_S | FLAG_Z | FLAG_X | FLAG_Y | FLAG_PV | FLAG_PV_PARITY);
	z80_set_flag(z80, FLAG_H | FLAG_N | FLAG_C, 0);
}

void z80_or_R(struct z80 *z80)
{
	uint8_t o0 = z80_get_op(z80, 0);
	uint8_t *r = z80_regs_rrr(z80, o0);
	z80_or(z80, *r);
	INCPC(1);
}

void z80_or_J(struct z80 *z80)
{
	uint8_t *j = z80_get_J(z80, z80_get_op(z80, 0));
	z80_or(z80, *j);
	INCPC(2);
}

void z80_or_N(struct z80 *z80)
{
	z80_or(z80, z80_get_op(z80, 1));
	INCPC(2);
}

void z80_or_hl(struct z80 *z80)
{
	z80_or(z80, z80->mem[z80->regs.hl]);
	INCPC(1);
}

void z80_or_I_D(struct z80 *z80)
{
	uint8_t *id = z80_get_I_D(z80);
	z80_or(z80, *id);
	INCPC(3);
}
