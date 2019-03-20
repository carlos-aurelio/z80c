#include <string.h>
#include "z80_instructions.h"

/* prints a warning if the opcode in the given table already has a function
 * assigned to it
 */
#define WARN_IF_CONFLICT(opcode, tblname, function) do {\
	if ((&optbl_ ## tblname[opcode])->func) {\
		WARN("Replacing %8s [ 0x%02X ] : '%s' -> '%s' @ %d",\
		xstr(tblname), (unsigned int)(opcode),\
		(&optbl_ ## tblname[opcode])->name,\
		xstr(function),\
		__LINE__\
		);\
	}\
} while (0)

/* assigns a function to an opcode in the given table, warning if the opcode
 * already has a function assigned to it
 */
#define OPFUNC(opcode_0, tblname, function) do {\
	WARN_IF_CONFLICT(opcode_0, tblname, function);\
	(&optbl_ ## tblname[opcode_0])->func = &function;\
	(&optbl_ ## tblname[opcode_0])->name = xstr(function);\
} while (0)

/* assigns a function to an opcode in both extende IX and IY tables */
#define OPFUNC_i(opcode_1, function) do {\
	OPFUNC(opcode_1, ix, function);\
	OPFUNC(opcode_1, iy, function);\
} while (0)

/* assigns a function to an opcode in bot extended IX and IY BITS tables */
#define OPFUNC_ibits(opcode_3, function) do {\
	OPFUNC(opcode_3, ixbits, function);\
	OPFUNC(opcode_3, iybits, function);\
} while (0)

/* assigns a dummy function to an opcode in the given table */
#define OPNOPE(opcode_1, tblname) OPFUNC(opcode_1, tblname, z_nothing_ext)

/*           .---- 3 bits: 0 through 7 ----.
 *          / \                             V
 *  FORBITS(xxx) -> for (int xxx; xxx < 2 ^ 3; xxx++)
 */
#define FORBITS(var) for (int var = 0; var < (1 << strlen(xstr(var))); var++)

/* opcode table declaration */
#define TBL_DECL(name) static struct z80_opcode optbl_ ## name[256] = { 0 }
TBL_DECL(base);
TBL_DECL(extended);
TBL_DECL(bits);
TBL_DECL(ix);
TBL_DECL(ixbits);
TBL_DECL(iy);
TBL_DECL(iybits);

void z_nothing_ext(struct z80 *z80)
{
	WARN("Undefined opcode: %02X%02X",
		z80_get_op(z80, 0), z80_get_op(z80, 1));
}

void z_nothing_ixiybits(struct z80 *z80)
{
	WARN("Undefined opcode: %02X%02X**%02X",
		z80_get_op(z80, 0), z80_get_op(z80, 1), z80_get_op(z80, 3));
}

void z_base(struct z80 *z80)
{
	uint8_t op = z80_get_op(z80, 0);
	optbl_base[op].func(z80);
}

void z_extended(struct z80 *z80)
{
	uint8_t op1 = z80_get_op(z80, 1);
	optbl_extended[op1].func(z80);
}

void z_bits(struct z80 *z80)
{
	uint8_t op1 = z80_get_op(z80, 1);
	optbl_bits[op1].func(z80);
}

void z_ix(struct z80 *z80)
{
	uint8_t op1 = z80_get_op(z80, 1);
	optbl_ix[op1].func(z80);
}

void z_ix_bits(struct z80 *z80)
{
	uint8_t op3 = z80_get_op(z80, 3);
	optbl_ixbits[op3].func(z80);
}

void z_iy(struct z80 *z80)
{
	uint8_t op1 = z80_get_op(z80, 1);
	optbl_iy[op1].func(z80);
}

void z_iy_bits(struct z80 *z80)
{
	uint8_t op3 = z80_get_op(z80, 3);
	optbl_iybits[op3].func(z80);
}

