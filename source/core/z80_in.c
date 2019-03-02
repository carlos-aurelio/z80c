#include <stdlib.h>

#include "z80_instructions.h"

void z80_in_a_N(struct z80 *z80)
{
	*z80->regs.a = z80->io[z80_get_op(z80, 1)];
	INCPC(2);
}

void z80_in__c(struct z80 *z80, uint8_t *r)
{
	uint8_t c = z80->io[*z80->regs.c];
	uint8_t res = c;
	z80_set_flags_default(z80, res, res, c, 0, FLAG_S | FLAG_Z | FLAG_X | FLAG_Y | FLAG_PV | FLAG_PV_PARITY);
	z80_set_flag(z80, FLAG_H | FLAG_N, 0);
	if (r)
		*r = res;
	INCPC(2);
}

void z80_in_R_c(struct z80 *z80)
{
	uint8_t o1 = z80_get_op(z80, 1);
	uint8_t *r = z80_regs_rrr(z80, o1 >> 3);
	z80_in__c(z80, r);
}

void z80_in_f_c(struct z80 *z80)
{
	z80_in__c(z80, NULL);
}