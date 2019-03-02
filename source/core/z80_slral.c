#include "z80_instructions.h"

/*

sla		cf := R.7, R :=  R << 1
sra		cf := R.0, R :=  R >> 1, R.7 := R.6
sll		cf := R.7, R := [R << 1] + 1
srl		cf := R.0, R :=  R >> 1
*/

void z80_slral_(struct z80 *z80, uint8_t *r, uint8_t _r, uint8_t _l)
{
	uint8_t res;
	if (_r) {	/* sra, srl */
		z80_set_flag(z80, FLAG_C, bitval(*r, 0));
		res = *r >> 1;
	} else {	/* sla, sll */
		z80_set_flag(z80, FLAG_C, bitval(*r, 7));
		res = *r << 1;
	}

	if (!_r && _l)		/* sll */
		res++;
	if (_r && !_l) {	/* sra */
		if (bitval(res, 6))
			res |= 1 << 7;
		else
			res &= ~(1 << 7);
	}
	*r = res;
	z80_set_flags_default(z80, res, *r, 0, 0, FLAG_S | FLAG_Z | FLAG_X | FLAG_Y | FLAG_PV | FLAG_PV_PARITY);
	z80_set_flag(z80, FLAG_H | FLAG_N, 0);
}

void z80_slrar_R(struct z80 *z80, uint8_t _r, uint8_t _l)
{
	uint8_t op1 = z80_get_op(z80, 1);
	uint8_t *r = z80_regs_rrr(z80, op1);
	z80_slral_(z80, r, _r, _l);
	INCPC(2);
}

void z80_slrar_hl(struct z80 *z80, uint8_t _r, uint8_t _l)
{
	uint8_t *hl = &z80->mem[z80->regs.hl];
	z80_slral_(z80, hl, _r, _l);
	INCPC(2);
}

void z80_slrar_I_D(struct z80 *z80, uint8_t _r, uint8_t _l)
{
	uint8_t *id = z80_get_I_D(z80);
	z80_slral_(z80, id, _r, _l);
	INCPC(4);
}

void z80_slrar_I_D_R(struct z80 *z80, uint8_t _r, uint8_t _l)
{
	uint8_t *id = z80_get_I_D(z80);
	uint8_t op3 = z80_get_op(z80, 3);
	uint8_t *r = z80_regs_rrr(z80, op3);
	z80_slral_(z80, id, _r, _l);
	*r = *id;
	INCPC(4);
}

void z80_sla_R(struct z80 *z80)
{
	z80_slrar_R(z80, 0, 0);
}

void z80_sla_hl(struct z80 *z80)
{
	z80_slrar_hl(z80, 0, 0);
}

void z80_sla_I_D(struct z80 *z80)
{
	z80_slrar_I_D(z80, 0, 0);
}

void z80_sla_I_D_R(struct z80 *z80)
{
	z80_slrar_I_D_R(z80, 0, 0);
}

void z80_sra_R(struct z80 *z80)
{
	z80_slrar_R(z80, 1, 0);
}

void z80_sra_hl(struct z80 *z80)
{
	z80_slrar_hl(z80, 1, 0);
}

void z80_sra_I_D(struct z80 *z80)
{
	z80_slrar_I_D(z80, 1, 0);
}

void z80_sra_I_D_R(struct z80 *z80)
{
	z80_slrar_I_D_R(z80, 1, 0);
}

void z80_sll_R(struct z80 *z80)
{
	z80_slrar_R(z80, 0, 1);
}

void z80_sll_hl(struct z80 *z80)
{
	z80_slrar_hl(z80, 0, 1);
}

void z80_sll_I_D(struct z80 *z80)
{
	z80_slrar_I_D(z80, 0, 1);
}

void z80_sll_I_D_R(struct z80 *z80)
{
	z80_slrar_I_D_R(z80, 0, 1);
}

void z80_srl_R(struct z80 *z80)
{
	z80_slrar_R(z80, 1, 1);
}

void z80_srr_hl(struct z80 *z80)
{
	z80_slrar_hl(z80, 1, 1);
}

void z80_srr_I_D(struct z80 *z80)
{
	z80_slrar_I_D(z80, 1, 0);
}

void z80_srr_I_D_R(struct z80 *z80)
{
	z80_slrar_I_D_R(z80, 1, 0);
}

void z80_srl_I_D(struct z80 *z80)
{
	z80_slrar_I_D(z80, 1, 1);
}

void z80_srl_I_D_R(struct z80 *z80)
{
	z80_slrar_I_D_R(z80, 1, 1);
}