static void z_init_opcode_unused(void)
{
	/* unused extended table ranges */
	static const uint8_t un_ex[][2] = {
		{ 0x00, 0x3F },
		{ 0x77, 0x77 }, { 0x7F, 0x7F },
		{ 0x80, 0x9F },
		{ 0xA4, 0xA7 }, { 0xAC, 0xAF },
		{ 0xB4, 0xB7 }, { 0xBC, 0xBF },
		{ 0xC0, 0xFF }
	};

	/* unused IX and IY tables ranges */
	static const uint8_t un_ixiy[][2] = {
		{ 0x00, 0x08 }, { 0x0A, 0x0F },
		{ 0x10, 0x18 }, { 0x1A, 0x1F },
		{ 0x20, 0x20 }, { 0x27, 0x28 }, { 0x2F, 0x2F },
		{ 0x30, 0x33 }, { 0x37, 0x38 }, { 0x3A, 0x3F },
		{ 0x40, 0x43 }, { 0x47, 0x4B }, { 0x4F, 0x4F },
		{ 0x50, 0x53 }, { 0x57, 0x5B }, { 0x5F, 0x5F },
		{ 0x76, 0x76 }, { 0x78, 0x7B }, { 0x7F, 0x7F },
		{ 0x80, 0x83 }, { 0x87, 0x8B }, { 0x8F, 0x8F },
		{ 0x90, 0x93 }, { 0x97, 0x9B }, { 0x9F, 0x9F },
		{ 0xA0, 0xA3 }, { 0xA7, 0xAB }, { 0xAF, 0xAF },
		{ 0xB0, 0xB3 }, { 0xB7, 0xBB }, { 0xBF, 0xBF },
		{ 0xC0, 0xCA }, { 0xCC, 0xCF },
		{ 0xD0, 0xDF },
		{ 0xE0, 0xE0 }, { 0xE2, 0xE2 }, { 0xE4, 0xE4 }, { 0xE6, 0xE8 }, { 0xEA, 0xEF },
		{ 0xF0, 0xF8 }, { 0xFA, 0xFF }
	};
	for (size_t i = 0; i < ARRLEN(un_ex); i++) {
		for (size_t j = un_ex[i][0]; j <= un_ex[i][1]; j++)
			OPNOPE(j, extended);
	}

	for (size_t i = 0; i < ARRLEN(un_ixiy); i++) {
		for (size_t j = un_ixiy[i][0]; j <= un_ixiy[i][1]; j++) {
			OPNOPE(j, ix);
			OPNOPE(j, iy);
		}
	}
	
}

static void z_init_opcode_adc_add(void)
{
	FORBITS(rrr) {				/* adc|add a,R|(hl) */
		OPFUNC(0x88 | rrr, base, z80_adc_a_R);
		OPFUNC(0x80 | rrr, base, z80_add_a_R);
	}
	FORBITS(b) {				/* adc|add a , J */
		OPFUNC_i(0x8C | b, z80_adc_a_J);
		OPFUNC_i(0x84 | b, z80_add_a_J);
	}
	OPFUNC(0xCE, base, z80_adc_a_N);	/* adc|add a,N */
	OPFUNC(0xC6, base, z80_add_a_N);
	OPFUNC_i(0x8E, z80_adc_a_I_D);		/* adc|add a,(I+D) */
	OPFUNC_i(0x86, z80_add_a_I_D);
	FORBITS(qq) {				/* adc|add hl|I,Q */
		OPFUNC(0x4A | (qq << 4), extended, z80_adc_hl_Q);
		OPFUNC(0x09 | (qq << 4), base, z80_add_hl_Q);
		OPFUNC_i(0x09 | (qq << 4), z80_add_I_Q);
	}
}

static void z_init_opcode_and(void)
{
	FORBITS(rrr)				/* and R|(hl) */
		OPFUNC(0xA0 | rrr, base, z80_and_R);
	FORBITS(b)
		OPFUNC_i(0xA4 | b, z80_and_J);	/* and J */
	OPFUNC(0xE6, base, z80_and_N);		/* and N */
	OPFUNC_i(0xA6, z80_and_I_D);		/* and (I+D) */
}

