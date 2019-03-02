#include "z80_instructions.h"

/*

rl:		ocf := cf, cf := a.7, a := [a << 1] + ocf
rlc:			   cf := a.7, a := [a << 1] + cf

rr:		ocf := cf, cf := a.0, a := [a >> 1] + [ocf << 7]
rrc:			   cf := a.0, a := [a >> 1] + [cf << 7]

*/
void z80_rlrx_(struct z80 *z80, uint8_t *r, uint8_t c, uint8_t rr)
{
	uint8_t ocf = 0;
	
	if (!c)	/* rl or rr */
		z80_get_flag(z80, FLAG_C);

	if (rr) /* rr or rrc */
		z80_set_flag(z80, FLAG_C, bitval(*r, 7));
	else	/* rl or rlc */
		z80_set_flag(z80, FLAG_C, bitval(*r, 0));

	if (c)	/* rl or rr */
		ocf = z80_get_flag(z80, FLAG_C);

	if (rr)	/* rr or rrc */
		*r = (*r >> 1) + (ocf << 7);
	else	/* rl or rlc */
		*r = (*r << 1) + ocf;

	uint8_t flagdef = FLAG_X | FLAG_Y;
	if (r != z80->regs.a) /* rla, rlca, rra or rrca */
		flagdef |= FLAG_S | FLAG_Z | FLAG_PV | FLAG_PV_PARITY;
	z80_set_flags_default(z80, *r, 0, 0, 0, flagdef);
}

void z80_rlrx_a(struct z80 *z80, uint8_t c, uint8_t rr)
{
	z80_rlrx_(z80, z80->regs.a, c, rr);
	INCPC(1);
}

void z80_rlrx_R(struct z80 *z80, uint8_t c, uint8_t rr)
{
	uint8_t op1 = z80_get_op(z80, 1);
	z80_rlrx_(z80, z80_regs_rrr(z80, op1), c, rr);
	INCPC(2);
}

void z80_rlrx_hl(struct z80 *z80, uint8_t c, uint8_t rr)
{
	uint8_t *hl = &z80->mem[z80->regs.hl];
	z80_rlrx_(z80, hl, c, rr);
	INCPC(2);
}

void z80_rlrx_I_D(struct z80 *z80, uint8_t c, uint8_t rr)
{
	uint8_t *id = z80_get_I_D(z80);
	z80_rlrx_(z80, id, c, rr);
	INCPC(4);
}

void z80_rlrx_I_D_R(struct z80 *z80, uint8_t c, uint8_t rr)
{
	uint8_t *id = z80_get_I_D(z80);
	uint8_t op3 = z80_get_op(z80, 3);
	uint8_t *r = z80_regs_rrr(z80, op3);
	z80_rlrx_(z80, id, c, rr);
	*r = *id;
	INCPC(4);
}

void z80_rla(struct z80 *z80)
{
	z80_rlrx_a(z80, 0, 0);
}

void z80_rl_R(struct z80 *z80)
{
	z80_rlrx_R(z80, 0, 0);
}

void z80_rl_hl(struct z80 *z80)
{
	z80_rlrx_hl(z80, 0, 0);
}

void z80_rl_I_D(struct z80 *z80)
{
	z80_rlrx_I_D(z80, 0, 0);
}

void z80_rl_I_D_R(struct z80 *z80)
{
	z80_rlrx_I_D_R(z80, 0, 0);
}

void z80_rlca(struct z80 *z80)
{
	z80_rlrx_a(z80, 1, 0);
}

void z80_rlc_R(struct z80 *z80)
{
	z80_rlrx_R(z80, 1, 0);
}

void z80_rlc_hl(struct z80 *z80)
{
	z80_rlrx_hl(z80, 1, 0);
}

void z80_rlc_I_D(struct z80 *z80)
{
	z80_rlrx_I_D(z80, 1, 0);
}

void z80_rlc_I_D_R(struct z80 *z80)
{
	z80_rlrx_I_D_R(z80, 1, 0);
}

void z80_rld(struct z80 *z80)
{
	uint16_t tmp = (z80->mem[z80->regs.hl] << 4) + ((*z80->regs.a) & 0x0f);
	z80->mem[z80->regs.hl] = tmp;
	(*z80->regs.a) &= 0xf0 + (tmp >> 8);
	z80_set_flags_default(z80, *z80->regs.a, 0, 0, 0, FLAG_S | FLAG_Z | FLAG_X | FLAG_Y | FLAG_PV | FLAG_PV_PARITY);
}

void z80_rra(struct z80 *z80)
{
	z80_rlrx_a(z80, 0, 1);
}

void z80_rr_R(struct z80 *z80)
{
	z80_rlrx_R(z80, 0, 1);
}

void z80_rr_hl(struct z80 *z80)
{
	z80_rlrx_hl(z80, 0, 1);
}

void z80_rr_I_D(struct z80 *z80)
{
	z80_rlrx_I_D(z80, 0, 1);
}

void z80_rr_I_D_R(struct z80 *z80)
{
	z80_rlrx_I_D_R(z80, 0, 1);
}

void z80_rrca(struct z80 *z80)
{
	z80_rlrx_a(z80, 1, 1);
}

void z80_rrc_R(struct z80 *z80)
{
	z80_rlrx_R(z80, 1, 1);
}

void z80_rrc_hl(struct z80 *z80)
{
	z80_rlrx_hl(z80, 1, 1);
}

void z80_rrc_I_D(struct z80 *z80)
{
	z80_rlrx_I_D(z80, 1, 1);
}

void z80_rrc_I_D_R(struct z80 *z80)
{
	z80_rlrx_I_D_R(z80, 1, 1);
}

void z80_rrd(struct z80 *z80)
{
	uint8_t tmp = z80->mem[z80->regs.hl];
	z80->mem[z80->regs.hl] = (tmp >> 4) + (((*z80->regs.a) & 0x0f) << 4);
	(*z80->regs.a) &= 0xf0 + (tmp & 0x0f);
	z80_set_flags_default(z80, *z80->regs.a, 0, 0, 0, FLAG_S | FLAG_Z | FLAG_X | FLAG_Y | FLAG_PV | FLAG_PV_PARITY);
	INCPC(2);
}
