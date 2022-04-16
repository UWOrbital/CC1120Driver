#ifndef UTILITY_CC1120_H
#define UTILITY_CC1120_H

#include <stdint.h>

#define FXOSC 32000000

/**
 * @brief Gets the nth to kth bits of an unsigned integer.
 *
 * @param number - The number to get bits from.
 * @param start - The lowest bit to get (0-indexed, inclusive).
 * @param end - the highest bit to get (0-indexed, inclusive).
 * @return The integer equal to the slice of bits taken.
 */
uint32_t extract_bits(uint32_t number, uint8_t start, uint8_t end);

#endif /* UTILITY_CC1120_H */
