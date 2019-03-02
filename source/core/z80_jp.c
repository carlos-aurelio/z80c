#include "z80_instructions.h"

void z80_jp_A(struct z80 *z80)
{
	z80->pc = z80_get_op16(z80, 1);
}

void z80_jp_hl(struct z80 *z80)
{
	z80->pc = z80->regs.hl;
}

void z80_jp_I(struct z80 *z80)
{
	uint16_t *i = z80_regs_ixiy(z80);
	z80->pc = *i;
}

void z80_jp_C_A(struct z80 *z80)
{
	if (z80_cond_ccc(z80))
		z80->pc = z80_get_op16(z80, 1);
	else
		INCPC(3);
}

void z80_jr_E(struct z80 *z80)
{
	z80->pc += (int8_t)z80_get_op(z80, 1);
}

void z80_jr_nz_E(struct z80 *z80)
{
	if (!z80_get_flag(z80, FLAG_Z))
		z80_jr_E(z80);
	else
		INCPC(2);
}

void z80_jr_z_E(struct z80 *z80)
{
	if (z80_get_flag(z80, FLAG_Z))
		z80_jr_E(z80);
	else
		INCPC(2);
}

void z80_jr_nc_E(struct z80 *z80)
{
	if (!z80_get_flag(z80, FLAG_C))
		z80_jr_E(z80);
	else
		INCPC(2);
}

void z80_jr_c_E(struct z80 *z80)
{
	if (z80_get_flag(z80, FLAG_C))
		z80_jr_E(z80);
	else
		INCPC(2);
}