#include "cc1120_spi.h"
#include "cc1120_regs.h"
#include "cc1120_mcu.h"

/**
 * @brief - Reads from consecutive registers from the CC1120.
 * 
 * @param addr - The address of the first register to read.
 * @param data - The array to store the read data, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to read.
 * @return CC1120_ERROR_CODE_SUCCESS - If the read was successful.
 * @return An error code - If the register is not valid, or the status byte is invalid.
 */
cc1120_error_code cc1120_read_spi(uint8_t addr, uint8_t data[], uint8_t len) {
    cc1120_error_code status = CC1120_ERROR_CODE_SUCCESS;
    
    if (addr >= CC1120_REGS_EXT_ADDR) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_read_spi: Not a valid register!\n");
        status = CC1120_ERROR_CODE_READ_SPI_INVALID_REGISTER;
        return status;
    }

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_read_spi: Not a valid length!\n");
        status = CC1120_ERROR_CODE_READ_SPI_INVALID_LENGTH;
        return status;
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t header = (len > 1) ? (R_BIT | BURST_BIT | addr) : (R_BIT | addr);

        mcu_cc1120_cs_assert();
        if (cc1120_send_byte_receive_status(header) != CC1120_ERROR_CODE_SUCCESS) {
            status = CC1120_ERROR_CODE_READ_SPI_INVALID_STATUS_BYTE;
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t i;
        for(i = 0; i < len; i++) {
            data[i] = mcu_cc1120_spi_transfer(0x00);
        }
    }

    mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Reads from consecutive extended address space registers on the CC1120
 * 
 * @param addr - The address of the first register to read.
 * @param data - The array to store the read data, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to read.
 * @return CC1120_ERROR_CODE_SUCCESS - If the read was successful.
 * @return An error code - If the register is not valid, or the status byte is invalid.
 */
