#include "z80_instructions.h"


void z80_ccf(struct z80 *z80)
{
	uint8_t cf = z80_get_flag(z80, FLAG_C);
	z80_set_flag(z80, FLAG_X, FLAG_X | *z80->regs.a);
	z80_set_flag(z80, FLAG_H, cf);
	z80_set_flag(z80, FLAG_Y, FLAG_Y | *z80->regs.a);
	z80_set_flag(z80, FLAG_N, 0);
	z80_set_flag(z80, FLAG_C, !cf);
	INCPC(1);
}

void z80_daa(struct z80 *z80)
{
	uint8_t *a = z80->regs.a;
	uint8_t tmp = *a;
	if (!z80_get_flag(z80, FLAG_N)) {
		if (z80_get_flag(z80, FLAG_H) || (*a & 0xF) > 0x09)
			tmp -= 0x06;
		if (z80_get_flag(z80, FLAG_C) || *a > 0x99)
			tmp -= 0x60;
	} else {
		if (z80_get_flag(z80, FLAG_H) || (*a & 0xF) > 0x09)
			tmp += 0x06;
		if (z80_get_flag(z80, FLAG_C) || *a > 0x99)
			tmp += 0x60;
	}
	z80_set_flags_default(z80, tmp, tmp, *a, 0, FLAG_S | FLAG_Z | FLAG_X | FLAG_Y | FLAG_PV | FLAG_PV_PARITY);
	z80_set_flag(z80, FLAG_C, z80_get_flag(z80, FLAG_C) | (*a > 0x99));
	z80_set_flag(z80, FLAG_H, bitval(*a, 4) ^ bitval(tmp, 4));
	*a = tmp;
	INCPC(1);
}

void z80_di(struct z80 *z80)
{
	z80->iff1 = z80->iff2 = 0;
	INCPC(1);
}

void z80_djnz_E(struct z80 *z80)
{
	(*z80->regs.b)--;
	if (*z80->regs.b)
		z80->pc += (int8_t)z80_get_op(z80, 1);
	else
		INCPC(2);
}

void z80_ei(struct z80 *z80)
{
	WARN("FIXME: enable with 1 instruction delay");
	z80->iff1 = z80->iff2 = 1;
	INCPC(1);
}

void z80_halt(struct z80 *z80)
{
	WARN("FIXME: wait for interrupt");
	INCPC(1);
}

void z80_neg(struct z80 *z80)
{
	*z80->regs.a = -*z80->regs.a;
	INCPC(2);
}

void z80_nop(struct z80 *z80)
{
	INCPC(1);
}

void z80_rst_S(struct z80 *z80)
{
	uint8_t op0 = z80_get_op(z80, 0);
	uint8_t s = (op0 >> 3) & 7;
	z80->sp -= 2;
	*((uint16_t *)&z80->mem[z80->sp]) = z80->pc;
	z80->pc = s;
}

void z80_scf(struct z80 *z80)
{
	z80_set_flag(z80, FLAG_X, z80_get_flag(z80, FLAG_X) | (*z80->regs.a & FLAG_X));
	z80_set_flag(z80, FLAG_Y, z80_get_flag(z80, FLAG_Y) | (*z80->regs.a & FLAG_Y));
	z80_set_flag(z80, FLAG_H | FLAG_N, 0);
	z80_set_flag(z80, FLAG_C, 1);
	INCPC(1);
}
