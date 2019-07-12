#include "z80_instructions.h"

void z80_set_B_(struct z80 *z80, uint8_t *dst, uint8_t op_idx)
{
	uint8_t op = z80_get_op(z80, op_idx);
	uint8_t b = (op >> 3) & 7;
	*dst |= (1 << b);
}

void z80_set_B_R(struct z80 *z80)
{
	uint8_t op1 = z80_get_op(z80, 1);
	uint8_t *r = z80_regs_rrr(z80, op1);
	z80_set_B_(z80, r, 1);
	INCPC(2);
}

void z80_set_B_hl(struct z80 *z80)
{
	uint8_t *hl = &z80->mem[z80->regs.hl];
	z80_set_B_(z80, hl, 1);
	INCPC(2);
}

void z80_set_B_I_D(struct z80 *z80)
{
	uint8_t *id = z80_get_I_D(z80);
	z80_set_B_(z80, id, 3);
	INCPC(4);
}

void z80_set_B_I_D_R(struct z80 *z80)
{
	uint8_t op3 = z80_get_op(z80, 3);
	uint8_t *r = z80_regs_rrr(z80, op3);
	uint8_t *id = z80_get_I_D(z80);
	z80_set_B_(z80, id, 3);
	*r = *id;
	INCPC(4);
}
