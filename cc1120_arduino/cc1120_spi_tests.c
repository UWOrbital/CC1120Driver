#include "cc1120_spi_tests.h"
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
 * @return CC1120_ERROR_CODE_SUCCESS - If all registers are read correctly and have the right value.
 * @return An error code - If any register does not have the expected value,
 *                 or status byte is invalid.
 */
cc1120_error_code cc1120_test_spi_read() { 
    cc1120_error_code status;
    uint8_t addr = 0x00U;
    uint8_t data;
    uint8_t burstData[CC1120_REGS_EXT_ADDR];
    status = cc1120_read_spi(addr, burstData, CC1120_REGS_EXT_ADDR);

    if (status != CC1120_ERROR_CODE_SUCCESS) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 burst read test failed.\n");
        return status;
    }

    if (memcmp(CC1120_REGS_DEFAULTS, burstData, CC1120_REGS_EXT_ADDR)) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 burst read test failed.\n");
        status = CC1120_ERROR_CODE_TEST_SPI_READ_INCORRECT_BURST_READ;
        return status;
    }
    
    if (status == CC1120_ERROR_CODE_SUCCESS) {
        status = cc1120_read_ext_addr_spi(CC1120_REGS_EXT_MARCSTATE, &data, 1);
        
        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 read test failed.\n");
            return status;
        } 
            
        if (data != 0x41U) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 read test failed.\n");
            mcu_log(CC1120_LOG_LEVEL_ERROR, "MARCSTATE read 0x%02X, expected 0x%02X\n", data, 0x41U);
            status = CC1120_ERROR_CODE_TEST_SPI_READ_INCORRECT_SINGLE_EXT_ADDR_READ;
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t extBurstData[3];
        uint8_t expected[3] = {0x00U, 0x00U, 0x00U};
        status = cc1120_read_ext_addr_spi(CC1120_REGS_EXT_FREQ2, extBurstData, 3);

        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 burst read test failed.\n");
            return status;            
        }

        if (memcmp(extBurstData, expected, 3)) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 burst read test failed.\n");
            status = CC1120_ERROR_CODE_TEST_SPI_READ_INCORRECT_BURST_EXT_ADDR_READ;
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS)
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
 * @return CC1120_ERROR_CODE_SUCCESS - If both writes are successful.
 * @return An error code - If any register does not have the expected value,
 *                 or status byte is invalid.
 */
cc1120_error_code cc1120_test_spi_write() {
    cc1120_error_code status;
    uint8_t w_data = 0xFFU;
    uint8_t r_data;

    status = cc1120_write_spi(CC1120_REGS_EXT_FREQOFF0, &w_data, 1);

    if (status != CC1120_ERROR_CODE_SUCCESS) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 write test failed.\n");
        return status;
    } 

    status = cc1120_read_spi(CC1120_REGS_EXT_FREQOFF0, &r_data, 1);

    if (status != CC1120_ERROR_CODE_SUCCESS) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 write test failed.\n");
        return status;
    } 
    
    if (r_data != 0xFFU) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI write test failed\n");
        mcu_log(CC1120_LOG_LEVEL_ERROR, "FREQOFF0 read 0x%02X, expected 0x%02X\n", r_data, 0xFFU);
        status = CC1120_ERROR_CODE_TEST_SPI_WRITE_INCORRECT_SINGLE_ADDR_WRITE; 
        return status;
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t burstData[4] = {0xFFU, 0xFFU, 0xFFU, 0xFFU};
        uint8_t burstDataRead[4];

        status = cc1120_write_spi(CC1120_REGS_SYNC3, burstData, 4);

        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 burst write test failed.\n");
            return status;
        } 

        status = cc1120_read_spi(CC1120_REGS_SYNC3, burstDataRead, 4);

        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 burst write test failed.\n");
            return status;
        }

        if (memcmp(burstData, burstDataRead, 4)) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 burst write test failed.\n");
            status = CC1120_ERROR_CODE_TEST_SPI_WRITE_INCORRECT_BURST_ADDR_WRITE;
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        w_data = 0x80U;

        status = cc1120_write_ext_addr_spi(CC1120_REGS_EXT_FREQ1, &w_data, 1);

        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI write test failed.\n");
            return status;
        } 

        status = cc1120_read_ext_addr_spi(CC1120_REGS_EXT_FREQ1, &r_data, 1);
        
        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI write test failed.\n");
            return status;
        } 
        
        if (r_data != 0x80U) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI write test failed.\n");
            mcu_log(CC1120_LOG_LEVEL_ERROR, "FREQ1 read 0x%02X, expected 0x%02X\n", r_data, 0x80U);
            status = CC1120_ERROR_CODE_TEST_SPI_WRITE_INCORRECT_SINGLE_EXT_ADDR_WRITE;
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t extBurstData[3] = {0x70U, 0x80U, 0x00U};
        uint8_t extBurstRead[3];

        status = cc1120_write_ext_addr_spi(CC1120_REGS_EXT_FREQ2, extBurstData, 3);

        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI write test failed.\n");
            return status;
        } 

        status = cc1120_read_ext_addr_spi(CC1120_REGS_EXT_FREQ2, extBurstRead, 3);

        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI write test failed.\n");
            return status;
        } 
        
        if (memcmp(extBurstData, extBurstRead, 3)) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI write test failed.\n");
            status = CC1120_ERROR_CODE_TEST_SPI_WRITE_INCORRECT_BURST_EXT_ADDR_WRITE;
            return status;
        }
    }
    
    if (status == CC1120_ERROR_CODE_SUCCESS)
        mcu_log(CC1120_LOG_LEVEL_INFO, "CC1120 SPI write test passed.\n");
    
    return status;
}