static void z_init_opcode_bit(void)
{
	FORBITS(bbbrrr)				/* bit B,R|(hl) */
		OPFUNC(0x40 | bbbrrr, bits, z80_bit_B_R);
	FORBITS(bbbbbb)				/* bit B,(I+D) */
		OPFUNC_ibits(0x40 | bbbbbb, z80_bit_B_I_D);
}

static void z_init_opcode_call(void)
{
	OPFUNC(0xCD, base, z80_call_A);		/* call A */
	FORBITS(ccc)				/* call C,A */
		OPFUNC(0xC4 | (ccc << 3), base, z80_call_C_A);
}

static void z_init_opcode_cp(void)
{
	FORBITS(rrr)				/* cp R|(hl) */
		OPFUNC(0xB8 | rrr, base, z80_cp_R);
	FORBITS(b)				/* cp J */
		OPFUNC_i(0xBC | b, z80_cp_J);
	OPFUNC(0xFE, base, z80_cp_N);		/* cp N */
	OPFUNC_i(0xBE, z80_cp_I_D);		/* cp (I+D) */
	OPFUNC(0xA9, extended, z80_cpd);	/* cpd */
	OPFUNC(0xB9, extended, z80_cpdr);	/* cpdr */
	OPFUNC(0xA1, extended, z80_cpi);	/* cpi */
	OPFUNC(0xB1, extended, z80_cpir);	/* cpir */
	OPFUNC(0x2F, base, z80_cpl);		/* cpl */
}

static void z_init_opcode_dec(void)
{
	FORBITS(rrr)				/* dec R|(hl) */
		OPFUNC(0x05 | (rrr << 3), base, z80_dec_R);
	FORBITS(b)				/* dec J */
		OPFUNC_i(0x25 | (b << 3), z80_dec_J);
	OPFUNC_i(0x35, z80_dec_I_D);		/* dec (I+D) */
	FORBITS(qq)				/* dec Q */
		OPFUNC(0x0B | (qq << 4), base, z80_dec_Q);
	OPFUNC_i(0x2B, z80_dec_I);		/* dec I */
}

static void z_init_opcode_ex(void)
{
	OPFUNC(0xE3, base, z80_ex_sp_hl);	/* ex (sp),hl */
	OPFUNC_i(0xE3, z80_ex_sp_I);		/* ex (sp),I */
	OPFUNC(0x08, base, z80_ex_af_af);	/* ex af,af' */
	OPFUNC(0xEB, base, z80_ex_de_hl);	/* ex de,hl */
	OPFUNC(0xD9, base, z80_exx);		/* exx */
}

static void z_init_opcode_im(void)
{
	FORBITS(i) {				/* im 0|1|2 */
		FORBITS(j)
			OPFUNC(0x46 | (i << 5) | (j << 3), extended, z80_im_0);
		OPFUNC(0x56 | (i << 5), extended, z80_im_1);
		OPFUNC(0x5E | (i << 5), extended, z80_im_2);
	}
}

static void z_init_opcode_in(void)
{
	OPFUNC(0xDB, base, z80_in_a_N);		/* in a,N */
	FORBITS(rrr)				/* in R,(c) */
		OPFUNC(0x40 | (rrr << 3), extended, z80_in_R_c);
	/* in f,(c): replaces z80_in_R_c when R = (hl) */
	OPFUNC(0x70, extended, z80_in_f_c);
}

static void z_init_opcode_inc(void)
{
	FORBITS(rrr)				/* inc R|(hl) */
		OPFUNC(0x04 | (rrr << 3) , base, z80_inc_R);
	FORBITS(b)				/* inc J */
		OPFUNC_i(0x24 | (b << 3), z80_inc_J);
	OPFUNC_i(0x34, z80_inc_I_D);		/* inc (I+D) */
	FORBITS(qq)				/* inc Q */
		OPFUNC(0x03 | (qq << 4), base, z80_inc_Q);
	OPFUNC_i(0x23, z80_inc_I);		/* inc I */
}

