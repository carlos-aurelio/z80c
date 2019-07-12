#ifndef __Z80_H__
#define __Z80_H__

#include <stdint.h>

typedef uint8_t (*cb_in)(uint8_t port);
typedef void (*cb_out)(uint8_t port, uint8_t val);

struct regs {
	uint16_t af;
	uint16_t bc;
	uint16_t de;
	uint16_t hl;

	/* reference registers */
	uint8_t *a;
	uint8_t *f;
	uint8_t *b;
	uint8_t *c;
	uint8_t *d;
	uint8_t *e;
	uint8_t *h;
	uint8_t *l;
};

struct z80 {
	struct z80_opcode *opcodes;
	struct regs regs;
	struct regs regs_alt;
	uint16_t ix, iy, sp;
	uint8_t i, r;
	uint16_t pc;
	uint8_t iff1, iff2;

	/* reference registers */
	uint8_t *ixh, *ixl, *iyh, *iyl;

	/* external memory */
	uint8_t *mem;

	/* i/o callbacks */
	cb_in cb_in;
	cb_out cb_out;
};

typedef void (*opcode_func)(struct z80 *);

struct z80_opcode {
	opcode_func func;
	const char *name;
};

/* initializes the core emulator with given memory and i/o callbacks */
void z80_init(struct z80 *z80, uint8_t *mem, cb_in cb_in, cb_out cb_out);

/* executes a single instruction and returns */
void z80_step(struct z80 *z80);

#endif /* __Z80_H__ */
