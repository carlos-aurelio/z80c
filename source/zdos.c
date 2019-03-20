/* http://map.grauw.nl/resources/dos2_functioncalls.php */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>
#include "core/util.h"

#define MEM_LEN 65536
#define IO_LEN 256

#define DECL_FUNC(num) { 0x ## num, zdos_ ## num ## h }
#define ARRLEN(arr) (sizeof(arr) / sizeof((arr)[0]))

#define PRMSG(...) do {\
	printf("\n\n***\n*** ");\
	printf(__VA_ARGS__);\
	printf("\n***\n\n");\
} while (0)

typedef void (*zdos_function)(struct z80 *);

struct func_tuple {
	int number;
	zdos_function function;
};


/* _TERM0 */
void zdos_00h(struct z80 *z80)
{
	(void)z80;
	PRMSG("Program exited.");
	exit(0);
}

/* _CONIN */
void zdos_01h(struct z80 *z80)
{
	int c;
	do {
		c = getchar();
	} while (c < 0);
	printf("%c", c);
	*z80->regs.l = *z80->regs.a = c;
}

/* _CONOUT */
void zdos_02h(struct z80 *z80)
{
	printf("%c", *z80->regs.e);
}

/* _STROUT */
void zdos_09h(struct z80 *z80)
{
	uint8_t *p = &z80->mem[z80->regs.de];
	while (*p != '$' && (p - z80->mem) < 65536) {
		putchar(*p);
		p++;
	}
}

struct func_tuple functions[] = {
	DECL_FUNC(00),
	DECL_FUNC(01),
	DECL_FUNC(02),
	DECL_FUNC(09),
};

void zdos_call(struct z80 *z80)
{
	int ok = 0;
	for (size_t i = 0; i < ARRLEN(functions) && !ok; i++) {
		if (functions[i].number == *z80->regs.c) {
			functions[i].function(z80);
			ok = 1;
		}
	}
	if (!ok)
		PRMSG("Invalid ZDOS function: 0x%02X\n", *z80->regs.c);
}

void zdos_putc(struct z80 *z80)
{
	if (*z80->regs.a == '\r')
		putchar('\n');
	else
		putchar(*z80->regs.a);
	fflush(stdout);
}

void step(struct z80 *z80)
{
	if (z80->pc == 0x05)
		zdos_call(z80);
	if (z80->pc == 0x10)
		zdos_putc(z80);
	if (z80->mem[z80->pc] == 0x76) {
		PRMSG("CPU halt.");
		exit(1);
	}
	z80_step(z80);
}

void init(struct z80 *z80, uint8_t *mem, uint8_t *io, uint16_t run_addr)
{
	z80_init(z80, mem, io);

	/* setup zdos call return */
	/* ld c,0 (_TERM0) */
	mem[0] = 0x0E;
	mem[1] = 0x00;
	/* nop */
	mem[2] = 0x00;
	mem[3] = 0x00;
	mem[4] = 0x00;
	/* ret */
	mem[5] = 0xC9;
	/* ret */
	mem[16] = 0xC9;

	/* setup entry point */
	z80->pc = run_addr;
}

int load_program(uint8_t *mem, const char *fname, uint16_t load_addr)
{
	FILE *f = fopen(fname, "rb");
	int c, p = load_addr;
	if (!f) {
		perror("fopen");
		return 1;
	}
	while (!feof(f)) {
		if (p >= MEM_LEN) {
			printf("Program too big, discarding extra data.\n");
			break;
		}
		c = fgetc(f);
		if (c >= 0) {
			mem[p] = c;
			p++;
		}
	}
	fclose(f);
	return 0;
}

void run(struct z80 *z80)
{
	PRMSG("ZDOS: High level Z80 operating system emulation");
	for (;;)
		step(z80);
}

void printhelp(char *argv0)
{
	printf("Usage: %s <program> <load addr> <run addr>\n\n", argv0);
	printf("      program      Binary file containing program to load.\n");
	printf("    load addr      Memory address to load program into (hexadecimal).\n");
	printf("     run addr      Memory address to start execution (hexadecimal).\n");
}

int main(int argc, char *argv[])
{
	uint16_t load_addr;
	uint16_t run_addr;
	struct z80 z80;
	uint8_t mem[MEM_LEN];
	uint8_t io[IO_LEN];

	if (argc != 4) {
		printhelp(argv[0]);
		return 1;
	}

	if (sscanf(argv[2], "%hX", &load_addr) != 1) {
		printf("Invalid loading address: %s\n", argv[2]);
		return 1;
	}

	if (sscanf(argv[3], "%hX", &run_addr) != 1) {
		printf("Invalid starting address: %s\n", argv[3]);
		return 1;
	}

	memset(mem, 0, sizeof(mem));
	memset(io, 0, sizeof(io));
	if (load_program(mem, argv[1], load_addr))
		return 1;

	init(&z80, mem, io, run_addr);
	run(&z80);
	return 0;
}