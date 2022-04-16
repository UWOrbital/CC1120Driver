#include "cc1120_utility.h"

/**
 * @brief Gets the nth to kth bits of an unsigned integer.
 *
 * @param number - The number to get bits from.
 * @param start - The lowest bit to get (0-indexed, inclusive).
 * @param end - the highest bit to get (0-indexed, inclusive).
 * @return The integer equal to the slice of bits taken.
 */
uint32_t extract_bits(uint32_t number, uint8_t start, uint8_t end)
{
    return (number >> start) & ((1 << (end + 1)) - 1);
}
