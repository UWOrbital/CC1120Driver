#include "cc1120_spi.h"
#include "cc1120_regs.h"
#include "cc1120_ext_regs.h"
#include "arduino_unit_tests.h"
#include "Arduino.h"
#include <stdint.h>
#include <SPI.h>

/**
 * @brief Reads a register from the CC1120.
 * 
 * @param addr - The address of the register to read.
 * @param data - Pointer to the variable to store the read data.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoReadSPI(uint8_t addr, uint8_t *data) {
    if(addr >= EXT_ADDR) {
        Serial.println("Not a valid register!");
        return false;
    }

    union cc_st ccstatus;
    digitalWrite(CS, LOW);
    ccstatus.v = SPI.transfer(R_BIT | addr);
    if (ccstatus.ccst.chip_ready == 1) {
        Serial.println("CC1120 chip not ready");
        return false;
    }
    *data = SPI.transfer(0x00);
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief Reads an extended address space register from the CC1120.
 * 
 * @param addr - The address of the register to read, starting at 0x00.
 * @param data - Pointer to the variable to store the read data.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoReadExtAddrSPI(uint8_t addr, uint8_t *data) {
    union cc_st ccstatus;
    digitalWrite(CS, LOW);
    ccstatus.v = SPI.transfer(R_BIT | EXT_ADDR);
    if (ccstatus.ccst.chip_ready == 1) {
        Serial.println("CC1120 chip not ready");
        return false;
    }
    SPI.transfer(addr);
    *data = SPI.transfer(0xff);
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief Writes to a register on the CC1120.
 * 
 * @param addr - The address of the register to write to.
 * @param data - The data to write to the register.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoWriteSPI(uint8_t addr, uint8_t data) {
    if(addr >= EXT_ADDR) {
        Serial.println("Not a valid register!");
        return false;
    }

    union cc_st ccstatus;
    digitalWrite(CS, LOW);
    ccstatus.v = SPI.transfer(addr);
    if (ccstatus.ccst.chip_ready == 1) {
        Serial.println("CC1120 chip not ready");
        return false;
    }
    SPI.transfer(data);
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief Writes to an extended address space register on the CC1120.
 * 
 * @param addr - The address of the register to write to, starting at 0x00.
 * @param data - The data to write to the register.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoWriteExtAddrSPI(uint8_t addr, uint8_t data) {
    union cc_st ccstatus;
    digitalWrite(CS, LOW);
    ccstatus.v = SPI.transfer(EXT_ADDR);
    if (ccstatus.ccst.chip_ready == 1) {
        Serial.println("CC1120 chip not ready");
        return false;
    }
    if (SPI.transfer(addr) != 0x00) {
        Serial.println("CC1120 write ext addr failed");
        return false;
    }
    ccstatus.v = SPI.transfer(data);
    if (ccstatus.ccst.chip_ready == 1) {
        Serial.println("CC1120 chip not ready");
        return false;
    }
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief Calls a strobe command on the CC1120.
 * 
 * @param addr - The address of the strobe command.
 * @return true - If the strobe command was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoStrobeSPI(uint8_t addr) {
    if (!(addr >= 0x30 && addr <= 0x3f)) {
        Serial.println("Not a strobe register!");
        return false;
    }

    union cc_st ccstatus;
    digitalWrite(CS, LOW);
    ccstatus.v = SPI.transfer(addr);
    if (ccstatus.ccst.chip_ready == 1) {
        Serial.println("CC1120 chip not ready");
        return false;
    }
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief Reads directly from the FIFO on the CC1120.
 * 
 * @param addr - The address of the register to read. Range 0x00 - 0xFF.
 * @param data - Pointer to the variable to store the read data.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoReadFIFODirect(uint8_t addr, uint8_t *data) {
    if (addr <= FIFO_TX_START || addr >= FIFO_TX_END) {
        Serial.println("Not a valid FIFO register!");
        return false;
    }

    union cc_st ccstatus;
    digitalWrite(CS, LOW);
    ccstatus.v = SPI.transfer(DIR_FIFO_ACCESS);
    if (ccstatus.ccst.chip_ready == 1) {
        Serial.println("CC1120 chip not ready");
        return false;
    }
    SPI.transfer(addr);
    *data = SPI.transfer(0xff);
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief Writes directly to the FIFO on the CC1120.
 * 
 * @param addr - The address of the register to write to. Range 0x00 - 0xFF.
 * @param data - The data to write to the register.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoWriteFIFODirect(uint8_t addr, uint8_t data) {
    if (addr <= FIFO_TX_START || addr >= FIFO_RX_END) {
        Serial.println("Not a valid FIFO register!");
        return false;
    }

    union cc_st ccstatus;
    digitalWrite(CS, LOW);
    ccstatus.v = SPI.transfer(DIR_FIFO_ACCESS);
    if (ccstatus.ccst.chip_ready == 1) {
        Serial.println("CC1120 chip not ready");
        return false;
    }
    SPI.transfer(addr);
    SPI.transfer(data);
    digitalWrite(CS, HIGH);
    return true;
}
