#include "z80_instructions.h"

void z80_add_a_R(struct z80 *z80)
{
	z80_set_flag(z80, FLAG_C, 0);
	z80_adc_a_R(z80);
}

void z80_add_a_J(struct z80 *z80)
{
	z80_set_flag(z80, FLAG_C, 0);
	z80_adc_a_J(z80);
}

void z80_add_a_N(struct z80 *z80)
{
	z80_set_flag(z80, FLAG_C, 0);
	z80_adc_a_N(z80);
}

void z80_add_a_hl(struct z80 *z80)
{
	z80_set_flag(z80, FLAG_C, 0);
	z80_adc_a_hl(z80);
}

void z80_add_a_I_D(struct z80 *z80)
{
	z80_set_flag(z80, FLAG_C, 0);
	z80_adc_a_I_D(z80);
}

void z80_add_hl_Q(struct z80 *z80)
{
	z80_set_flag(z80, FLAG_C, 0);
	z80_adx_hl_Q(z80, FLAG_X | FLAG_H | FLAG_Y | FLAG_C);
	INCPC(1);
}

void z80_add_I_Q(struct z80 *z80)
{

	uint8_t o0 = z80_get_op(z80, 0);
	uint16_t *q = z80_regs_qq(z80, o0 >> 4);
	uint16_t *i = z80_regs_ixiy(z80);
	uint16_t res = *i + *q;
	z80_set_flags_default(z80, res, res, *i, *q, FLAG_X | FLAG_H | FLAG_Y | FLAG_C);
	*i = res;
	INCPC(2);
}
