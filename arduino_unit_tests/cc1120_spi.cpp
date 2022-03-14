#include "cc1120_spi.h"
#include "cc1120_regs.h"
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
    if(addr >= CC1120_REGS_EXT_ADDR) {
        Serial.println("Not a valid register!");
        return false;
    }

    union cc_st ccstatus;
    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(R_BIT | addr)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    *data = SPI.transfer(0x00);
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief - Reads consecutive registers from the CC1120 in burst access mode.
 * 
 * @param addr - The address of the first register to read.
 * @param data - The array to store the read data.
 * @param len - The number of registers to read.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoBurstReadSPI(uint8_t addr, uint8_t data[], uint8_t len) {
    if(addr >= CC1120_REGS_EXT_ADDR) {
        Serial.println("Not a valid register!");
        return false;
    }

    union cc_st ccstatus;
    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(R_BIT | BURST_BIT | addr)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    for(uint8_t i = 0; i < len; i++) {
        data[i] = SPI.transfer(0x00);
    }
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
    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(R_BIT | CC1120_REGS_EXT_ADDR)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    SPI.transfer(addr);
    *data = SPI.transfer(0xFF);
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief - Reads consecutive extended address space registers from the CC1120 in burst mode.
 * 
 * @param addr - The address of the first register to read.
 * @param data - The array to store the read data.
 * @param len - The number of registers to read.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoBurstReadExtAddrSPI(uint8_t addr, uint8_t data[], uint8_t len) {
    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(R_BIT | BURST_BIT | CC1120_REGS_EXT_ADDR)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    SPI.transfer(addr);
    for(uint8_t i = 0; i < len; i++) {
        data[i] = SPI.transfer(0xFF);
    }
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
    if(addr >= CC1120_REGS_EXT_ADDR) {
        Serial.println("Not a valid register!");
        return false;
    }

    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(addr)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    if (!sendByteReceiveStatus(data)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief - Writes consecutive registers on the CC1120 in burst mode.
 * 
 * @param addr - The address of the first register to write to.
 * @param data - The array of data to write to the registers.
 * @param len - The number of registers to write.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoBurstWriteSPI(uint8_t addr, uint8_t data[], uint8_t len) {
    if(addr >= CC1120_REGS_EXT_ADDR) {
        Serial.println("Not a valid register!");
        return false;
    }

    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(BURST_BIT | addr)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    for(uint8_t i = 0; i < len; i++) {
        if (!sendByteReceiveStatus(data[i])) {
            digitalWrite(CS, HIGH);
            return false;
        }
    }
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
    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(CC1120_REGS_EXT_ADDR)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    if (SPI.transfer(addr) != 0x00) {
        Serial.println("CC1120 write ext addr failed");
        digitalWrite(CS, HIGH);
        return false;
    }
    if (!sendByteReceiveStatus(data)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief - Writes consecutive extended address space registers on the CC1120 in burst mode.
 * 
 * @param addr - The address of the first register to write to.
 * @param data - The array of data to write to the registers.
 * @param len - The number of registers to write.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoBurstWriteExtAddrSPI(uint8_t addr, uint8_t data[], uint8_t len) {
    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(CC1120_REGS_EXT_ADDR)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    if (SPI.transfer(addr) != 0x00) {
        Serial.println("CC1120 write ext addr failed");
        digitalWrite(CS, HIGH);
        return false;
    }
    for(uint8_t i = 0; i < len; i++) {
        if (!sendByteReceiveStatus(data[i])) {
            digitalWrite(CS, HIGH);
            return false;
        }
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

    digitalWrite(CS, LOW);    
    if (!sendByteReceiveStatus(addr)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief Reads from FIFO RX queue on the CC1120.
 * 
 * @param data - Pointer to the variable to store the read data.
 * @return true - If the read was successful.
 * @return false - If the status byte is invalid.
 */
bool arduinoReadFIFO(uint8_t *data) {
    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(R_BIT | CC1120_REGS_FIFO_ACCESS_STD)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    *data = SPI.transfer(0xFF);
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief - Reads consecutive registers from the FIFO memory in burst mode.
 * 
 * @param data - The array to store the read data.
 * @param len - The number of registers to read.
 * @return true - If the read was successful.
 * @return false - If the status byte is invalid.
 */
bool arduinoBurstReadFIFO(uint8_t data[], uint8_t len) {
    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(R_BIT | BURST_BIT | CC1120_REGS_FIFO_ACCESS_STD)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    for(uint8_t i = 0; i < len; i++) {
        data[i] = SPI.transfer(0xFF);
    }
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief Writes to FIFO TX queue on the CC1120.
 * 
 * @param data - The data to write to the FIFO.
 * @return true - If the write was successful.
 * @return false - If the status byte is invalid.
 */
bool arduinoWriteFIFO(uint8_t data) {
    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(CC1120_REGS_FIFO_ACCESS_STD)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    SPI.transfer(data);
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief - Writes consecutive registers to the FIFO memory in burst mode.
 * 
 * @param data - The array of data to write to the FIFO.
 * @param len - The number of registers to write.
 * @return true - If the write was successful.
 * @return false - If the status byte is invalid.
 */
bool arduinoBurstWriteFIFO(uint8_t data[], uint8_t len) {
    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(BURST_BIT | CC1120_REGS_FIFO_ACCESS_STD)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    for(uint8_t i = 0; i < len; i++) {
        SPI.transfer(data[i]);
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
    if (addr < CC1120_REGS_FIFO_TX_START || addr > CC1120_REGS_FIFO_RX_END) {
        Serial.println("Not a valid FIFO register!");
        return false;
    }

    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(CC1120_REGS_FIFO_ACCESS_DIR)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    SPI.transfer(addr);
    *data = SPI.transfer(0xff);
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief - Reads consecutive registers directly from the FIFO on the CC1120 in burst mode.
 * 
 * @param addr - The address of the first register to read. Range 0x00 - 0xFF.
 * @param data - The array to store the read data.
 * @param len - The number of registers to read.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoBurstReadFIFODirect(uint8_t addr, uint8_t data[], uint8_t len) {
    if (addr < CC1120_REGS_FIFO_TX_START || addr > CC1120_REGS_FIFO_RX_END) {
        Serial.println("Not a valid FIFO register!");
        return false;
    }

    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(CC1120_REGS_FIFO_ACCESS_DIR)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    SPI.transfer(addr);
    for(uint8_t i = 0; i < len; i++) {
        data[i] = SPI.transfer(0xff);
    }
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
    if (addr < CC1120_REGS_FIFO_TX_START || addr > CC1120_REGS_FIFO_RX_END) {
        Serial.println("Not a valid FIFO register!");
        return false;
    }

    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(CC1120_REGS_FIFO_ACCESS_DIR)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    SPI.transfer(addr);
    SPI.transfer(data);
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief - Writes consecutive registers directly to the FIFO on the CC1120 in burst mode.
 * 
 * @param addr - The address of the first register to write to. Range 0x00 - 0xFF.
 * @param data - The array of data to write to the registers.
 * @param len - The number of registers to write.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoBurstWriteFIFODirect(uint8_t addr, uint8_t data[], uint8_t len) {
    if (addr < CC1120_REGS_FIFO_TX_START || addr > CC1120_REGS_FIFO_RX_END) {
        Serial.println("Not a valid FIFO register!");
        return false;
    }

    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(CC1120_REGS_FIFO_ACCESS_DIR)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    SPI.transfer(addr);
    for(uint8_t i = 0; i < len; i++) {
        SPI.transfer(data[i]);
    }
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief - Reads the status register on the CC1120 and consecutively sends a byte over SPI.
 * 
 * @param data - The data to send to the status register.
 * @return true - If the status byte is valid.
 * @return false - If the status byte is invalid.
 */
bool sendByteReceiveStatus(uint8_t data) {
    union cc_st ccstatus;

    uint8_t i=1;
    for (; i <= 5; i++) {
        ccstatus.data = SPI.transfer(data);
        if (ccstatus.ccst.chip_ready == 1) {
            Serial.print("CC1120 chip not ready. Retrying... (");
            Serial.print(i);
            Serial.println("/5)");
        } else {
            return true;
        }
    }

    return false;
}
