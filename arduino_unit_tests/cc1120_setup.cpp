#include "cc1120_setup.h"

#include "cc1120_spi.h"
#include "cc1120_regs.h"

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
 * @brief Sets data rate to 9600 symbols per second.
 * 
 */
void setDataRate() {
    // STRATE_E = 0, STRATE_M = 0x1421F
    arduinoWriteSPI(SYMBOL_RATE2, 0b00000001);
    arduinoWriteSPI(SYMBOL_RATE1, 0b01000010);
    arduinoWriteSPI(SYMBOL_RATE0, 0b00011111);
}
