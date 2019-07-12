#include "z80_instructions.h"

void z80_ld_R1_R2(struct z80 *z80)
{
	uint8_t o0 = z80_get_op(z80, 0);
	uint8_t *r1 = z80_regs_rrr(z80, o0 >> 3);
	uint8_t *r2 = z80_regs_rrr(z80, o0);
	*r1 = *r2;
	INCPC(1);
}

void z80_ld_R_J(struct z80 *z80)
{
	uint8_t o1 = z80_get_op(z80, 1);
	uint8_t *j = z80_get_J(z80, o1);
	uint8_t *r = z80_regs_rrr(z80, o1 >> 3);
	*r = *j;
	INCPC(2);
}

void z80_ld_J_R(struct z80 *z80)
{
	uint8_t o1 = z80_get_op(z80, 1);
	uint8_t *j = z80_get_J(z80, o1 >> 3);
	uint8_t *r = z80_regs_rrr(z80, o1);
	*j = *r;
	INCPC(2);
}

void z80_ld_ixh_ixl(struct z80 *z80)
{
	*z80->ixh = *z80->ixl;
	INCPC(2);
}

void z80_ld_ixl_ixh(struct z80 *z80)
{
	*z80->ixl = *z80->ixh;
	INCPC(2);
}

void z80_ld_iyh_iyl(struct z80 *z80)
{
	*z80->iyh = *z80->iyl;
	INCPC(2);
}

void z80_ld_iyl_iyh(struct z80 *z80)
{
	*z80->iyl = *z80->iyh;
	INCPC(2);
}

void z80_ld_R(struct z80 *z80, uint8_t val)
{
	uint8_t o0 = z80_get_op(z80, 0);
	uint8_t *r = z80_regs_rrr(z80, o0 >> 3);
	*r = val;
}

void z80_ld_R_N(struct z80 *z80)
{
	z80_ld_R(z80, z80_get_op(z80, 1));
	INCPC(2);
}

void z80_ld_R_hl(struct z80 *z80)
{
	z80_ld_R(z80, z80->mem[z80->regs.hl]);
	INCPC(1);
}

void z80_ld_R_I_D(struct z80 *z80)
{
	z80_ld_R(z80, *z80_get_I_D(z80));
	INCPC(3);
}

void z80_ld_hl_R(struct z80 *z80)
{
	uint8_t o0 = z80_get_op(z80, 0);
	uint8_t *r = z80_regs_rrr(z80, o0);
	z80->mem[z80->regs.hl] = *r;
	INCPC(1);
}

void z80_ld_hl_N(struct z80 *z80)
{
	z80->mem[z80->regs.hl] = z80_get_op(z80, 1);
	INCPC(2);
}

void z80_ld_I_D_R(struct z80 *z80)
{
	uint8_t *id = z80_get_I_D(z80);
	uint8_t o1 = z80_get_op(z80, 1);
	uint8_t *r = z80_regs_rrr(z80, o1);
	*id = *r;
	INCPC(3);
}

void z80_ld_I_D_N(struct z80 *z80)
{
	uint8_t *id = z80_get_I_D(z80);
	*id = z80_get_op(z80, 2);
	INCPC(4);
}

void z80_ld_a_bc(struct z80 *z80)
{
	*z80->regs.a = z80->mem[z80->regs.bc];
	INCPC(1);
}

void z80_ld_a_de(struct z80 *z80)
{
	*z80->regs.a = z80->mem[z80->regs.de];
	INCPC(1);
}

void z80_ld_a_A(struct z80 *z80)
{
	*z80->regs.a = z80->mem[z80_get_op16(z80, 1)];
	INCPC(3);
}

void z80_ld_bc_a(struct z80 *z80)
{
	z80->mem[z80->regs.bc] = *z80->regs.a;
	INCPC(1);
}

void z80_ld_de_a(struct z80 *z80)
{
	z80->mem[z80->regs.de] = *z80->regs.a;
	INCPC(1);
}

void z80_ld_A_a(struct z80 *z80)
{
	z80->mem[z80_get_op16(z80, 1)] = *z80->regs.a;
	INCPC(3);
}

void z80_ld_i_a(struct z80 *z80)
{
	z80->i = *z80->regs.a;
	INCPC(2);
}

void z80_ld_r_a(struct z80 *z80)
{
	z80->r = *z80->regs.a;
	INCPC(2);
}