static void z_init_opcode_ind_ini(void)
{
	/* in(d|i|dr|ir) */
	OPFUNC(0xAA, extended, z80_ind);
	OPFUNC(0xBA, extended, z80_indr);
	OPFUNC(0xA2, extended, z80_ini);
	OPFUNC(0xB2, extended, z80_inir);
}

static void z_init_opcode_jumps(void)
{
	OPFUNC(0xC3, base, z80_jp_A);		/* jp A */
	OPFUNC(0xE9, base, z80_jp_hl);		/* jp (hl) */
	OPFUNC_i(0xE9, z80_jp_I);		/* jp I */
	FORBITS(ccc)				/* jp C,A */
		OPFUNC(0xC2 | (ccc << 3), base, z80_jp_C_A);
	OPFUNC(0x18, base, z80_jr_E);		/* jr E */
	OPFUNC(0x20, base, z80_jr_nz_E);	/* jr nz,E */
	OPFUNC(0x28, base, z80_jr_z_E);		/* jr z,E */
	OPFUNC(0x30, base, z80_jr_nc_E);	/* jr nc,E */
	OPFUNC(0x38, base, z80_jr_c_E);		/* jr c,E */
}

static void z_init_opcode_ld(void)
{
	FORBITS(rrrsss) {			/* ld R1|(hl),R2|(hl) */
		if (rrrsss != 0x36) /* this is the halt opcode! */
			OPFUNC(0x40 | rrrsss, base, z80_ld_R1_R2);
	}
	FORBITS(rr) {
		if (rr != 6) {
			FORBITS(b) {		/* ld R|J,J|R */
				OPFUNC_i(0x44 | (rr << 3) | b, z80_ld_R_J);
				OPFUNC_i(0x60 | (b << 3) | rr, z80_ld_J_R);
			}
		}
	}
	FORBITS(b)				/* ld J,a */
		OPFUNC_i(0x67 | (b << 3), z80_ld_J_R);

	OPFUNC(0x65, ix, z80_ld_ixh_ixl);	/* ld ixh,ixl */
	OPFUNC(0x6C, ix, z80_ld_ixl_ixh);	/* ld ixl,ixh */
	OPFUNC(0x65, iy, z80_ld_iyh_iyl);	/* ld iyh,iyl */
	OPFUNC(0x6C, iy, z80_ld_iyl_iyh);	/* ld iyl,iyh */
	FORBITS(rrr) {				/* ld R|(hl),N */
		OPFUNC(0x06 | (rrr << 3), base, z80_ld_R_N);
		if (rrr != 6)			/* ld R|(I+D) */
			OPFUNC_i(0x46 | (rrr << 3), z80_ld_R_I_D);
		if (rrr != 6)			/* ld (I+D),R */
			OPFUNC_i(0x70 | rrr, z80_ld_I_D_R);	
		else				/* ld (I+D),N */
			OPFUNC_i(0x36 | rrr, z80_ld_I_D_N);	
	}
	OPFUNC(0x0A, base, z80_ld_a_bc);	/* ld a,(bc) */
	OPFUNC(0x1A, base, z80_ld_a_de);	/* ld a,(de) */
	OPFUNC(0x3A, base, z80_ld_a_A);		/* ld a,(A) */
	OPFUNC(0x02, base, z80_ld_bc_a);	/* ld (bc),a */
	OPFUNC(0x12, base, z80_ld_de_a);	/* ld (de),a */
	OPFUNC(0x32, base, z80_ld_A_a);		/* ld (A),a */
	OPFUNC(0x47, extended, z80_ld_i_a);	/* ld i,a */
	OPFUNC(0x4F, extended, z80_ld_r_a);	/* ld r,a */
	OPFUNC(0x57, extended, z80_ld_a_i);	/* ld a,i */
	OPFUNC(0x5F, extended, z80_ld_a_r);	/* ld a,r */
	FORBITS(qq) {				/* ld Q|(A),A|(A) */
		OPFUNC(0x01 | (qq << 4), base, z80_ld_Q_A);
		OPFUNC(0x4B | (qq << 4), extended, z80_ld_Q_indA);
		OPFUNC(0x43 | (qq << 4), extended, z80_ld_A_Q);
	}
	OPFUNC_i(0x21, z80_ld_I_A);		/* ld I,A */
	OPFUNC_i(0x2A, z80_ld_I_indA);		/* ld I,(A) */
	OPFUNC(0x2A, base, z80_ld_hl_indA);	/* ld hl,(A) */
	OPFUNC(0x22, base, z80_ld_A_hl);	/* ld (A),hl */
	OPFUNC_i(0x22, z80_ld_A_I);		/* ld (A),I */
	OPFUNC(0xF9, base, z80_ld_sp_hl);	/* ld sp,hl */
	OPFUNC_i(0xF9, z80_ld_sp_I);		/* ld sp,I */
	OPFUNC(0xA8, extended, z80_ldd);	/* ldd */
	OPFUNC(0xB8, extended, z80_lddr);	/* lddr */
	OPFUNC(0xA0, extended, z80_ldi);	/* ldi */
	OPFUNC(0xB0, extended, z80_ldir);	/* ldir */
}