cc1120_error_code cc1120_read_ext_addr_spi(uint8_t addr, uint8_t data[], uint8_t len) {
    cc1120_error_code status = CC1120_ERROR_CODE_SUCCESS;

    if ((addr > CC1120_REGS_EXT_PA_CFG3 && addr < CC1120_REGS_EXT_WOR_TIME1) ||
        (addr > CC1120_REGS_EXT_XOSC_TEST0 && addr < CC1120_REGS_EXT_RXFIRST) ||
        (addr > CC1120_REGS_EXT_FIFO_NUM_RXBYTES)) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_read_ext_addr_spi: Not a valid register!\n");
        status = CC1120_ERROR_CODE_READ_EXT_ADDR_SPI_INVALID_REGISTER;
        return status;
    }

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_read_ext_addr_spi: Not a valid length!\n");
        status = CC1120_ERROR_CODE_READ_EXT_ADDR_SPI_INVALID_LENGTH;
        return status;
    }
    

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t header = (len > 1) ? (R_BIT | BURST_BIT | CC1120_REGS_EXT_ADDR) :
                                     (R_BIT | CC1120_REGS_EXT_ADDR);

        mcu_cc1120_cs_assert();
        if (cc1120_send_byte_receive_status(header) != CC1120_ERROR_CODE_SUCCESS) {
            status = CC1120_ERROR_CODE_READ_EXT_ADDR_SPI_INVALID_STATUS_BYTE;
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        if (mcu_cc1120_spi_transfer(addr) != 0x00) { // When sending the extended address, SO will return all zeros. See section 3.2.
            mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_read_ext_addr_spi: CC1120_read_ext_addr_spi failed\n");
            status = CC1120_ERROR_CODE_READ_EXT_ADDR_SPI_FAILED;
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t i;
        for(i = 0; i < len; i++) {
            data[i] = mcu_cc1120_spi_transfer(0x00);
        }
    }

    mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Writes to consecutive registers on the CC1120.
 * 
 * @param addr - The address of the first register to write to.
 * @param data - The array of data to write to the registers, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to write.
 * @return CC1120_ERROR_CODE_SUCCESS - If the write was successful.
 * @return An error code - If the register is not valid, or the status byte is invalid.
 */
cc1120_error_code cc1120_write_spi(uint8_t addr, uint8_t data[], uint8_t len) {
    cc1120_error_code status = CC1120_ERROR_CODE_SUCCESS;

    if(addr >= CC1120_REGS_EXT_ADDR) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_spi: Not a valid register!\n");
        status = CC1120_ERROR_CODE_WRITE_SPI_INVALID_REGISTER;
        return status;
    }

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_spi: Not a valid length!\n");
        status = CC1120_ERROR_CODE_WRITE_SPI_INVALID_LENGTH;
        return status;
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t header = (len > 1) ? (BURST_BIT | addr) : addr;

        mcu_cc1120_cs_assert();
        if (cc1120_send_byte_receive_status(header) != CC1120_ERROR_CODE_SUCCESS) {
            status = CC1120_ERROR_CODE_WRITE_SPI_INVALID_STATUS_BYTE_HEADER;
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t i;
        for(i = 0; i < len; i++) {
            if (cc1120_send_byte_receive_status(data[i]) != CC1120_ERROR_CODE_SUCCESS) {
                status = CC1120_ERROR_CODE_WRITE_SPI_INVALID_STATUS_BYTE_DATA;
                return status;
            }
        }
    }

    mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Writes to consecutive extended address space registers on the CC1120.
 * 
 * @param addr - The address of the first register to write to.
 * @param data - The array of data to write to the registers, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to write.
 * @return CC1120_ERROR_CODE_SUCCESS - If the write was successful.
 * @return An error code - If the register is not valid, or the status byte is invalid.
 */
cc1120_error_code cc1120_write_ext_addr_spi(uint8_t addr, uint8_t data[], uint8_t len) {
    cc1120_error_code status = CC1120_ERROR_CODE_SUCCESS;

    if ((addr > CC1120_REGS_EXT_PA_CFG3 && addr < CC1120_REGS_EXT_WOR_TIME1) ||
        (addr > CC1120_REGS_EXT_XOSC_TEST0 && addr < CC1120_REGS_EXT_RXFIRST) ||
        (addr > CC1120_REGS_EXT_FIFO_NUM_RXBYTES)) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_ext_addr_spi: Not a valid register!\n");
        status = CC1120_ERROR_CODE_WRITE_EXT_ADDR_SPI_INVALID_REGISTER;
        return status;
    }

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_ext_addr_spi: Not a valid length!\n");
        status = CC1120_ERROR_CODE_WRITE_EXT_ADDR_SPI_INVALID_LENGTH;
        return status;
    }
    
    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t header = (len > 1) ? (BURST_BIT | CC1120_REGS_EXT_ADDR) : CC1120_REGS_EXT_ADDR;

        mcu_cc1120_cs_assert();
        if (cc1120_send_byte_receive_status(header) != CC1120_ERROR_CODE_SUCCESS) {
            status = CC1120_ERROR_CODE_WRITE_EXT_ADDR_SPI_INVALID_STATUS_BYTE_HEADER;
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        if (mcu_cc1120_spi_transfer(addr) != 0x00) { // When sending the extended address, SO will return all zeros. See section 3.2.
            mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_ext_addr_spi: CC1120 write_ext_addr_spi failed\n");
            status = CC1120_ERROR_CODE_WRITE_EXT_ADDR_SPI_FAILED;
            return status;
        }
    }
    
    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t i;
        for(i = 0; i < len; i++) {
            if (cc1120_send_byte_receive_status(data[i]) != CC1120_ERROR_CODE_SUCCESS) {
                status = CC1120_ERROR_CODE_WRITE_EXT_ADDR_SPI_INVALID_STATUS_BYTE_DATA;
                return status;
            }
        }
    }
    
    mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief Calls a strobe command on the CC1120.
 * 
 * @param addr - The address of the strobe command.
 * @return CC1120_ERROR_CODE_SUCCESS - If the strobe command was successful.
 * @return An error code - If the register is not valid, or the status byte is invalid.
 */
