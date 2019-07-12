#include "z80_instructions.h"

void z80_out_N_a(struct z80 *z80)
{
	z80->cb_out(z80_get_op(z80, 1), *z80->regs.a);
	INCPC(2);
}

void z80_out_c_R(struct z80 *z80)
{
	uint8_t *r = z80_regs_rrr(z80, z80_get_op(z80, 0) >> 3);
	z80->cb_out(*z80->regs.c, *r);
	INCPC(2);
}

void z80_out_c_0(struct z80 *z80)
{
	z80->cb_out(*z80->regs.c, 0);
	INCPC(2);
}

void z80_outx(struct z80 *z80, int8_t n)
{
	uint16_t tmp = z80->mem[z80->regs.hl];
	z80->cb_out(*z80->regs.c, tmp);
	z80->regs.hl += n;
	(*z80->regs.b)--;
	uint16_t tmp2 = tmp + *z80->regs.l;
	z80_set_flags_default(z80, tmp, 0, 0, 0, FLAG_S | FLAG_Z | FLAG_X | FLAG_Y);
	z80_set_flag(z80, FLAG_N, bitval(tmp, 7));
	z80_set_flag(z80, FLAG_PV, !odd_parity((tmp2 & 7) ^ *z80->regs.b));
	z80_set_flag(z80, FLAG_H | FLAG_C, tmp2 > 255);
	INCPC(2);
}

void z80_otxr(struct z80 *z80, int8_t n)
{
	z80_outx(z80, n);
	if (*z80->regs.b)
		z80->pc -= 2;
}

void z80_outd(struct z80 *z80)
{
	z80_outx(z80, -1);
}

void z80_otdr(struct z80 *z80)
{
	z80_otxr(z80, -1);
}

void z80_outi(struct z80 *z80)
{
	z80_outx(z80, 1);
}

void z80_otir(struct z80 *z80)
{
	z80_otxr(z80, 1);
}