static void z_init_opcode_neg(void)
{
	FORBITS(xxx)				/* neg */
		OPFUNC(0x44 | (xxx << 3), extended, z80_neg);
}

static void z_init_opcode_or(void)
{
	FORBITS(rrr)				/* or R|(hl) */
		OPFUNC(0xB0 | rrr, base, z80_or_R);
	FORBITS(b)
		OPFUNC_i(0xB4 | b, z80_or_J);	/* or J */
	OPFUNC(0xF6, base, z80_or_N);		/* or N */
	OPFUNC_i(0xB6, z80_or_I_D);		/* or (I+D) */
}

static void z_init_opcode_out(void)
{
	OPFUNC(0xD3, base, z80_out_N_a);	/* out (N),a */
	FORBITS(rrr)				/* out (c),R|0 */
		OPFUNC(0x41 | (rrr << 3), extended, z80_out_c_R);
	OPFUNC(0xAB, extended, z80_outd);	/* outd */
	OPFUNC(0xBB, extended, z80_otdr);	/* otdr */
	OPFUNC(0xA3, extended, z80_outi);	/* outi */
	OPFUNC(0xB3, extended, z80_otir);	/* otir */
}

static void z_init_opcode_pop_push(void)
{
	FORBITS(pp) {				/* pop|push P */
		OPFUNC(0xC1 | (pp << 4), base, z80_pop_P);
		OPFUNC(0xC5 | (pp << 4), base, z80_push_P);
	}
	OPFUNC_i(0xE1, z80_pop_I);		/* pop I */
	OPFUNC_i(0xE5, z80_push_I);		/* push I */
}

static void z_init_opcode_res(void)
{
	FORBITS(bbbrrr) {			/* res B,R|(hl) */
		OPFUNC(0x80 | bbbrrr, bits, z80_res_B_R);
		if ((bbbrrr & 7) != 6)		/* res B,(I+D)->R */
			OPFUNC_ibits(0x80 | bbbrrr, z80_res_B_I_D_R);
		else				/* res B,(I+D) */
			OPFUNC_ibits(0x80 | bbbrrr, z80_res_B_I_D);
	}
}

static void z_init_opcode_ret(void)
{
	OPFUNC(0xC9, base, z80_ret);		/* ret */
	FORBITS(ccc)				/* ret C */
		OPFUNC(0xC0 | (ccc << 3), base, z80_ret_C);
	FORBITS(xx) {				/* reti|retn */
		OPFUNC(0x4D | (xx << 4), extended, z80_reti);
		OPFUNC(0x45 | (xx << 4), extended, z80_retn);
	}
}

