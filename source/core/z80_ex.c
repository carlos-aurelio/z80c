#include "z80_instructions.h"

void z80_ex_sp_hl(struct z80 *z80)
{
	swap16((uint16_t *)(&z80->mem[z80->sp]), &z80->regs.hl);
	INCPC(1);
}

void z80_ex_sp_I(struct z80 *z80)
{
	uint16_t *i = z80_regs_ixiy(z80);
	swap16((uint16_t *)(&z80->mem[z80->sp]), i);
	INCPC(2);
}

void z80_ex_af_af(struct z80 *z80)
{
	swap16(&z80->regs.af, &z80->regs_alt.af);
	INCPC(1);
}

void z80_ex_de_hl(struct z80 *z80)
{
	swap16(&z80->regs.de, &z80->regs.hl);
	INCPC(1);
}

void z80_exx(struct z80 *z80)
{
	swap16(&z80->regs.bc, &z80->regs_alt.bc);
	swap16(&z80->regs.de, &z80->regs_alt.de);
	swap16(&z80->regs.hl, &z80->regs_alt.hl);
	INCPC(1);
}
