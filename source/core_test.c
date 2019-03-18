#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>
#include "core/trace.h"

void print_regs(struct z80 *z80)
{
	printf("       main     alternate    S Z X H Y P N C\n");
	printf(" A F : %02X %02X    %02X %02X", *z80->regs.a, *z80->regs.f,
	       *z80->regs_alt.a, *z80->regs_alt.f);
	printf("        %d %d %d %d %d %d %d %d\n", 
	       (*z80->regs.f >> 7) & 1, (*z80->regs.f >> 6) & 1,
	       (*z80->regs.f >> 5) & 1, (*z80->regs.f >> 4) & 1,
	       (*z80->regs.f >> 3) & 1, (*z80->regs.f >> 2) & 1,
	       (*z80->regs.f >> 1) & 1, (*z80->regs.f >> 0) & 1);
	printf(" B C : %02X %02X    %02X %02X\n", *z80->regs.b, *z80->regs.c,
	       *z80->regs_alt.b, *z80->regs_alt.c);
	printf(" D E : %02X %02X    %02X %02X\n", *z80->regs.d, *z80->regs.e,
	       *z80->regs_alt.d, *z80->regs_alt.e);
	printf(" H L : %02X %02X    %02X %02X\n", *z80->regs.h, *z80->regs.l,
	       *z80->regs_alt.h, *z80->regs_alt.l);
	printf("  IX : %04X\n", z80->ix);
	printf("  IY : %04X\n", z80->iy);
	printf("  SP : %04X\n", z80->sp);
	printf("   I : %02X\n", z80->i);
	printf("   R : %02X\n", z80->r);
	printf("  PC : %04X\n", z80->pc);
	printf("\n");
	printf("                    PC\n");
	printf(" mem : %02X %02X %02X %02X [%02X] %02X %02X %02X\n",
	       z80->mem[z80->pc - 4], z80->mem[z80->pc - 3],
	       z80->mem[z80->pc - 2], z80->mem[z80->pc - 1],
	       z80->mem[z80->pc], z80->mem[z80->pc + 1],
	       z80->mem[z80->pc + 2], z80->mem[z80->pc + 3]);
}

void run(struct z80 *z80)
{
	int nonstop = 0;
	char tmp[100];
	char last_cmd = 0;
	printf("core ready\n");
	for (;;) {
		if (!nonstop) {
			printf("> ");
			fflush(stdin);
			fgets(tmp, sizeof(tmp) - 1, stdin);
			if (tmp[0] != '\n')
				last_cmd = tmp[0];
		}
		switch (last_cmd) {
			case 'n':
				z80_step(z80);
				break;
			case 's':
				z80_step(z80);
				print_regs(z80);
				break;
			case 'p':
				print_regs(z80);
				break;
			case 'r':
				z80_step(z80);
				nonstop = 1;
				break;
			case 'q':
				return;
			default:
				printf("invalid command: %s\n", tmp);
				break;
		}
	}
}

int hex2nibble(int c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 0xA;
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 0xa;
	else
		return -1;
}

void readfile(FILE *f, uint8_t *mem, size_t len, int is_hex)
{
	int b, c, n;
	int msbready = 0;
	int byteready = 0;
	size_t pos = 0;
	while (!feof(f)) {
		c = fgetc(f);
		if (b < 0)
			return;
		if (is_hex) {
			n = hex2nibble(c);
			/* ignore non-hexadecimal characters */
			if (n >= 0) {
				if (!msbready) {
					b = n << 4;
					msbready = 1;
				} else {
					b |= n;
					msbready = 0;
					byteready = 1;
				}
			}
		} else {
			b = c;
			byteready = 1;
		}
		if (byteready) {
			if (pos >= len) {
				printf("Input file too big, discarding exceeding data.\n");
				return;
			}
			mem[pos++] = (uint8_t)b;
			byteready = 0;
		}
	}
}

void printhelp(char *argv0)
{
	printf("Usage: %s [-h] <input file>\n\n", argv0);
	printf("    -h    Read input file as hexadecimal string.\n");
	printf("          Default is binary.");
}

int main(int argc, char *argv[])
{
	int is_hex;
	if (argc == 2) {
		is_hex = 0;
	} else if (argc == 3) {
		if (strcmp(argv[1], "-h")) {
			printhelp(argv[0]);
			return 1;
		}
		is_hex = 1;
	} else {
		printhelp(argv[0]);
		return 1;
	}

	FILE *f = fopen(argv[argc - 1], "rb");
	if (!f) {
		printf("file not found: %s\n", argv[argc - 1]);
		return 1;
	}

	struct z80 z80;
	uint8_t mem[65536] = { 0 };
	uint8_t io[256] = { 0 };

	readfile(f, mem, sizeof(mem), is_hex);

	z80_init(&z80, mem, io);
	run(&z80);
	return 0;
}