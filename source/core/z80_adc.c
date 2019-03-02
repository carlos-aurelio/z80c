#include "z80_instructions.h"

void z80_adc_a(struct z80 *z80, uint8_t n)
{
	uint8_t a = *z80->regs.a;
	uint8_t c = z80_get_flag(z80, FLAG_C);
	z80_set_flags_default(z80, a + n + c, a, n, c, FLAG_S | FLAG_Z | FLAG_X | FLAG_H | FLAG_Y | FLAG_PV | FLAG_C);
	z80_set_flag(z80, FLAG_N, 0);
	*z80->regs.a = a + n + c;
}

void z80_adc_a_R(struct z80 *z80)
{
	uint8_t *r = z80_regs_rrr(z80, z80_get_op(z80, 1));
	z80_adc_a(z80, *r);
	INCPC(1);
}

void z80_adc_a_J(struct z80 *z80)
{
	z80_adc_a(z80, *z80_get_J(z80, z80_get_op(z80, 1)));
	INCPC(2);
}

void z80_adc_a_N(struct z80 *z80)
{
	z80_adc_a(z80, z80_get_op(z80, 1));
	INCPC(2);
}

void z80_adc_a_hl(struct z80 *z80)
{
	z80_adc_a(z80, z80->mem[z80->regs.hl]);
	INCPC(1);
}

void z80_adc_a_I_D(struct z80 *z80)
{
	z80_adc_a(z80, *z80_get_I_D(z80));
	INCPC(3);
}

void z80_adc_hl_Q(struct z80 *z80)
{
	z80_adx_hl_Q(z80, FLAG_S | FLAG_Z | FLAG_X | FLAG_H | FLAG_Y | FLAG_PV | FLAG_C);
	INCPC(2);
}