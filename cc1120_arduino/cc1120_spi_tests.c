#include "cc1120_spi_tests.h"
#include "cc1120_spi.h"
#include "cc1120_regs.h"
#include "cc1120_mcu.h"
#include <string.h>

union cc_st ccstatus;

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
bool cc1120_test_spi_read() {
    bool status = true;
    uint8_t addr = 0x00U;
    uint8_t data;

    if (status) {
        addr = 0x00U;
        uint8_t burstData[CC1120_REGS_EXT_ADDR];
        if (!cc1120_read_spi(addr, burstData, CC1120_REGS_EXT_ADDR) ||
            memcmp(CC1120_REGS_DEFAULTS, burstData, CC1120_REGS_EXT_ADDR) != 0) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 burst read test failed.\n");
            status = false;
        }
    }
    
    if (status) {
        if (!cc1120_read_ext_addr_spi(CC1120_REGS_EXT_MARCSTATE, &data, 1)) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 read test failed.\n");
            status = false;
        } else if (data != 0x41U) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 read test failed.\n");
            mcu_log(CC1120_LOG_LEVEL_ERROR, "MARCSTATE read 0x%02X, expected 0x%02X\n", data, 0x41U);
            status = false;
        }
    }

    if (status) {
        uint8_t extBurstData[3];
        uint8_t expected[3] = {0x00U, 0x00U, 0x00U};
        if (!cc1120_read_ext_addr_spi(CC1120_REGS_EXT_FREQ2, extBurstData, 3) ||
            memcmp(extBurstData, expected, 3) != 0) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 burst read test failed.\n");
            status = false;
        }
    }

    if (status)
        mcu_log(CC1120_LOG_LEVEL_INFO, "CC1120 SPI read test passed.\n");

    return status;
}

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
bool cc1120_test_spi_write() {
    bool status = true;
    uint8_t w_data = 0xFFU;
    uint8_t r_data;
    if (!cc1120_write_spi(CC1120_REGS_EXT_FREQOFF0, &w_data, 1) || !cc1120_read_spi(CC1120_REGS_EXT_FREQOFF0, &r_data, 1)) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 write test failed.\n");
        status = false;
    } else if (r_data != 0xFFU) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI write test failed\n");
        mcu_log(CC1120_LOG_LEVEL_ERROR, "FREQOFF0 read 0x%02X, expected 0x%02X\n", r_data, 0xFFU);
        status = false;
    }

    if (status) {
        uint8_t burstData[4] = {0xFFU, 0xFFU, 0xFFU, 0xFFU};
        uint8_t burstDataRead[4];
        if (!cc1120_write_spi(CC1120_REGS_SYNC3, burstData, 4) ||
            !cc1120_read_spi(CC1120_REGS_SYNC3, burstDataRead, 4) ||
            memcmp(burstData, burstDataRead, 4) != 0) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 burst write test failed.\n");
            status = false;
        }
    }

    if (status) {
        w_data = 0x80U;
        if (!cc1120_write_ext_addr_spi(CC1120_REGS_EXT_FREQ1, &w_data, 1) || !cc1120_read_ext_addr_spi(CC1120_REGS_EXT_FREQ1, &r_data, 1)) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI write test failed.\n");
            status = false;
        } else if (r_data != 0x80U) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI write test failed.\n");
            mcu_log(CC1120_LOG_LEVEL_ERROR, "FREQ1 read 0x%02X, expected 0x%02X\n", r_data, 0x80U);
            status = false;
        }
    }

    if (status) {
        uint8_t extBurstData[3] = {0x70U, 0x80U, 0x00U};
        uint8_t extBurstRead[3];
        if (!cc1120_write_ext_addr_spi(CC1120_REGS_EXT_FREQ2, extBurstData, 3) ||
            !cc1120_read_ext_addr_spi(CC1120_REGS_EXT_FREQ2, extBurstRead, 3) ||
            memcmp(extBurstData, extBurstRead, 3) != 0) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI write test failed.\n");
            status = false;
        }
    }
    
    if (status)
        mcu_log(CC1120_LOG_LEVEL_INFO, "CC1120 SPI write test passed.\n");
    
    return status;
}

