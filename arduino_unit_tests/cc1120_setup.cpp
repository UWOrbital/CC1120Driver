#include "cc1120_setup.h"

#include "cc1120_spi.h"
#include "cc1120_regs.h"
#include <math.h>
#include <stdbool.h>
#include <string.h>

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
    succeeded = succeeded && arduinoWriteSPI(SYMBOL_RATE2, STRATE_M << 4 || NtoKBits(STRATE_M, 16, 19));
    succeeded = succeeded && arduinoWriteSPI(SYMBOL_RATE1, NtoKBits(STRATE_M, 8, 15));
    succeeded = succeeded && arduinoWriteSPI(SYMBOL_RATE0, NtoKBits(STRATE_M, 0, 7));
    return succeeded;
}

/**
 * @brief Sets default state to transition to after receiving/transmitting a good packet.
 * 
 * @param mode - The mode to transition to. Must be one of IDLE/FSTXON/TX/RX.
 * @param state - The state to configure. Must be RX or TX.
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool configureTransition(char mode[], char state[]) {
    int registerUsed;
    if (!strcmp("TX", state)) {
        registerUsed = RFEND_CFG0;
    } else if (!strcmp("RX", state)) {
        registerUsed = RFEND_CFG1;
    } else {
        // Invalid argument
        return false;
    }

    int code;
    if (!strcmp("IDLE", mode)) {
        code = 0;
    } else if (!strcmp("FSTXON", mode)) {
        code = 1;
    } else if (!strcmp("TX", mode)) {
        code = 2;
    } else if (!strcmp("RX", mode)) {
        code = 3;
    } else {
        // Invalid argument
        return false;
    }

    uint8_t readData;
    if (!arduinoReadSPI(registerUsed, &readData)) {
        return false;
    }
    return arduinoWriteSPI(registerUsed, code << 4 | readData);
}

/**
 * @brief Flushes the RX FIFO. Only issue in IDLE or RX_FIFO_ERR states.
 * 
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool flushRX() {
    uint8_t readData;
    return arduinoReadSPI(SFRX, &readData);
}

/**
 * @brief Flushes the TX FIFO. Only issue in IDLE or TX_FIFO_ERR states.
 * 
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool flushTX() {
    uint8_t readData;
    return arduinoReadSPI(SFTX, &readData);
}

/**
 * @brief Sets band select setting for LO divider.
 * 
 * @param setting - One of the following values:
 * 
 * 2:  820.0 - 960.0 MHz band (LO divider = 4)
 * 4:  410.0 - 480.0 MHz band (LO divider = 8)
 * 6:  273.3 - 320.0 MHz band (LO divider = 12)
 * 8:  205.0 - 240.0 MHz band (LO divider = 16)
 * 10: 164.0 - 192.0 MHz band (LO divider = 20)
 * 11: 136.7 - 160.0 MHz band (LO divider = 24)
 * 
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool setRFBandAndLODivider(int setting) {
    uint8_t readData;
    if (!arduinoReadSPI(FS_CFG, &readData)) {
        return false;
    }
    return arduinoWriteSPI(FS_CFG, setting | readData);
}

/**
 * @brief Sends STX command strobe.
 * 
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool sendSTXStrobe() {
    uint8_t readData;
    return arduinoReadSPI(STX, &readData);
}

/**
 * @brief Sends SRX command strobe.
 * 
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool sendSRXStrobe() {
    uint8_t readData;
    return arduinoReadSPI(SRX, &readData);
}

/**
 * @brief Sends SPWD command strobe.
 * 
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool sendSPWDStrobe() {
    uint8_t readData;
    return arduinoReadSPI(SPWD, &readData);
}

/**
 * @brief Sends SWOR command strobe.
 * 
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool sendSWORStrobe() {
    uint8_t readData;
    return arduinoReadSPI(SWOR, &readData);
}

/**
 * @brief Sets check sum configuration.
 * 
 * @param setting - One of the following values:
 * 
 * 0: CRC disabled for TX and RX
 * 1: CRC calculation in TX mode and CRC check in RX mode enabled. 
 * CRC16(X^16+X^15+X^2+1). Initialized to 0xFFFF
 * 2: CRC calculation in TX mode and CRC check in RX mode enabled. 
 * CRC16(X^16+X^12+X^5+1). Initialized to 0x0000
 * 
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool setCheckSumConfig(int setting) {
    uint8_t readData;
    if (!arduinoReadSPI(PKT_CFG1, &readData)) {
        return false;
    }
    return arduinoWriteSPI(PKT_CFG1, setting << 2 | readData);
}

/**
 * @brief Sets address check configuration.
 * 
 * @param setting - One of the following values:
 * 
 * 0: No address check
 * 1: Address check, no broadcast
 * 2: Address check, 0x00 broadcast
 * 3: Address check, 0x00 and 0xFF broadcast
 * 
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool setAddressCheckConfig(int setting) {
    uint8_t readData;
    if (!arduinoReadSPI(PKT_CFG1, &readData)) {
        return false;
    }
    return arduinoWriteSPI(PKT_CFG1, setting << 4 | readData);
}

/**
 * @brief Sets length field configuration.
 * 
 * @param setting - One of the following values:
 * 
 * 0: Fixed packet length mode. Packet length configured through the PKT_LEN register
 * 1: Variable packet length mode. Packet length configured by the first byte received after sync word
 * 2: Infinite packet length mode
 * 3: Variable packet length mode. Length configured by the 5 LSB of the first byte received after sync word
 * 
 * @param numberOfBits - number of bits added to default number of bytes to read in fixed length mode.
 * 
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool configLengthField(int setting, int numberOfBits) {
    uint8_t readData;
    if (!arduinoReadSPI(PKT_CFG0, &readData)) {
        return false;
    }
    return arduinoWriteSPI(PKT_CFG0, setting << 5 | numberOfBits << 2 | readData);
}

/**
 * @brief Sets sync word configuration.
 * 
 * @param syncMode - Sync word configuration.
 * @param syncNumError - Configure number of bit errors allowable for sync word to be valid.
 * @param syncWord - Sync word. Should fit in 32 bits.
 * 
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool configSync(int syncMode, int syncNumError, long int syncWord) {
    uint8_t readData;
    if (!arduinoReadSPI(SYNC_CFG0, &readData)) {
        return false;
    }
    if (!arduinoWriteSPI(SYNC_CFG0, syncNumError | syncMode << 2 | readData)) {
        return false;
    }

    bool succeeded = true;
    succeeded = succeeded && arduinoWriteSPI(SYNC3, NtoKBits(syncWord, 24, 31));
    succeeded = succeeded && arduinoWriteSPI(SYNC2, NtoKBits(syncWord, 16, 23));
    succeeded = succeeded && arduinoWriteSPI(SYNC1, NtoKBits(syncWord, 8, 15));
    succeeded = succeeded && arduinoWriteSPI(SYNC0, NtoKBits(syncWord, 0, 7));
    return succeeded;
}

/**
 * @brief Sets preamble configuration.
 * 
 * @param preambleWord - Minimum number of preamble bits to be transmitted
 * @param numPreamble - Specific combination of bits for preamble word.
 * 
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool configPreamble(int preambleWord, int numPreamble) {
    uint8_t readData;
    if (!arduinoReadSPI(PREAMBLE_CFG1, &readData)) {
        return false;
    }
    return arduinoWriteSPI(PREAMBLE_CFG1, numPreamble << 2 | preambleWord | readData);
}
