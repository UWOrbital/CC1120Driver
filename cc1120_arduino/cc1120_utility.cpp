/**
 * @brief Gets the nth to kth bits of an unsigned integer.
 *
 * @param number - The number to get bits from.
 * @param start - The lowest bit to get (0-indexed).
 * @param end - the highest bit to get (0-indexed).
 * @return The integer equal to the slice of bits taken.
 */
int NtoKBits(int number, int start, int end)
{
    return (number >> start) & ((1 << (end + 1)) - 1);
}
