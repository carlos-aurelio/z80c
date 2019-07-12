#include "util.h"

uint8_t bitval(uint16_t val, uint8_t bit)
{
	if (val & (1 << bit))
		return 1;
	else
		return 0;
}

int32_t mask(uint8_t bits)
{
	return (1 << bits) - 1;
}


void swap8(uint8_t *r1, uint8_t *r2)
{
	uint8_t tmp = *r1;
	*r1 = *r2;
	*r2 =  tmp;
}

void swap16(uint16_t *r1, uint16_t *r2)
{
	uint16_t tmp = *r1;
	*r1 = *r2;
	*r2 = tmp;
}


uint8_t sum_carry(uint16_t v1, uint16_t v2, uint16_t v3, uint8_t bit)
{
	int32_t iv1 = (int16_t)v1 & mask(bit), iv2 = (int16_t)v2 & mask(bit), iv3 = (int16_t)v3 & mask(bit);
	int32_t sum = iv1 + iv2 + iv3;
	if (sum & (1 << bit))
		return 1;
	else
		return 0;

}

uint8_t odd_parity(uint16_t v)
{
	uint8_t ret = 0; 
	for (uint8_t i = 0; i < sizeof(uint16_t); i++) {
		ret += (v & 1);
		v >>= 1;
	}
	return (ret % 2);
}

uint8_t sum_overflow(uint16_t v1, uint16_t v2, uint16_t v3, uint8_t bits)
{
	return sum_carry(v1, v2, v3, bits - 1) ^ sum_carry(v1, v2, v3, bits);
}