/**
 * @brief E2E test for SPI strobe functionality.
 * Runs the reset strobe and checks the MARCSTATE register.
 * 
 * @return CC1120_ERROR_CODE_SUCCESS - If MARCSTATE is 0x41 after reset.
 * @return An error code - If MARCSTATE is not 0x41 after reset, or status byte is invalid.
 */
cc1120_error_code cc1120_test_spi_strobe() {
    cc1120_error_code status;
    uint8_t data;
    
    status = cc1120_strobe_spi(CC1120_STROBE_SRES);

    if (status != CC1120_ERROR_CODE_SUCCESS) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI strobe test failed.\n");
        return status;
    }
    status = cc1120_read_ext_addr_spi(CC1120_REGS_EXT_MARCSTATE, &data, 1);

    if (status != CC1120_ERROR_CODE_SUCCESS) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI strobe test failed.\n");
        return status;
    }

    if (data != 0x41U) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 SPI strobe test failed.\n");
        mcu_log(CC1120_LOG_LEVEL_ERROR, "MARCSTATE read 0x%02X, expected 0x%02X\n", data, 0x41U);
        status = CC1120_ERROR_CODE_TEST_SPI_STROBE_INCORRECT_MARCSTATE_READ;
        return status;
    }
    
    if (status == CC1120_ERROR_CODE_SUCCESS)
        mcu_log(CC1120_LOG_LEVEL_INFO, "CC1120 SPI strobe test passed.\n");
    
    return status;    
}

/**
 * @brief - E2E test for the CC1120's FIFO read and write functionality.
 * Writes to the FIFO and then does a direct read to see if the write was successful.
 * Wries directly to the FIFO and then reads the FIFO to see if the write was successful.
 * 
 * @return CC1120_ERROR_CODE_SUCCESS - If the FIFO read and write tests pass.
 * @return An error code - If the FIFO read and write tests fail.
 */
cc1120_error_code cc1120_test_fifo_read_write() {
    cc1120_error_code status;
    uint8_t w_data = 0x0AU;
    uint8_t r_data;

    status = cc1120_write_fifo(&w_data, 1);

    if (status != CC1120_ERROR_CODE_SUCCESS) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
        return status;
    } 
    
    status = cc1120_read_fifo_direct(CC1120_FIFO_TX_START, &r_data, 1);

    if (status != CC1120_ERROR_CODE_SUCCESS) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
        return status;
    } 
    
    else if (r_data != 0x0AU) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
        mcu_log(CC1120_LOG_LEVEL_ERROR, "FIFO_TX_START read 0x%02X, expected 0x%02X\n", r_data, 0x0AU);
        status = CC1120_ERROR_CODE_TEST_FIFO_READ_WRITE_SINGLE_WRITE_DIRECT_READ_FAILED;
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t burstWriteData[3] = {0x0BU, 0x0CU, 0x0DU};
        uint8_t burstReadData[3];

        status = cc1120_write_fifo(burstWriteData, 3);

        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
            return status;
        } 

        status = cc1120_read_fifo_direct(CC1120_FIFO_TX_START+1, burstReadData, 3);

        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
            return status;
        } 

        if (memcmp(burstWriteData, burstReadData, 3)) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
            status = CC1120_ERROR_CODE_TEST_FIFO_READ_WRITE_BURST_WRITE_DIRECT_READ_FAILED;
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t rxLastPos = 5;
        /* When the size of the RXFIFO is 0, the first byte must be ignored. See section 3.2.3 of datasheet. */
        uint8_t ignore;
        w_data = 0x0EU;

        status = cc1120_write_ext_addr_spi(CC1120_REGS_EXT_RXLAST, &rxLastPos, 1);

        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
            return status;
        } 

        status = cc1120_write_fifo_direct(CC1120_FIFO_RX_START, &w_data, 1);

        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
            return status;
        } 

        status = cc1120_read_fifo(&ignore, 1);

        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
            return status;
        } 

        status = cc1120_read_fifo(&r_data, 1);

        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
            return status;
        } 
        
        if (r_data != 0x0EU) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
            mcu_log(CC1120_LOG_LEVEL_ERROR, "FIFO_RX_START read 0x%02X, expected 0x%02X\n", r_data, 0x0EU);
            status = CC1120_ERROR_CODE_TEST_FIFO_READ_WRITE_SINGLE_DIRECT_WRITE_FAILED; // what is happening here?
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t burstWriteData2[3] = {0x0FU, 0x10U, 0x11U};
        uint8_t burstReadData2[3];
        bool incorrectValues = false;
        
        status = cc1120_write_fifo_direct(CC1120_FIFO_RX_START+1, burstWriteData2, 3);

        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
            return status; 
        }

        status = cc1120_read_fifo(burstReadData2, 3);

        if (status != CC1120_ERROR_CODE_SUCCESS) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
            return status; 
        }

        if (memcmp(burstWriteData2, burstReadData2, 3) != 0) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "CC1120 FIFO test failed.\n");
            status = CC1120_ERROR_CODE_TEST_FIFO_READ_WRITE_BURST_DIRECT_WRITE_FAILED;
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS)
        mcu_log(CC1120_LOG_LEVEL_INFO, "CC1120 FIFO test passed.\n");
    
    return status;
}
