#ifndef SPI_TESTS_CC1120_H
#define SPI_TESTS_CC1120_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Set the default values for CC1120 registers.
 * 
 */
void setRegisterDefaults();

/**
 * @brief E2E test for SPI read function.
 * Reads through all registers up to the extended register space,
 * and compares values to default values.
 * Burst reads all the values and compares them to the default values.
 * Burst reads FREQ registers in extended address space and compares them to defaults.
 * 
 * @return true - If all registers are read correctly and have the right value.
 * @return false - If any register does not have the expected value,
 *                 or status byte is invalid.
 */
bool checkSPIRead();

/**
 * @brief E2E test for SPI write function.
 * Writes to frequency offset register and extended address space RNDGEN register.
 * Reads the registers to see if the write was successful.
 * Burst writes to the sync word registers, then burst reads to see if the writes were successful.
 * Burst writes to the FREQ registers, then burst reads to see if the writes were successful.
 * 
 * @return true - If both writes are successful.
 * @return false - If any register does not have the expected value,
 *                 or status byte is invalid.
 */
bool checkSPIWrite();

/**
 * @brief E2E test for SPI strobe functionality.
 * Runs the reset strobe and checks the MARCSTATE register.
 * 
 * @return true - If MARCSTATE is 0x41 after reset.
 * @return false - If MARCSTATE is not 0x41 after reset, or status byte is invalid.
 */
bool checkStrobe();

/**
 * @brief - E2E test for the CC1120's FIFO read and write functionality.
 * Writes to the FIFO and then does a direct read to see if the write was successful.
 * Wries directly to the FIFO and then reads the FIFO to see if the write was successful.
 * 
 * @return true - If the FIFO read and write tests pass.
 * @return false - If the FIFO read and write tests fail.
 */
bool checkFIFOReadWrite();

#endif /* SPI_TESTS_CC1120_H */