cc1120_error_code cc1120_strobe_spi(uint8_t addr) {
    cc1120_error_code status = CC1120_ERROR_CODE_SUCCESS;

    if (addr < CC1120_STROBE_SRES || addr > CC1120_STROBE_SNOP) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_strobe_spi: Not a strobe register!\n");
        status = CC1120_ERROR_CODE_STROBE_SPI_INVALID_REGISTER;
        return status;
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        mcu_cc1120_cs_assert();    
        if (cc1120_send_byte_receive_status(addr) != CC1120_ERROR_CODE_SUCCESS) {
            status = CC1120_ERROR_CODE_STROBE_SPI_INVALID_STATUS_BYTE;
            return status;
        }
    }

    mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Reads consecutive registers from the FIFO memory.
 * 
 * @param data - The array to store the read data, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to read.
 * @return CC1120_ERROR_CODE_SUCCESS - If the read was successful.
 * @return An error code - If the status byte is invalid.
 */
cc1120_error_code cc1120_read_fifo(uint8_t data[], uint8_t len) {
    cc1120_error_code status = CC1120_ERROR_CODE_SUCCESS;

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_read_fifo: Not a valid length!\n");
        status = CC1120_ERROR_CODE_READ_FIFO_INVALID_LENGTH;
        return status;
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t header = (len > 1) ? (R_BIT | BURST_BIT | CC1120_REGS_FIFO_ACCESS_STD) :
                                    (R_BIT | CC1120_REGS_FIFO_ACCESS_STD);

        mcu_cc1120_cs_assert();
        if (cc1120_send_byte_receive_status(header) != CC1120_ERROR_CODE_SUCCESS) {
            status = CC1120_ERROR_CODE_READ_FIFO_INVALID_STATUS_BYTE;
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t i;
        for(i = 0; i < len; i++) {
            data[i] = mcu_cc1120_spi_transfer(0x00);
        }
    }
    mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Writes consecutive registers to the FIFO memory.
 * 
 * @param data - The array of data to write to the registers, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to write.
 * @return CC1120_ERROR_CODE_SUCCESS - If the write was successful.
 * @return An error code - If the status byte is invalid.
 */
cc1120_error_code cc1120_write_fifo(uint8_t data[], uint8_t len) {
    cc1120_error_code status = CC1120_ERROR_CODE_SUCCESS;

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_fifo: Not a valid length!\n");
        status = CC1120_ERROR_CODE_WRITE_FIFO_INVALID_LENGTH;
        return status;
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t header = (len > 1) ? (BURST_BIT | CC1120_REGS_FIFO_ACCESS_STD) :
                                    CC1120_REGS_FIFO_ACCESS_STD;

        mcu_cc1120_cs_assert();
        if (cc1120_send_byte_receive_status(header) != CC1120_ERROR_CODE_SUCCESS) {
            status = CC1120_ERROR_CODE_WRITE_FIFO_INVALID_STATUS_BYTE;
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t i;
        for(i = 0; i < len; i++) {
            mcu_cc1120_spi_transfer(data[i]);
        }
    }

    mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Reads consecutive registers directly from the FIFO on the CC1120.
 * 
 * @param addr - The address of the first register to read. Range 0x00 - 0xFF.
 * @param data - The array to store the read data, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to read.
 * @return CC1120_ERROR_CODE_SUCCESS - If the read was successful.
 * @return An error code - If the register is not valid, or the status byte is invalid.
 */
cc1120_error_code cc1120_read_fifo_direct(uint8_t addr, uint8_t data[], uint8_t len) {
    cc1120_error_code status = CC1120_ERROR_CODE_SUCCESS;

    if (addr < CC1120_FIFO_TX_START || addr > CC1120_FIFO_RX_END) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_read_fifo_direct: Not a valid FIFO register!\n");
        status = CC1120_ERROR_CODE_READ_FIFO_DIRECT_INVALID_REGISTER;
        return status;
    }

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_read_fifo_direct: Not a valid length!\n");
        status = CC1120_ERROR_CODE_READ_FIFO_DIRECT_INVALID_LENGTH;
        return status;
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t header = (len > 1) ? (R_BIT | BURST_BIT | CC1120_REGS_FIFO_ACCESS_DIR) :
                                    (R_BIT | CC1120_REGS_FIFO_ACCESS_DIR);

        mcu_cc1120_cs_assert();
        if ((cc1120_send_byte_receive_status(header) != CC1120_ERROR_CODE_SUCCESS)) {
            status = CC1120_ERROR_CODE_READ_FIFO_DIRECT_INVALID_STATUS_BYTE;
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        mcu_cc1120_spi_transfer(addr);
        uint8_t i;
        for(i = 0; i < len; i++) {
            data[i] = mcu_cc1120_spi_transfer(0x00);
        }
    }

    mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Writes consecutive registers directly to the FIFO on the CC1120.
 * 
 * @param addr - The address of the first register to write to. Range 0x00 - 0xFF.
 * @param data - The array of data to write to the registers, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to write.
 * @return CC1120_ERROR_CODE_SUCCESS - If the write was successful.
 * @return An error code - If the register is not valid, or the status byte is invalid.
 */
cc1120_error_code cc1120_write_fifo_direct(uint8_t addr, uint8_t data[], uint8_t len) {
    cc1120_error_code status = CC1120_ERROR_CODE_SUCCESS;

    if (addr < CC1120_FIFO_TX_START || addr > CC1120_FIFO_RX_END) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_fifo_direct: Not a valid FIFO register!\n");
        status = CC1120_ERROR_CODE_WRITE_FIFO_DIRECT_INVALID_RGISTER;
        return status;
    }

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_fifo_direct: Not a valid length!\n");
        status = CC1120_ERROR_CODE_WRITE_FIFO_DIRECT_INVALID_LENGTH;
        return status;
    }
    
    if (status == CC1120_ERROR_CODE_SUCCESS) {
        uint8_t header = (len > 1) ? (BURST_BIT | CC1120_REGS_FIFO_ACCESS_DIR) :
                                    CC1120_REGS_FIFO_ACCESS_DIR;


        mcu_cc1120_cs_assert();
        if ((cc1120_send_byte_receive_status(header) != CC1120_ERROR_CODE_SUCCESS)) {
            status = CC1120_ERROR_CODE_WRITE_FIFO_DIRECT_INVALID_STATUS_BYTE;
            return status;
        }
    }

    if (status == CC1120_ERROR_CODE_SUCCESS) {
        mcu_cc1120_spi_transfer(addr);
        uint8_t i;
        for(i = 0; i < len; i++) {
            mcu_cc1120_spi_transfer(data[i]);
        }
    }
    mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Reads the status register on the CC1120 and consecutively sends a byte over SPI.
 * 
 * @param data - The data to send to the status register.
 * @return CC1120_ERROR_CODE_SUCCESS - If the status byte is valid.
 * @return CC1120_ERROR_CODE_SEND_BYTE_RECEIVE_STATUS_INVALID_STATUS_BYTE - If the status byte is invalid.
 */
cc1120_error_code cc1120_send_byte_receive_status(uint8_t data) {
    cc1120_error_code status = CC1120_ERROR_CODE_SEND_BYTE_RECEIVE_STATUS_INVALID_STATUS_BYTE;
    union cc_st ccstatus;

    uint8_t i;
    for (i = 1; i <= 5; i++) {
        ccstatus.data = mcu_cc1120_spi_transfer(data);
        if (ccstatus.ccst.chip_ready == 1) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_send_byte_receive_status: CC1120 chip not ready. Retrying... (%u/5)\n", i);
        } else {
            status = CC1120_ERROR_CODE_SUCCESS;
            break;
        }
    }

    return status;
}