void z80_ld_a_i(struct z80 *z80)
{
	*z80->regs.a = z80->i;
	z80_set_flags_default(z80, *z80->regs.a, 0, 0, 0, FLAG_S | FLAG_Z | FLAG_X | FLAG_Y);
	z80_set_flag(z80, FLAG_H | FLAG_N, 0);
	z80_set_flag(z80, FLAG_PV, z80->iff1);
	INCPC(2);
}

void z80_ld_a_r(struct z80 *z80)
{
	*z80->regs.a = z80->r;
	z80_set_flags_default(z80, *z80->regs.a, 0, 0, 0, FLAG_S | FLAG_Z | FLAG_X | FLAG_Y);
	z80_set_flag(z80, FLAG_H | FLAG_N, 0);
	z80_set_flag(z80, FLAG_PV, z80->iff1);
	INCPC(2);
}

void z80_ld_Q_A(struct z80 *z80)
{
	uint8_t o0 = z80_get_op(z80, 0);
	uint16_t *q = z80_regs_qq(z80, o0 >> 4);
	*q = z80_get_op16(z80, 1);
	INCPC(3);
}

void z80_ld_I_A(struct z80 *z80)
{
	uint16_t *i = z80_regs_ixiy(z80);
	*i = z80_get_op16(z80, 2);
	INCPC(4);
}

void z80_ld_Q_indA(struct z80 *z80)
{
	uint8_t o1 = z80_get_op(z80, 1);
	uint16_t *q = z80_regs_qq(z80, o1 >> 4);
	*q = z80->mem[z80_get_op16(z80, 2)];
	INCPC(4);
}

void z80_ld_hl_indA(struct z80 *z80)
{
	z80->mem[z80->regs.hl] = z80->mem[z80_get_op16(z80, 1)];
	INCPC(3);
}

void z80_ld_I_indA(struct z80 *z80)
{
	uint16_t *i = z80_regs_ixiy(z80);
	z80->mem[*i] = z80_get_op16(z80, 2);
	INCPC(4);
}

void z80_ld_A_Q(struct z80 *z80)
{
	uint8_t o1 = z80_get_op(z80, 1);
	uint16_t *q = z80_regs_qq(z80, o1 >> 4);
	z80->mem[z80_get_op16(z80, 2)] = *q;
	INCPC(4);
}

void z80_ld_A_hl(struct z80 *z80)
{
	z80->mem[z80_get_op16(z80, 1)] = z80->mem[z80->regs.hl];
	INCPC(3);
}

void z80_ld_A_I(struct z80 *z80)
{
	uint16_t *i = z80_regs_ixiy(z80);
	z80->mem[z80_get_op16(z80, 2)] = *i;
	INCPC(4);
}

void z80_ld_sp_hl(struct z80 *z80)
{
	z80->sp = z80->regs.hl;
	INCPC(1);
}

void z80_ld_sp_I(struct z80 *z80)
{
	uint16_t *i = z80_regs_ixiy(z80);
	z80->sp = *i;
	INCPC(2);
}

void z80_ldx(struct z80 *z80, int8_t n)
{
	uint8_t tmp = z80->mem[z80->regs.hl];
	uint8_t fpv = z80_get_flag(z80, FLAG_PV);
	z80->mem[z80->regs.de] = tmp;
	z80->regs.de += n;
	z80->regs.hl += n;
	z80->regs.bc--;
	z80_set_flag(z80, FLAG_X, (tmp + *z80->regs.a) & (1 << 1));
	z80_set_flag(z80, FLAG_H | FLAG_N, 0);
	z80_set_flag(z80, FLAG_Y, (tmp + *z80->regs.a) & (1 << 3));
	if (z80->regs.bc)
		z80_set_flag(z80, FLAG_PV, 1);
	else
		z80_set_flag(z80, FLAG_PV, fpv);
	INCPC(2);
}

void z80_ldd(struct z80 *z80)
{
	z80_ldx(z80, -1);
}

void z80_lddr(struct z80 *z80)
{
	z80_ldd(z80);
	if (z80->regs.bc)
		z80->pc -= 2;
}

void z80_ldi(struct z80 *z80)
{
	z80_ldx(z80, 1);
}

void z80_ldir(struct z80 *z80)
{
	z80_ldi(z80);
	if (z80->regs.bc)
		z80->pc -= 2;
}
