#include "cc1120_setup_transmit.h"

#include "cc1120_utility.h"
#include "cc1120_spi.h"
#include "cc1120_regs.h"
#include <math.h>
#include <string.h>

/**
 * @brief Sets modulation format to 2-GFSK.
 *
 */
bool setGSFK()
{
    // 7:6 MODEM_MODE: 00 (Normal mode)
    // 5:3 MOD_FORMAT: 001 (2-GFSK)
    // 2:0 DEV_E: 011
    uint8_t data = 0b00001011;
    return cc1120_write_spi(CC1120_REGS_MODCFG_DEV_E, &data, 1);
}

/**
 * @brief Sets symbol rate.
 *
 * @param symbolRate - The symbol rate to set to, in symbols per second.
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool setSymbolRate(uint32_t symbolRate)
{
    uint8_t STRATE_E = 0;
    uint32_t STRATE_M = (pow(2, 38) * ((double)symbolRate / 1000)) / FXOSC;

    if (STRATE_M > pow(2, 20) - 1)
    {
        // Larger than largest 20 bit unsigned integer
        return false;
    }

    uint8_t data1 = STRATE_M << 4 || extract_bits(STRATE_M, 16, 19);
    uint8_t data2 = extract_bits(STRATE_M, 8, 15);
    uint8_t data3 = extract_bits(STRATE_M, 0, 7);

    bool succeeded = true;
    succeeded = succeeded && cc1120_write_spi(CC1120_REGS_SYMBOL_RATE2, &data1, 1);
    succeeded = succeeded && cc1120_write_spi(CC1120_REGS_SYMBOL_RATE1, &data2, 1);
    succeeded = succeeded && cc1120_write_spi(CC1120_REGS_SYMBOL_RATE0, &data3, 1);
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
bool configureTransition(char mode[], char state[])
{
    uint8_t registerUsed;
    if (!strcmp("TX", state))
    {
        registerUsed = CC1120_REGS_RFEND_CFG0;
    }
    else if (!strcmp("RX", state))
    {
        registerUsed = CC1120_REGS_RFEND_CFG1;
    }
    else
    {
        // Invalid argument
        return false;
    }

    uint8_t code;
    if (!strcmp("IDLE", mode))
    {
        code = 0;
    }
    else if (!strcmp("FSTXON", mode))
    {
        code = 1;
    }
    else if (!strcmp("TX", mode))
    {
        code = 2;
    }
    else if (!strcmp("RX", mode))
    {
        code = 3;
    }
    else
    {
        // Invalid argument
        return false;
    }

    uint8_t readData;
    if (!cc1120_read_spi(registerUsed, &readData, 1))
    {
        return false;
    }
    uint8_t data = code << 4 | readData;
    return cc1120_write_spi(registerUsed, &data, 1);
}

/**
 * @brief Flushes the RX FIFO. Only issue in IDLE or RX_FIFO_ERR states.
 *
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool flushRX()
{
    return cc1120_strobe_spi(CC1120_STROBE_SFRX);
}

/**
 * @brief Flushes the TX FIFO. Only issue in IDLE or TX_FIFO_ERR states.
 *
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool flushTX()
{
    return cc1120_strobe_spi(CC1120_STROBE_SFTX);
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
bool setRFBandAndLODivider(uint8_t setting)
{
    uint8_t readData;
    if (!cc1120_read_spi(CC1120_REGS_FS_CFG, &readData, 1))
    {
        return false;
    }
    uint8_t data = setting | readData;
    return cc1120_write_spi(CC1120_REGS_FS_CFG, &data, 1);
}

/**
 * @brief Sends STX command strobe.
 *
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool sendSTXStrobe()
{
    return cc1120_strobe_spi(CC1120_STROBE_STX);
}

/**
 * @brief Sends SRX command strobe.
 *
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool sendSRXStrobe()
{
    return cc1120_strobe_spi(CC1120_STROBE_SRX);
}

/**
 * @brief Sends SPWD command strobe.
 *
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool sendSPWDStrobe()
{
    return cc1120_strobe_spi(CC1120_STROBE_SPWD);
}

/**
 * @brief Sends SWOR command strobe.
 *
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool sendSWORStrobe()
{
    return cc1120_strobe_spi(CC1120_STROBE_SWOR);
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
bool setCheckSumConfig(uint8_t setting)
{
    uint8_t readData;
    if (!cc1120_read_spi(CC1120_REGS_PKT_CFG1, &readData, 1))
    {
        return false;
    }
    uint8_t data = setting << 2 | readData;
    return cc1120_write_spi(CC1120_REGS_PKT_CFG1, &data, 1);
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
bool setAddressCheckConfig(uint8_t setting)
{
    uint8_t readData;
    if (!cc1120_read_spi(CC1120_REGS_PKT_CFG1, &readData, 1))
    {
        return false;
    }
    uint8_t data = setting << 4 | readData;
    return cc1120_write_spi(CC1120_REGS_PKT_CFG1, &data, 1);
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
bool configLengthField(uint8_t setting, uint8_t numberOfBits)
{
    uint8_t readData;
    if (!cc1120_read_spi(CC1120_REGS_PKT_CFG0, &readData, 1))
    {
        return false;
    }
    uint8_t data = setting << 5 | numberOfBits << 2 | readData;
    return cc1120_write_spi(CC1120_REGS_PKT_CFG0, &data, 1);
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
bool configSync(uint8_t syncMode, uint8_t syncNumError, uint32_t syncWord)
{
    uint8_t readData;
    if (!cc1120_read_spi(CC1120_REGS_SYNC_CFG0, &readData, 1))
    {
        return false;
    }
    uint8_t data = syncNumError | syncMode << 2 | readData;
    if (!cc1120_write_spi(CC1120_REGS_SYNC_CFG0, &data, 1))
    {
        return false;
    }

    uint8_t data1 = extract_bits(syncWord, 24, 31);
    uint8_t data2 = extract_bits(syncWord, 16, 23);
    uint8_t data3 = extract_bits(syncWord, 8, 15);
    uint8_t data4 = extract_bits(syncWord, 0, 7);

    bool succeeded = true;
    succeeded = succeeded && cc1120_write_spi(CC1120_REGS_SYNC3, &data1, 1);
    succeeded = succeeded && cc1120_write_spi(CC1120_REGS_SYNC2, &data2, 1);
    succeeded = succeeded && cc1120_write_spi(CC1120_REGS_SYNC1, &data3, 1);
    succeeded = succeeded && cc1120_write_spi(CC1120_REGS_SYNC0, &data4, 1);
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
bool configPreamble(uint8_t preambleWord, uint8_t numPreamble)
{
    uint8_t readData;
    if (!cc1120_read_spi(CC1120_REGS_PREAMBLE_CFG1, &readData, 1))
    {
        return false;
    }
    uint8_t data = numPreamble << 2 | preambleWord | readData;
    return cc1120_write_spi(CC1120_REGS_PREAMBLE_CFG1, &data, 1);
}

/**
 * @brief Sets VCO frequency.
 *
 * @param frequency - The frequency to set to.
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool setVCOFrequency(uint32_t frequency)
{
    uint32_t FREQ = (pow(2, 16) * frequency) / FXOSC;

    if (FREQ > pow(2, 24) - 1)
    {
        // Larger than largest 24 bit unsigned integer
        return false;
    }

    uint8_t data1 = extract_bits(FREQ, 16, 23);
    uint8_t data2 = extract_bits(FREQ, 8, 15);
    uint8_t data3 = extract_bits(FREQ, 0, 7);

    bool succeeded = true;
    succeeded = succeeded && cc1120_write_ext_addr_spi(CC1120_REGS_EXT_FREQ2, &data1, 1);
    succeeded = succeeded && cc1120_write_ext_addr_spi(CC1120_REGS_EXT_FREQ1, &data2, 1);
    succeeded = succeeded && cc1120_write_ext_addr_spi(CC1120_REGS_EXT_FREQ0, &data3, 1);
    return succeeded;
}

/**
 * @brief Sets RF frequency.
 *
 * @param frequency - The frequency to set to.
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool setRFFrequency(uint32_t frequency)
{
    uint8_t readData;
    if (!cc1120_read_spi(CC1120_REGS_FS_CFG, &readData, 1))
    {
        return false;
    }
    uint8_t bandSelectSetting = extract_bits(readData, 0, 3);
    uint8_t LODivider;

    if (bandSelectSetting == 11)
    {
        LODivider = 24;
    }
    else
    {
        LODivider = 2 * bandSelectSetting;
    }

    return setVCOFrequency(frequency * LODivider);
}
