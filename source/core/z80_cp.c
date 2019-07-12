#include "z80_instructions.h"

void z80_cp(struct z80 *z80, uint8_t n)
{
	uint8_t res = *z80->regs.a - n;
	z80_set_flags_default(z80, res, *z80->regs.a, n, 0, FLAG_S | FLAG_Z | FLAG_H | FLAG_PV | FLAG_C);
	z80_set_flag(z80, FLAG_X, n & (1 << 5));
	z80_set_flag(z80, FLAG_Y, n & (1 << 3));
	z80_set_flag(z80, FLAG_N, 1);
}

void z80_cp_R(struct z80 *z80)
{
	z80_cp(z80, *z80_regs_rrr(z80, z80_get_op(z80, 0)));
	INCPC(1);
}

void z80_cp_J(struct z80 *z80)
{
	z80_cp(z80, *z80_get_J(z80, z80_get_op(z80, 1)));
	INCPC(2);
}

void z80_cp_N(struct z80 *z80)
{
	z80_cp(z80, z80_get_op(z80, 1));
	INCPC(2);
}

void z80_cp_hl(struct z80 *z80)
{
	z80_cp(z80, z80->mem[z80->regs.hl]);
	INCPC(1);
}

void z80_cp_I_D(struct z80 *z80)
{
	z80_cp(z80, *z80_get_I_D(z80));
	INCPC(3);
}

void z80_cp_di(struct z80 *z80, int8_t hlinc)
{
	uint8_t tmp = z80->mem[z80->regs.hl];
	uint8_t fpv = z80_get_flag(z80, FLAG_PV);
	z80_cp(z80, tmp);
	z80->regs.bc--;
	z80->regs.hl += hlinc;
	z80_set_flag(z80, FLAG_X, (tmp - z80_get_flag(z80, FLAG_H)) & (1 << 1));
	z80_set_flag(z80, FLAG_X, (tmp - z80_get_flag(z80, FLAG_H)) & (1 << 3));
	if (z80->regs.bc)
		z80_set_flag(z80, FLAG_PV, 1);
	else
		z80_set_flag(z80, FLAG_PV, fpv);
	INCPC(2);
}

void z80_cpd(struct z80 *z80)
{
	z80_cp_di(z80, -1);
}

void z80_cp_di_r(struct z80 *z80, int8_t hlinc)
{
	z80_cp_di(z80, hlinc);
	if (z80->regs.bc && !z80_get_flag(z80, FLAG_Z))
		z80->pc -= 2;
}

void z80_cpdr(struct z80 *z80)
{
	z80_cp_di_r(z80, -1);
}

void z80_cpi(struct z80 *z80)
{
	z80_cp_di(z80, 1);
}

void z80_cpir(struct z80 *z80)
{
	z80_cp_di_r(z80, 1);
}

void z80_cpl(struct z80 *z80)
{
	uint8_t a = *z80->regs.a;
	*z80->regs.a = ~a;
	z80_set_flags_default(z80, *z80->regs.a, *z80->regs.a, a, 0, FLAG_X | FLAG_Y);
	z80_set_flag(z80, FLAG_H | FLAG_N, 1);
	INCPC(1);
}
