#include "z80_instructions.h"

void z80_xor_(struct z80 *z80, uint8_t *src)
{
	uint8_t res = *z80->regs.a ^ *src;
	z80_set_flags_default(z80, res, *z80->regs.a, *src, 0, FLAG_S | FLAG_Z | FLAG_X | FLAG_Y | FLAG_PV | FLAG_PV_PARITY);
	z80_set_flag(z80, FLAG_H | FLAG_N | FLAG_C, 0);
	*z80->regs.a = res;
}

void z80_xor_R(struct z80 *z80)
{
	uint8_t o0 = z80_get_op(z80, 0);
	uint8_t *r = z80_regs_rrr(z80, o0);
	z80_xor_(z80, r);
	INCPC(1);
}

void z80_xor_J(struct z80 *z80)
{
	uint8_t o1 = z80_get_op16(z80, 1);
	uint8_t *j = z80_get_J(z80, o1);
	z80_xor_(z80, j);
	INCPC(2);
}

void z80_xor_N(struct z80 *z80)
{
	uint8_t o1 = z80_get_op(z80, 1);
	z80_xor_(z80, &o1);
	INCPC(2);
}

void z80_xor_hl(struct z80 *z80)
{
	uint8_t *hl = &z80->mem[z80->regs.hl];
	z80_xor_(z80, hl);
	INCPC(1);
}

void z80_xor_I_D(struct z80 *z80)
{
	uint8_t *id = z80_get_I_D(z80);
	z80_xor_(z80, id);
	INCPC(3);
}
