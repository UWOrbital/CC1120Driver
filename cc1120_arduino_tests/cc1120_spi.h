#ifndef SPI_CC1120_H
#define SPI_CC1120_H

#include <stdint.h>
#include <stdbool.h>

#define R_BIT 1 << 7
#define BURST_BIT 1 << 6

struct cc_status {
  uint8_t res : 4;
  uint8_t state : 3;
  uint8_t chip_ready : 1;
};

union cc_st {
  struct cc_status ccst;
  uint8_t data;
};

/**
 * @brief Reads a register from the CC1120.
 * 
 * @param addr - The address of the register to read.
 * @param data - Pointer to the variable to store the read data.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_read_spi(uint8_t addr, uint8_t *data);

/**
 * @brief - Reads consecutive registers from the CC1120 in burst access mode.
 * 
 * @param addr - The address of the first register to read.
 * @param data - The array to store the read data.
 * @param len - The number of registers to read.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_burst_read_spi(uint8_t addr, uint8_t data[], uint8_t len);

/**
 * @brief Reads an extended address space register from the CC1120.
 * 
 * @param addr - The address of the register to read, starting at 0x00.
 * @param data - Pointer to the variable to store the read data.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_read_ext_addr_spi(uint8_t addr, uint8_t *data);

/**
 * @brief - Reads consecutive extended address space registers from the CC1120 in burst mode.
 * 
 * @param addr - The address of the first register to read.
 * @param data - The array to store the read data.
 * @param len - The number of registers to read.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_burst_read_ext_addr_spi(uint8_t addr, uint8_t data[], uint8_t len);

/**
 * @brief Writes to a register on the CC1120.
 * 
 * @param addr - The address of the register to write to.
 * @param data - The data to write to the register.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_write_spi(uint8_t addr, uint8_t data);

/**
 * @brief - Writes consecutive registers on the CC1120 in burst mode.
 * 
 * @param addr - The address of the first register to write to.
 * @param data - The array of data to write to the registers.
 * @param len - The number of registers to write.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_burst_write_spi(uint8_t addr, uint8_t data[], uint8_t len);

/**
 * @brief Writes to an extended address space register on the CC1120.
 * 
 * @param addr - The address of the register to write to, starting at 0x00.
 * @param data - The data to write to the register.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_write_ext_addr_spi(uint8_t addr, uint8_t data);

/**
 * @brief - Writes consecutive extended address space registers on the CC1120 in burst mode.
 * 
 * @param addr - The address of the first register to write to.
 * @param data - The array of data to write to the registers.
 * @param len - The number of registers to write.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_burst_write_ext_addr_spi(uint8_t addr, uint8_t data[], uint8_t len);

/**
 * @brief Calls a strobe command on the CC1120.
 * 
 * @param addr - The address of the strobe command.
 * @return true - If the strobe command was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_strobe_spi(uint8_t addr);

/**
 * @brief Reads from FIFO RX queue on the CC1120.
 * 
 * @param data - Pointer to the variable to store the read data.
 * @return true - If the read was successful.
 * @return false - If the status byte is invalid.
 */
bool cc1120_read_fifo(uint8_t *data);

/**
 * @brief - Reads consecutive registers from the FIFO memory in burst mode.
 * 
 * @param data - The array to store the read data.
 * @param len - The number of registers to read.
 * @return true - If the read was successful.
 * @return false - If the status byte is invalid.
 */
bool cc1120_burst_read_fifo(uint8_t data[], uint8_t len);

/**
 * @brief Writes to FIFO TX queue on the CC1120.
 * 
 * @param data - The data to write to the FIFO.
 * @return true - If the write was successful.
 * @return false - If the status byte is invalid.
 */
bool cc1120_write_fifo(uint8_t data);

/**
 * @brief - Writes consecutive registers to the FIFO memory in burst mode.
 * 
 * @param data - The array of data to write to the FIFO.
 * @param len - The number of registers to write.
 * @return true - If the write was successful.
 * @return false - If the status byte is invalid.
 */
bool cc1120_burst_write_fifo(uint8_t data[], uint8_t len);

/**
 * @brief Reads directly from the FIFO on the CC1120.
 * 
 * @param addr - The address of the register to read. Range 0x00 - 0xFF.
 * @param data - Pointer to the variable to store the read data.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_read_fifo_direct(uint8_t addr, uint8_t *data);

/**
 * @brief - Reads consecutive registers directly from the FIFO on the CC1120 in burst mode.
 * 
 * @param addr - The address of the first register to read. Range 0x00 - 0xFF.
 * @param data - The array to store the read data.
 * @param len - The number of registers to read.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_burst_read_fifo_direct(uint8_t addr, uint8_t data[], uint8_t len);

/**
 * @brief Writes directly to the FIFO on the CC1120.
 * 
 * @param addr - The address of the register to write to. Range 0x00 - 0xFF.
 * @param data - The data to write to the register.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_write_fifo_direct(uint8_t addr, uint8_t data);

/**
 * @brief - Writes consecutive registers directly to the FIFO on the CC1120 in burst mode.
 * 
 * @param addr - The address of the first register to write to. Range 0x00 - 0xFF.
 * @param data - The array of data to write to the registers.
 * @param len - The number of registers to write.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool cc1120_burst_write_fifo_direct(uint8_t addr, uint8_t data[], uint8_t len);

/**
 * @brief - Reads the status register on the CC1120 and consecutively sends a byte over SPI.
 * 
 * @param data - The data to send to the status register.
 * @return true - If the status byte is valid.
 * @return false - If the status byte is invalid.
 */
bool sendByteReceiveStatus(uint8_t data);

#endif /* SPI_CC1120_H */
