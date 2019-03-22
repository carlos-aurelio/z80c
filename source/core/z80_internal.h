#ifndef __Z80_INTERNAL_H__
#define __Z80_INTERNAL_H__

#include "util.h"
#include "z80.h"

#define ARRLEN(arr) (sizeof(arr) / sizeof((arr)[0]))

#define FLAG_S (1 << 7)
#define FLAG_Z (1 << 6)
#define FLAG_X (1 << 5)
#define FLAG_H (1 << 4)
#define FLAG_Y (1 << 3)
#define FLAG_PV (1 << 2)
#define FLAG_N (1 << 1)
#define FLAG_C (1 << 0)

#define FLAG_PV_OVERFLOW 0
#define FLAG_PV_PARITY (1 << 8)

#define INCPC(n) z80->pc += (n);


void z80_set_flag(struct  z80 *z80, uint8_t flag, uint32_t set_reset);
uint8_t z80_get_flag(struct z80 *z80, uint8_t flag);
uint8_t z80_get_op(struct z80 *z80, int8_t idx);
uint16_t z80_get_op16(struct z80 *z80, uint8_t idx);
uint8_t *z80_regs_rrr(struct z80 *z80, uint8_t rrr);
uint16_t *z80_regs_ixiy(struct z80 *z80);
uint8_t *z80_regs_half_ixiy(struct z80 *z80, uint8_t b);
uint16_t *z80_regs_qq(struct z80 *z80, uint8_t qq);
uint16_t *z80_regs_pp(struct z80 *z80);
uint8_t z80_cond_ccc(struct z80 *z80);
/* Relevant parameters:
 * S, Z, X, Y, P: res;
 * H, V, N: dst, src, cf;
 */
void z80_set_flags_default(struct z80 *z80, uint16_t res, uint16_t dst, uint16_t src, uint16_t cf, uint16_t flags);
uint8_t *z80_get_J(struct z80 *z80, uint8_t b);
uint8_t *z80_get_I_D(struct z80 *z80);

void z80_adx_hl_Q(struct z80 *z80, uint8_t flags);

void z_init_opcode_table(struct z80 *z80);
void z_base(struct z80 *z80);

#endif /* __Z80_INTERNAL_H__ */