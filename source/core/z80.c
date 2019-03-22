#include <stdlib.h>
#include <string.h>

#include "z80_instructions.h"

void z80_init(struct z80 *z80, uint8_t *mem, uint8_t *io)
{
	memset(z80, 0, sizeof(*z80));
	z80->regs.a = (uint8_t *)&z80->regs.af;
	z80->regs.f = z80->regs.a + 1;
	z80->regs.b = (uint8_t *)&z80->regs.bc;
	z80->regs.c = z80->regs.b + 1;
	z80->regs.d = (uint8_t *)&z80->regs.de;
	z80->regs.e = z80->regs.d + 1;
	z80->regs.h = (uint8_t *)&z80->regs.hl;
	z80->regs.l = z80->regs.h + 1;

	z80->regs_alt.a = (uint8_t *)&z80->regs_alt.af;
	z80->regs_alt.f = z80->regs_alt.a + 1;
	z80->regs_alt.b = (uint8_t *)&z80->regs_alt.bc;
	z80->regs_alt.c = z80->regs_alt.b + 1;
	z80->regs_alt.d = (uint8_t *)&z80->regs_alt.de;
	z80->regs_alt.e = z80->regs_alt.d + 1;
	z80->regs_alt.h = (uint8_t *)&z80->regs_alt.hl;
	z80->regs_alt.l = z80->regs_alt.h + 1;

	z80->ixh = (uint8_t *)&z80->ix;
	z80->ixl = z80->ixh + 1;
	z80->iyh = (uint8_t *)&z80->iy;
	z80->iyl = z80->iyh + 1;
	z80->io = io;
	z80->mem = mem;
	z_init_opcode_table(z80);
}

void z80_step(struct z80 *z80)
{
	z_base(z80);
}

void z80_set_flag(struct  z80 *z80, uint8_t flag, uint32_t set_reset)
{
	if (set_reset)
		*z80->regs.f |= flag;
	else
		*z80->regs.f &= ~flag;
}

uint8_t z80_get_flag(struct z80 *z80, uint8_t flag)
{
	if (*z80->regs.f & flag)
		return 1;
	else
		return 0;
}

uint8_t z80_get_op(struct z80 *z80, int8_t idx)
{
	return z80->mem[z80->pc + idx];
}

uint16_t z80_get_op16(struct z80 *z80, uint8_t idx)
{
	return (uint16_t)z80->mem[z80->pc + idx] | ((uint16_t)z80->mem[z80->pc + idx + 1] << 8);
}

uint8_t *z80_regs_rrr(struct z80 *z80, uint8_t rrr)
{
	switch (rrr & 7) {
		case 7: return z80->regs.a;
		case 0: return z80->regs.b;
		case 1: return z80->regs.c;
		case 2: return z80->regs.d;
		case 3: return z80->regs.e;
		case 4: return z80->regs.h;
		case 5: return z80->regs.l;
		case 6: return &z80->mem[z80->regs.hl];
		default: return NULL;
	}
}

uint16_t *z80_regs_ixiy(struct z80 *z80)
{
	uint8_t o0 = z80_get_op(z80, 0);
	if ((o0 >> 5) & 1)
		return &z80->iy;
	else
		return &z80->ix;
}

uint8_t *z80_regs_half_ixiy(struct z80 *z80, uint8_t b)
{
	uint8_t *reg = (uint8_t *)z80_regs_ixiy(z80);
	return reg + (b & 1);
}

uint16_t *z80_regs_qq(struct z80 *z80, uint8_t qq)
{
	uint16_t *regs[] = { &z80->regs.bc, &z80->regs.de, &z80->regs.hl, &z80->sp };
	switch (z80->mem[z80->pc]) {
		case 0xDD:
			regs[2] = &z80->ix;
			break;
		case 0xFD:
			regs[2] = &z80->iy;
			break;
		default:
			regs[2] = &z80->regs.hl;
			break;
	}
	return regs[qq & 3];
}

uint16_t *z80_regs_pp(struct z80 *z80)
{
	uint16_t *regs[] = { &z80->regs.bc, &z80->regs.de, &z80->regs.hl, &z80->regs.af };
	uint8_t pp = (z80_get_op(z80, 0) >> 4) & 3;
	return regs[pp];
}

uint8_t z80_cond_ccc(struct z80 *z80)
{
	uint8_t ccc = (z80_get_op(z80, 0) >> 3) & 7;
	uint8_t conds[] = { FLAG_Z, FLAG_C, FLAG_PV, FLAG_S };
	uint8_t flag = conds[ccc >> 1];
	uint8_t v = z80_get_flag(z80, flag);
	if (ccc & 1)
		return v;
	else
		return !v;
}

void z80_set_flags_default(struct z80 *z80, uint16_t res, uint16_t dst, uint16_t src, uint16_t cf, uint16_t flags)
{
	if (flags & FLAG_S)
		z80_set_flag(z80, FLAG_S, res & (1 << 7));
	if (flags & FLAG_Z)
		z80_set_flag(z80, FLAG_Z, res == 0);
	if (flags & FLAG_X)
		z80_set_flag(z80, FLAG_X, res & (1 << 5));
	if (flags & FLAG_H)
		z80_set_flag(z80, FLAG_H, sum_carry(dst, src, cf, 4));
	if (flags & FLAG_Y)
		z80_set_flag(z80, FLAG_Y, res & (1 << 3));
	if (flags & FLAG_PV) {
		if (flags & FLAG_PV_PARITY)
			z80_set_flag(z80, FLAG_PV, !odd_parity(res));
		else
			z80_set_flag(z80, FLAG_PV, sum_overflow(dst, src, cf, 8));
	}
	if (flags & FLAG_N)
		z80_set_flag(z80, FLAG_C, sum_carry(dst, src, cf, 8));
}

uint8_t *z80_get_J(struct z80 *z80, uint8_t b)
{
	uint8_t *j = z80_regs_half_ixiy(z80, b);
	return j;
}

uint8_t *z80_get_I_D(struct z80 *z80)
{
	uint16_t *i = z80_regs_ixiy(z80);
	int8_t d = (int8_t)z80_get_op(z80, 2);
	return &z80->mem[*i + d];
}

void z80_adx_hl_Q(struct z80 *z80, uint8_t flags)
{
	uint8_t o0 = z80_get_op(z80, 0);
	uint16_t *q = z80_regs_qq(z80, o0 >> 4);
	uint16_t hl = z80->regs.hl;
	uint8_t c = z80_get_flag(z80, FLAG_C);
	z80_set_flags_default(z80, hl + *q + c, hl, *q, c, flags);
	z80_set_flag(z80, FLAG_N, 0);
	z80->regs.hl = hl + *q + c;
}
