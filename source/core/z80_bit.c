#include "z80_instructions.h"

void z80_bit_B_R(struct z80 *z80)
{
	uint8_t op1 = z80_get_op(z80, 1);
	uint8_t *r = z80_regs_rrr(z80, op1);
	uint64_t b = (op1 >> 3) & mask(3);
	uint8_t tmp = *r & (1 << b);
	z80_set_flags_default(z80, tmp, tmp, *r, 0, FLAG_S | FLAG_Z | FLAG_X | FLAG_Y | FLAG_PV | FLAG_PV_PARITY);
	z80_set_flag(z80, FLAG_H, 1);
	z80_set_flag(z80, FLAG_N, 0);
	INCPC(2);
}

void z80_bit_B_hl(struct z80 *z80)
{
	uint8_t op1 = z80_get_op(z80, 1);
	uint64_t b = (op1 >> 3) & mask(3);
	uint8_t tmp = z80->mem[z80->regs.hl] & (1 << b);
	z80_set_flags_default(z80, tmp, tmp, z80->mem[z80->regs.hl], 0, FLAG_S | FLAG_Z | FLAG_PV | FLAG_PV_PARITY);
	/* TODO: confirm X and Y flags */
	z80_set_flag(z80, FLAG_X, z80->regs.hl & (1 << 13));
	z80_set_flag(z80, FLAG_H, 1);
	z80_set_flag(z80, FLAG_Y, z80->regs.hl & (1 << 11));
	z80_set_flag(z80, FLAG_N, 0);
	INCPC(2);
}

void z80_bit_B_I_D(struct z80 *z80)
{
	uint8_t b = (z80_get_op(z80, 3) >> 3) & 7;
	uint8_t *id = z80_get_I_D(z80);
	uint8_t tmp = *id & (1 << b);
	z80_set_flags_default(z80, tmp, tmp, *id, 0, FLAG_S | FLAG_Z | FLAG_PV | FLAG_PV_PARITY);
	/* TODO: confirm X and Y flags */
	z80_set_flag(z80, FLAG_X, (uint16_t)(id - z80->mem) & (1 << 13));
	z80_set_flag(z80, FLAG_H, 1);
	z80_set_flag(z80, FLAG_Y, (uint16_t)(id - z80->mem) & (1 << 11));
	z80_set_flag(z80, FLAG_N, 0);
	INCPC(4);
}
