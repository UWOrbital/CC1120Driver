#include "cc1120_spi.h"
#include "cc1120_regs.h"
#include "cc1120_mcu.h"
#include <stdbool.h>

/**
 * @brief - Reads from consecutive registers from the CC1120.
 * 
 * @param addr - The address of the first register to read.
 * @param data - The array to store the read data, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to read.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_read_spi(uint8_t addr, uint8_t data[], uint8_t len) {
    bool status = true;
    
    if (addr >= CC1120_REGS_EXT_ADDR) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_read_spi: Not a valid register!\n");
        status = false;
    }

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_read_spi: Not a valid length!\n");
        status = false;
    }

    if (status) {
        uint8_t header = (len > 1) ? R_BIT | BURST_BIT | addr : R_BIT | addr;

        // mcu_cc1120_cs_assert();
        if (!cc1120_send_byte_receive_status(header, true)) {
            status = false;
        }
    }

    if (status) {
        uint8_t i;
        for (i = 0; i < len - 1; i++) {
            data[i] = mcu_cc1120_spi_transfer(0x00, true);
        }

        data[len-1] = mcu_cc1120_spi_transfer(0x00, false);
    }

    // mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Reads from consecutive extended address space registers on the CC1120
 * 
 * @param addr - The address of the first register to read.
 * @param data - The array to store the read data, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to read.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_read_ext_addr_spi(uint8_t addr, uint8_t data[], uint8_t len) {
    bool status = true;

    if (addr > CC1120_REGS_EXT_PA_CFG3 && addr < CC1120_REGS_EXT_WOR_TIME1 ||
        addr > CC1120_REGS_EXT_XOSC_TEST0 && addr < CC1120_REGS_EXT_RXFIRST ||
        addr > CC1120_REGS_EXT_FIFO_NUM_RXBYTES) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_read_ext_addr_spi: Not a valid register!\n");
        status = false;
    }

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_read_ext_addr_spi: Not a valid length!\n");
        status = false;
    }
    
    if (status) {
        uint8_t header = (len > 1) ? R_BIT | BURST_BIT | CC1120_REGS_EXT_ADDR :
                                     R_BIT | CC1120_REGS_EXT_ADDR;

        // mcu_cc1120_cs_assert();
        if (!cc1120_send_byte_receive_status(header, true)) {
            status = false;
        }
    }

    if (status) {
        mcu_cc1120_spi_transfer(addr, true); // check if all zeroes are returned here from sending the extended address
        uint8_t i;
        for (i = 0; i < len - 1; i++) {
            data[i] = mcu_cc1120_spi_transfer(0x00, true);
        }

        data[len-1] = mcu_cc1120_spi_transfer(0x00, false);
    }

    // mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Writes to consecutive registers on the CC1120.
 * 
 * @param addr - The address of the first register to write to.
 * @param data - The array of data to write to the registers, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to write.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_write_spi(uint8_t addr, uint8_t data[], uint8_t len) {
    bool status = true;

    if(addr >= CC1120_REGS_EXT_ADDR) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_spi: Not a valid register!\n");
        status = false;
    }

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_spi: Not a valid length!\n");
        status = false;
    }

    if (status) {
        uint8_t header = (len > 1) ? BURST_BIT | addr : addr;

        // mcu_cc1120_cs_assert();
        if (!cc1120_send_byte_receive_status(header, true)) {
            status = false;
        }
    }

    if (status) {
        uint8_t i;
        for(i = 0; i < len - 1; i++) {
            if (!cc1120_send_byte_receive_status(data[i], true)) {
                status = false;
            }
        }

         if (!cc1120_send_byte_receive_status(data[len-1], false)) 
                status = false;
    }

    // mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Writes to consecutive extended address space registers on the CC1120.
 * 
 * @param addr - The address of the first register to write to.
 * @param data - The array of data to write to the registers, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to write.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_write_ext_addr_spi(uint8_t addr, uint8_t data[], uint8_t len) {
    bool status = true;

    if (addr > CC1120_REGS_EXT_PA_CFG3 && addr < CC1120_REGS_EXT_WOR_TIME1 ||
        addr > CC1120_REGS_EXT_XOSC_TEST0 && addr < CC1120_REGS_EXT_RXFIRST ||
        addr > CC1120_REGS_EXT_FIFO_NUM_RXBYTES) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_ext_addr_spi: Not a valid register!\n");
        status = false;
    }

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_ext_addr_spi: Not a valid length!\n");
        status = false;
    }
    
    if (status) {
        uint8_t header = (len > 1) ? BURST_BIT | CC1120_REGS_EXT_ADDR : CC1120_REGS_EXT_ADDR;

        // mcu_cc1120_cs_assert();
        if (!cc1120_send_byte_receive_status(header, true)) {
            status = false;
        }
    }

    if (status) {
        if (mcu_cc1120_spi_transfer(addr, true) != 0x00) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_ext_addr_spi: CC1120 write ext addr failed\n");
            status = false;
        }
    }
    
    if (status) {
        uint8_t i;
        for(i = 0; i < len - 1; i++) {
            if (!cc1120_send_byte_receive_status(data[i], true)) {
                status = false;
            }
        }

         if (!cc1120_send_byte_receive_status(data[len-1], false)) 
                status = false;
    }
    
    // mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief Calls a strobe command on the CC1120.
 * 
 * @param addr - The address of the strobe command.
 * @return true - If the strobe command was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_strobe_spi(uint8_t addr) {
    bool status = true;

    if (addr < CC1120_STROBE_SRES || addr > CC1120_STROBE_SNOP) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_strobe_spi: Not a strobe register!\n");
        status = false;
    }

    if (status) {
        // mcu_cc1120_cs_assert();    
        if (!cc1120_send_byte_receive_status(addr, false)) {
            status = false;
        }
    }

    // mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Reads consecutive registers from the FIFO memory.
 * 
 * @param data - The array to store the read data, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to read.
 * @return true - If the read was successful.
 * @return false - If the status byte is invalid.
 */