static void z_init_opcode_rl_rr(void)
{
	OPFUNC(0x17, base, z80_rla);		/* rla */
	OPFUNC(0x07, base, z80_rlca);		/* rlca */
	OPFUNC(0x6F, extended, z80_rld);	/* rld */
	OPFUNC(0x1F, base, z80_rra);		/* rra */
	OPFUNC(0x0F, base, z80_rrca);		/* rrca */
	OPFUNC(0x67, extended, z80_rrd);	/* rrd */
	FORBITS(rrr) {				/* rl|rr|rlc|rrc R */
		OPFUNC(0x10 | rrr, bits, z80_rl_R);
		OPFUNC(0x00 | rrr, bits, z80_rlc_R);
		OPFUNC(0x18 | rrr, bits, z80_rr_R);
		OPFUNC(0x08 | rrr, bits, z80_rrc_R);
		if ((rrr & 7) != 6) {		/* rl|rr|rlc|rrc (I+D)->R */
			OPFUNC_ibits(0x10 | rrr, z80_rl_I_D_R);
			OPFUNC_ibits(0x00 | rrr, z80_rlc_I_D_R);
			OPFUNC_ibits(0x18 | rrr, z80_rr_I_D_R);
			OPFUNC_ibits(0x08 | rrr, z80_rrc_I_D_R);
		} else {			/* rl|rr|rlc|rrc (I+D) */
			OPFUNC_ibits(0x10 | rrr, z80_rl_I_D);
			OPFUNC_ibits(0x00 | rrr, z80_rlc_I_D);
			OPFUNC_ibits(0x18 | rrr, z80_rr_I_D);
			OPFUNC_ibits(0x08 | rrr, z80_rrc_I_D);
		}
	}
}

static void z_init_opcode_rst(void)
{
	FORBITS(sss)				/* rst S */
		OPFUNC(0xC7 | (sss << 3), base, z80_rst_S);
}

static void z_init_opcode_sbc(void)
{
	FORBITS(rrr)				/* sbc a,R|(hl) */
		OPFUNC(0x98 | rrr, base, z80_sbc_a_R);
	FORBITS(b)				/* sbc a,J */
		OPFUNC_i(0x9C | b, z80_sbc_a_J);
	OPFUNC(0xDE, base, z80_sbc_a_N);	/* sbc a,N */
	OPFUNC_i(0x9E, z80_sbc_a_I_D);		/* sbc a,(I+D) */
	FORBITS(qq)				/* sbc hl,Q */
		OPFUNC(0x42 | (qq << 4), extended, z80_sbc_hl_Q);
}

static void z_init_opcode_set(void)
{
	FORBITS(bbbrrr) {
		OPFUNC(0xC0 | bbbrrr, bits, z80_set_B_R);	/* set B,R|(hl) */
		if ((bbbrrr & 7) != 6)
			OPFUNC_ibits(0xC0 | bbbrrr, z80_set_B_I_D_R);	/* set B,(I+D)->R */
		else
			OPFUNC_ibits(0xC0 | bbbrrr, z80_set_B_I_D);	/* set B,(I+D) */
	}
}

static void z_init_opcode_sla_sra_sll_srl(void)
{
	FORBITS(rrr) {
		OPFUNC(0x20 | rrr, bits, z80_sla_R);			/* sla R|(hl) */
		OPFUNC(0x28 | rrr, bits, z80_sra_R);			/* sra R|(hl) */
		OPFUNC(0x30 | rrr, bits, z80_sll_R);			/* sll R|(hl) */
		OPFUNC(0x38 | rrr, bits, z80_srl_R);			/* srl R|(hl) */
		if ((rrr & 7) != 6) {
			OPFUNC_ibits(0x20 | rrr, z80_sla_I_D_R);	/* sla (I+D)->R */
			OPFUNC_ibits(0x28 | rrr, z80_sra_I_D_R);	/* sra (I+D)->R */
			OPFUNC_ibits(0x30 | rrr, z80_sll_I_D_R);	/* sll (I+D)->R */
			OPFUNC_ibits(0x38 | rrr, z80_srl_I_D_R);	/* srl (I+D)->R */
		} else {	
			OPFUNC_ibits(0x20 | rrr, z80_sla_I_D);		/* sla (I+D) */
			OPFUNC_ibits(0x28 | rrr, z80_sra_I_D);		/* sra (I+D) */
			OPFUNC_ibits(0x30 | rrr, z80_sll_I_D);		/* sll (I+D) */
			OPFUNC_ibits(0x38 | rrr, z80_srl_I_D);		/* srl (I+D) */
		}
	}
}

