#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>
#include "trace.h"

uint8_t bitval(uint16_t val, uint8_t bit);
int32_t mask(uint8_t bits);
void swap8(uint8_t *r1, uint8_t *r2);
void swap16(uint16_t *r1, uint16_t *r2);
uint8_t sum_carry(uint16_t v1, uint16_t v2, uint16_t v3, uint8_t bit);
uint8_t odd_parity(uint16_t v);
uint8_t sum_overflow(uint16_t v1, uint16_t v2, uint16_t v3, uint8_t bits);

#endif /* __UTIL_H__ */
