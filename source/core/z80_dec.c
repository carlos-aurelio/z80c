#include "z80_instructions.h"

void z80_dec(struct z80 *z80, uint8_t *reg)
{
	uint8_t dst = *reg;
	uint8_t res = dst - 1;
	z80_set_flags_default(z80, *reg, dst, -1, 0, FLAG_S | FLAG_Z | FLAG_X | FLAG_H | FLAG_Y | FLAG_PV);
	z80_set_flag(z80, FLAG_N, 1);
	*reg = res;
}

void z80_dec_R(struct z80 *z80)
{
	uint8_t *r = z80_regs_rrr(z80, z80_get_op(z80, 0) >> 3);
	z80_dec(z80, r);
	INCPC(1);
}

void z80_dec_J(struct z80 *z80)
{
	uint8_t *j = z80_get_J(z80, z80_get_op(z80, 1) >> 3);
	z80_dec(z80, j);
	INCPC(2);
}

void z80_dec_hl(struct z80 *z80)
{
	uint8_t *hl = &z80->mem[z80->regs.hl];
	z80_dec(z80, hl);
	INCPC(1);
}

void z80_dec_I_D(struct z80 *z80)
{
	uint8_t *id = z80_get_I_D(z80);
	z80_dec(z80, id);
	INCPC(3);
}

void z80_dec_Q(struct z80 *z80)
{
	uint8_t o0 = z80_get_op(z80, 0);
	uint16_t *q = z80_regs_qq(z80, o0 >> 4);
	(*q)--;
	INCPC(1);
}

void z80_dec_I(struct z80 *z80)
{
	uint16_t *i = z80_regs_ixiy(z80);
	(*i)--;
	INCPC(2);
}