bool cc1120_read_fifo(uint8_t data[], uint8_t len) {
    bool status = true;

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_read_fifo: Not a valid length!\n");
        status = false;
    }

    if (status) {
        uint8_t header = (len > 1) ? R_BIT | BURST_BIT | CC1120_REGS_FIFO_ACCESS_STD :
                                    R_BIT | CC1120_REGS_FIFO_ACCESS_STD;

        // mcu_cc1120_cs_assert();
        if (!cc1120_send_byte_receive_status(header, true)) {
            status = false;
        }
    }

    if (status) {
        uint8_t i;
        for (i = 0; i < len - 1; i++) {
            data[i] = mcu_cc1120_spi_transfer(0x00, true);
        }

        data[len-1] = mcu_cc1120_spi_transfer(0x00, false);
    }
    
    // mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Writes consecutive registers to the FIFO memory.
 * 
 * @param data - The array of data to write to the registers, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to write.
 * @return true - If the write was successful.
 * @return false - If the status byte is invalid.
 */
bool cc1120_write_fifo(uint8_t data[], uint8_t len) {
    bool status = true;

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_fifo: Not a valid length!\n");
        status = false;
    }

    if (status) {
        uint8_t header = (len > 1) ? BURST_BIT | CC1120_REGS_FIFO_ACCESS_STD :
                                    CC1120_REGS_FIFO_ACCESS_STD;

        // mcu_cc1120_cs_assert();
        if (!cc1120_send_byte_receive_status(header, true)) {
            status = false;
        }
    }

    if (status) {
        uint8_t i;
        for(i = 0; i < len - 1; i++) {
            mcu_cc1120_spi_transfer(data[i], true);
        }

        mcu_cc1120_spi_transfer(data[len-1], false);
    }

    // mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Reads consecutive registers directly from the FIFO on the CC1120.
 * 
 * @param addr - The address of the first register to read. Range 0x00 - 0xFF.
 * @param data - The array to store the read data, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to read.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_read_fifo_direct(uint8_t addr, uint8_t data[], uint8_t len) {
    bool status = true;

    if (addr < CC1120_FIFO_TX_START || addr > CC1120_FIFO_RX_END) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_read_fifo_direct: Not a valid FIFO register!\n");
        status = false;
    }

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_read_fifo_direct: Not a valid length!\n");
        status = false;
    }

    if (status) {
        uint8_t header = (len > 1) ? R_BIT | BURST_BIT | CC1120_REGS_FIFO_ACCESS_DIR :
                                    R_BIT | CC1120_REGS_FIFO_ACCESS_DIR;

        // mcu_cc1120_cs_assert();
        if (!cc1120_send_byte_receive_status(header, true)) {
            status = false;
        }
    }

    if (status) {
        mcu_cc1120_spi_transfer(addr, true);
        uint8_t i;
        for(i = 0; i < len - 1; i++) {
            data[i] = mcu_cc1120_spi_transfer(0x00, true);
        }

        data[len-1] = mcu_cc1120_spi_transfer(0x00, false);
    }

    // mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Writes consecutive registers directly to the FIFO on the CC1120.
 * 
 * @param addr - The address of the first register to write to. Range 0x00 - 0xFF.
 * @param data - The array of data to write to the registers, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to write.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_write_fifo_direct(uint8_t addr, uint8_t data[], uint8_t len) {
    bool status = true;

    if (addr < CC1120_FIFO_TX_START || addr > CC1120_FIFO_RX_END) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_fifo_direct: Not a valid FIFO register!\n");
        status = false;
    }

    if (len < 1) {
        mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_write_fifo_direct: Not a valid length!\n");
        status = false;
    }
    
    if (status) {
        uint8_t header = (len > 1) ? BURST_BIT | CC1120_REGS_FIFO_ACCESS_DIR :
                                    CC1120_REGS_FIFO_ACCESS_DIR;


        // mcu_cc1120_cs_assert();
        if (!cc1120_send_byte_receive_status(header, true)) {
            status = false;
        }
    }

    if (status) {
        mcu_cc1120_spi_transfer(addr, true);
        uint8_t i;
        for(i = 0; i < len - 1; i++) {
            mcu_cc1120_spi_transfer(data[i], true);
        }

        mcu_cc1120_spi_transfer(data[len-1], false);
    }
    // mcu_cc1120_cs_deassert();
    return status;
}

/**
 * @brief - Reads the status register on the CC1120 and consecutively sends a byte over SPI.
 * 
 * @param data - The data to send to the status register.
 * @param csHold - Boolean that represents whether to hold CS low for consecutive sends and receives.
 * @return true - If the status byte is valid.
 * @return false - If the status byte is invalid.
 */
bool cc1120_send_byte_receive_status(uint8_t data, bool csHold) {
    bool status = false;
    union cc_st ccstatus;

    uint8_t i;
    for (i = 1; i <= 5; i++) {
        ccstatus.data = mcu_cc1120_spi_transfer(data, csHold);
        if (ccstatus.ccst.chip_ready == 1) {
            mcu_log(CC1120_LOG_LEVEL_ERROR, "cc1120_send_byte_receive_status: CC1120 chip not ready. Retrying... (%u/5)\n", i);
        } else {
            status = true;
            break;
        }
    }

    return status;
}