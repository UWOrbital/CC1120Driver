#ifndef SETUP_TRANSMIT_CC1120_H
#define SETUP_TRANSMIT_CC1120_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Sets modulation format to 2-GFSK.
 *
 * @return true - If the operation was successful
 */
bool setGSFK();

/**
 * @brief Sets symbol rate.
 *
 * @param symbolRate - The symbol rate to set to, in symbols per second.
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool setSymbolRate(uint32_t symbolRate);

/**
 * @brief Sets default state to transition to after receiving/transmitting a good packet.
 *
 * @param mode - The mode to transition to. Must be one of IDLE/FSTXON/TX/RX.
 * @param state - The state to configure. Must be RX or TX.
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool configureTransition(char mode[], char state[]);

/**
 * @brief Flushes the RX FIFO. Only issue in IDLE or RX_FIFO_ERR states.
 *
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool flushRX();

/**
 * @brief Flushes the TX FIFO. Only issue in IDLE or TX_FIFO_ERR states.
 *
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool flushTX();

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
bool setRFBandAndLODivider(uint8_t setting);

/**
 * @brief Sends STX command strobe.
 *
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool sendSTXStrobe();

/**
 * @brief Sends SRX command strobe.
 *
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool sendSRXStrobe();

/**
 * @brief Sends SPWD command strobe.
 *
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool sendSPWDStrobe();

/**
 * @brief Sends SWOR command strobe.
 *
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool sendSWORStrobe();

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
bool setCheckSumConfig(uint8_t setting);

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
bool setAddressCheckConfig(uint8_t setting);

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
bool configLengthField(uint8_t setting, uint8_t numberOfBits);

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
bool configSync(uint8_t syncMode, uint8_t syncNumError, uint32_t syncWord);

/**
 * @brief Sets preamble configuration.
 *
 * @param preambleWord - Minimum number of preamble bits to be transmitted
 * @param numPreamble - Specific combination of bits for preamble word.
 *
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool configPreamble(uint8_t preambleWord, uint8_t numPreamble);

/**
 * @brief Sets VCO frequency.
 *
 * @param frequency - The frequency to set to.
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool setVCOFrequency(uint32_t frequency);

/**
 * @brief Sets RF frequency.
 *
 * @param frequency - The frequency to set to.
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool setRFFrequency(uint32_t frequency);

#endif /* SETUP_TRANSMIT_CC1120_H */
