#include "cc1120_setup.h"

#include "cc1120_spi.h"
#include "cc1120_regs.h"
#include <math.h>
#include <stdbool.h>

/**
 * @brief Gets the nth to kth bits of an unsigned integer.
 * 
 * @param number - The number to get bits from.
 * @param start - The lowest bit to get (0-indexed).
 * @param end - the highest bit to get (0-indexed).
 * @return The integer equal to the slice of bits taken.
 */
int NtoKBits(int number, int start, int end) {
    return (number >> start) & ((1 << (end + 1)) - 1);
}

/**
 * @brief Sets modulation format to 2-GFSK.
 * 
 */
void setGSFK() {
    // 7:6 MODEM_MODE: 00 (Normal mode)
    // 5:3 MOD_FORMAT: 001 (2-GFSK)
    // 2:0 DEV_E: 011
    arduinoWriteSPI(MODCFG_DEV_E, 0b00001011);
}

/**
 * @brief Sets symbol rate.
 * 
 * @param symbolRate - The symbol rate to set to, in symbols per second.
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool setSymbolRate(int symbolRate) {
    long int FXOSC = 32000000;

    int STRATE_E = 0;
    long int STRATE_M = (pow(2, 38) * ((double)symbolRate / 1000)) / FXOSC;

    if (STRATE_M > pow(2, 20) - 1) {
        // Larger than largest 20 bit unsigned integer
        return false;
    }

    bool succeeded = true;
    succeeded = succeeded & arduinoWriteSPI(SYMBOL_RATE2, STRATE_M << 4 || NtoKBits(STRATE_M, 16, 19));
    succeeded = succeeded & arduinoWriteSPI(SYMBOL_RATE1, NtoKBits(STRATE_M, 8, 15));
    succeeded = succeeded & arduinoWriteSPI(SYMBOL_RATE0, NtoKBits(STRATE_M, 0, 7));
    return succeeded;
}
