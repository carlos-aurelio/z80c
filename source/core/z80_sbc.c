#include "z80_instructions.h"

void z80_sbc_(struct z80 *z80, uint8_t *dst, uint8_t *src)
{
	uint8_t cf = z80_get_flag(z80, FLAG_C);
	uint8_t res = *dst - (*src + cf);
	z80_set_flags_default(z80, res, *dst, ~((uint16_t)*src) + 1, cf, FLAG_S | FLAG_Z | FLAG_X | FLAG_H | FLAG_Y | FLAG_PV | FLAG_C);
	z80_set_flag(z80, FLAG_N, 1);
	*dst = res;
}

void z80_sbc_a_(struct z80 *z80, uint8_t *src)
{
	z80_sbc_(z80, z80->regs.a, src);
}

void z80_sbc_a_R(struct z80 *z80)
{
	uint8_t op0 = z80_get_op(z80, 0);
	uint8_t *r = z80_regs_rrr(z80, op0);
	z80_sbc_a_(z80, r);
	INCPC(1);
}

void z80_sbc_a_J(struct z80 *z80)
{
	uint8_t *j = z80_get_J(z80, z80_get_op(z80, 1));
	z80_sbc_a_(z80, j);
	INCPC(2);
}

void z80_sbc_a_N(struct z80 *z80)
{
	uint8_t n = z80_get_op(z80, 1);
	z80_sbc_a_(z80, &n);
	INCPC(2);
}

void z80_sbc_a_hl(struct z80 *z80)
{
	uint8_t *hl = &z80->mem[z80->regs.hl];
	z80_sbc_a_(z80, hl);
	INCPC(1);
}

void z80_sbc_a_I_D(struct z80 *z80)
{
	uint8_t *id = z80_get_I_D(z80);
	z80_sbc_a_(z80, id);
	INCPC(3);
}

void z80_sbc_hl_Q(struct z80 *z80)
{
	uint8_t op1 = z80_get_op(z80, 1);
	uint16_t *q = z80_regs_qq(z80, op1 >> 4);
	uint16_t cf = z80_get_flag(z80, FLAG_C);
	uint16_t *hl = &z80->regs.hl;
	uint16_t res = *hl - (*q + cf);
	z80_set_flags_default(z80, res, *hl, ~(*q) + 1, cf, FLAG_S | FLAG_Z | FLAG_X | FLAG_H | FLAG_Y | FLAG_PV | FLAG_C);
	z80_set_flag(z80, FLAG_N, 1);
	*hl = res;
	INCPC(2);
}