/**
 * @brief E2E test for SPI strobe functionality.
 * Runs the reset strobe and checks the MARCSTATE register.
 * 
 * @return true - If MARCSTATE is 0x41 after reset.
 * @return false - If MARCSTATE is not 0x41 after reset, or status byte is invalid.
 */
bool cc1120_test_spi_strobe() {
    bool status = true;
    uint8_t data;
    
    if (!cc1120_strobe_spi(CC1120_STROBE_SRES) || !cc1120_read_ext_addr_spi(CC1120_REGS_EXT_MARCSTATE, &data, 1)) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI strobe test failed.\n");
    } else if (data != 0x41U) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI strobe test failed.\n");
        mcu_log(CC1120_LOG_LEVEL_ERROR, "MARCSTATE read 0x%02X, expected 0x%02X\n", data, 0x41U);
        status = false;
    }
    
    if (status)
        mcu_log(CC1120_LOG_LEVEL_INFO, "CC1120 SPI strobe test passed.\n");
    
    return status;    
}

/**
 * @brief - E2E test for the CC1120's FIFO read and write functionality.
 * Writes to the FIFO and then does a direct read to see if the write was successful.
 * Wries directly to the FIFO and then reads the FIFO to see if the write was successful.
 * 
 * @return true - If the FIFO read and write tests pass.
 * @return false - If the FIFO read and write tests fail.
 */
bool cc1120_test_fifo_read_write() {
    bool status = true;
    uint8_t w_data = 0x0AU;
    uint8_t r_data;
    if (!cc1120_write_fifo(&w_data, 1) || !cc1120_read_fifo_direct(CC1120_FIFO_TX_START, &r_data, 1)) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
        status = false;
    } else if (r_data != 0x0AU) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
        mcu_log(CC1120_LOG_LEVEL_ERROR, "FIFO_TX_START read 0x%02X, expected 0x%02X\n", r_data, 0x0AU);
        status = false;
    }

    

    if (status) {
        uint8_t burstWriteData[3] = {0x0BU, 0x0CU, 0x0DU};
        uint8_t burstReadData[3];
        if (!cc1120_write_fifo(burstWriteData, 3) ||
            !cc1120_read_fifo_direct(CC1120_FIFO_TX_START+1, burstReadData, 3) ||
            memcmp(burstWriteData, burstReadData, 3) != 0) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
            status = false;
        }
    }

    if (status) {
        uint8_t rxLastPos = 5;
        /* When the size of the RXFIFO is 0, the first byte must be ignored. See section 3.2.3 of datasheet. */
        uint8_t ignore;
        w_data = 0x0EU;
        if (!cc1120_write_ext_addr_spi(CC1120_REGS_EXT_RXLAST, &rxLastPos, 1) ||
                !cc1120_write_fifo_direct(CC1120_FIFO_RX_START, &w_data, 1) ||
                !cc1120_read_fifo(&ignore, 1) ||
                !cc1120_read_fifo(&r_data, 1)) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
            status = false;
        } else if (r_data != 0x0EU) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
            mcu_log(CC1120_LOG_LEVEL_ERROR, "FIFO_RX_START read 0x%02X, expected 0x%02X\n", r_data, 0x0EU);
            status = false;
        }
    }

    if (status) {
        uint8_t burstWriteData2[3] = {0x0FU, 0x10U, 0x11U};
        uint8_t burstReadData2[3];
        if (!cc1120_write_fifo_direct(CC1120_FIFO_RX_START+1, burstWriteData2, 3) ||
            !cc1120_read_fifo(burstReadData2, 3) ||
            memcmp(burstWriteData2, burstReadData2, 3) != 0) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
            status = false;
        }
    }

    if (status)
        mcu_log(CC1120_LOG_LEVEL_INFO, "CC1120 FIFO test passed.\n");
    
    return status;
}
