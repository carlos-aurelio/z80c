#include "z80_instructions.h"

void z80_inx(struct z80 *z80, int8_t n)
{
	uint16_t tmp = z80->io[*z80->regs.c];
	z80->mem[z80->regs.hl] = tmp;
	z80->regs.hl += n;
	(*z80->regs.b)--;
	z80_set_flag(z80, FLAG_N, bitval(tmp, 7));
	uint16_t tmp2 = tmp + ((*z80->regs.c + n) & 0xFF);
	z80_set_flags_default(z80, tmp2, 0, 0, 0, FLAG_S | FLAG_Z | FLAG_X | FLAG_Y);
	z80_set_flag(z80, FLAG_PV, !odd_parity((tmp2 & 7) ^ *z80->regs.b));
	z80_set_flag(z80, FLAG_H, tmp2 > 255);
	z80_set_flag(z80, FLAG_C, tmp2 > 255);
	INCPC(2);
}

void z80_inxr(struct z80 *z80, int8_t n)
{
	z80_inx(z80, n);
	if (*z80->regs.b)
		z80->pc -= 2;
}

void z80_ind(struct z80 *z80)
{
	z80_inx(z80, -1);
}

void z80_indr(struct z80 *z80)
{
	z80_inxr(z80, -1);
}

void z80_ini(struct z80 *z80)
{
	z80_inx(z80, 1);
}

void z80_inir(struct z80 *z80)
{
	z80_inxr(z80, 1);
}
