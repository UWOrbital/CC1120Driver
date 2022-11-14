#ifndef CC1120_SPI_TESTS_H
#define CC1120_SPI_TESTS_H

#include <stdbool.h>
#include "cc1120_spi.h"
#include "cc1120_logging.h"

/**
 * @brief E2E test for SPI read function.
 * Reads through all registers up to the extended register space,
 * and compares values to default values.
 * Burst reads all the values and compares them to the default values.
 * Burst reads FREQ registers in extended address space and compares them to defaults.
 * 
 * @return CC1120_ERROR_CODE_SUCCESS - If all registers are read correctly and have the right value.
 * @return An error code - If any register does not have the expected value,
 *                 or status byte is invalid.
 */
cc1120_status_code cc1120_test_spi_read();

/**
 * @brief E2E test for SPI write function.
 * Writes to frequency offset register and extended address space RNDGEN register.
 * Reads the registers to see if the write was successful.
 * Burst writes to the sync word registers, then burst reads to see if the writes were successful.
 * Burst writes to the FREQ registers, then burst reads to see if the writes were successful.
 * 
 * @return CC1120_ERROR_CODE_SUCCESS - If both writes are successful.
 * @return An error code - If any register does not have the expected value,
 *                 or status byte is invalid.
 */
cc1120_status_code cc1120_test_spi_write();

/**
 * @brief E2E test for SPI strobe functionality.
 * Runs the reset strobe and checks the MARCSTATE register.
 * 
 * @return CC1120_ERROR_CODE_SUCCESS - If MARCSTATE is 0x41 after reset.
 * @return An error code - If MARCSTATE is not 0x41 after reset, or status byte is invalid.
 */
cc1120_status_code cc1120_test_spi_strobe();

/**
 * @brief - E2E test for the CC1120's FIFO read and write functionality.
 * Writes to the FIFO and then does a direct read to see if the write was successful.
 * Wries directly to the FIFO and then reads the FIFO to see if the write was successful.
 * 
 * @return CC1120_ERROR_CODE_SUCCESS - If the FIFO read and write tests pass.
 * @return An error code - If the FIFO read and write tests fail.
 */
cc1120_status_code cc1120_test_fifo_read_write();

#endif /* CC1120_SPI_TESTS_H */