static void z_init_opcode_sub(void)
{
	FORBITS(rrr)
		OPFUNC(0x90 | rrr, base, z80_sub_R);	/* sub R|(hl) */
	FORBITS(b)
		OPFUNC_i(0x94 | b, z80_sub_J);		/* sub J */
	OPFUNC(0xD6, base, z80_sub_N);			/* sub N */
	OPFUNC_i(0x96, z80_sub_I_D);			/* sub (I+D) */
}

static void z_init_opcode_xor(void)
{
	FORBITS(rrr)
		OPFUNC(0xA8 | rrr, base, z80_xor_R);	/* xor R|(hl) */
	FORBITS(b)
		OPFUNC_i(0xAC | b, z80_xor_J);		/* xor J */
	OPFUNC(0xEE, base, z80_xor_N);			/* xor N */
	OPFUNC_i(0xAE, z80_xor_I_D);			/* xor (I+D) */
}

void z_init_opcode_table(struct z80 *z80)
{
	/* extension tables */
	OPFUNC(0xED, base, z_extended);
	OPFUNC(0xCB, base, z_bits);
	OPFUNC(0xDD, base, z_ix);
	OPFUNC(0xCB, ix, z_ix_bits);
	OPFUNC(0xFD, base, z_iy);
	OPFUNC(0xCB, iy, z_iy_bits);

	/* unused opcodes */
	z_init_opcode_unused();

	/* instructions */
	z_init_opcode_adc_add();
	z_init_opcode_and();
	z_init_opcode_bit();
	z_init_opcode_call();
	OPFUNC(0x3F, base, z80_ccf);
	z_init_opcode_cp();
	OPFUNC(0x27, base, z80_daa);
	z_init_opcode_dec();
	OPFUNC(0xF3, base, z80_di);
	OPFUNC(0x10, base, z80_djnz_E);
	OPFUNC(0xFB, base, z80_ei);
	z_init_opcode_ex();
	OPFUNC(0x76, base, z80_halt);
	z_init_opcode_im();
	z_init_opcode_in();
	z_init_opcode_inc();
	z_init_opcode_ind_ini();
	z_init_opcode_jumps();
	z_init_opcode_ld();
	z_init_opcode_neg();
	OPFUNC(0x00, base, z80_nop);
	z_init_opcode_or();
	z_init_opcode_out();
	z_init_opcode_pop_push();
	z_init_opcode_res();
	z_init_opcode_ret();
	z_init_opcode_rl_rr();
	z_init_opcode_rst();
	z_init_opcode_sbc();
	OPFUNC(0x37, base, z80_scf);
	z_init_opcode_set();
	z_init_opcode_sla_sra_sll_srl();
	z_init_opcode_sub();
	z_init_opcode_xor();

	/* check unassigned opcodes */
	for (size_t i = 0; i < 256; i++) {
		if (!optbl_base[i].func)
			WARN("Unassigned opcode:     base [ 0x%02zX ]", i);
		if (!optbl_extended[i].func)
			WARN("Unassigned opcode: extended [ 0x%02zX ]", i);
		if (!optbl_bits[i].func)
			WARN("Unassigned opcode:     bits [ 0x%02zX ]", i);
		if (!optbl_ix[i].func)
			WARN("Unassigned opcode:       ix [ 0x%02zX ]", i);
		if (!optbl_iy[i].func)
			WARN("Unassigned opcode:       iy [ 0x%02zX ]", i);
		if (!optbl_ixbits[i].func)
			WARN("Unassigned opcode:   ixbits [ 0x%02zX ]", i);
		if (!optbl_iybits[i].func)
			WARN("Unassigned opcode:   iybits [ 0x%02zX ]", i);
	}

	z80->opcodes = optbl_base;
}
