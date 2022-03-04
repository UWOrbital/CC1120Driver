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
    if (!sendByteReceiveStatus(R_BIT | addr)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    *data = SPI.transfer(0x00);
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief Reads registers from the CC1120 in burst access mode.
 * 
 * @param addr - The address of the first register to read.
 * @param dataPtrs - Array of pointers to the variables to store the read data.
 * @param length - Size of the pointer array.
 * @return true - If the read was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoReadBurstSPI(uint8_t addr, uint8_t dataPtrs[], size_t length) {

    if(addr >= EXT_ADDR) {
        Serial.println("Not a valid register!");
        return false;
    }

    union cc_st ccstatus;
    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(R_BIT | BURST_BIT | addr)) {
        digitalWrite(CS, HIGH);
        return false;
    }


    for (int i = 0; i < length; ++i) {
        *(dataPtr[i]) = SPI.transfer(0x00);
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
    if (!sendByteReceiveStatus(R_BIT | EXT_ADDR)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    SPI.transfer(addr);
    *data = SPI.transfer(0xFF);
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief Reads an extended address space register on the CC1120 in burst access mode.
 * 
 * @param addr - The address of the register to read, starting at 0x00.
 * @param dataPtrs - Array of pointers to the variables to store the read data.
 * @param length - Size of the pointer array.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoReadBurstExtAddrSPI(uint8_t addr, uint8_t dataPtrs[], size_t length) {
    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(BURST_BIT | R_BIT | EXT_ADDR)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    SPI.transfer(addr);
    for (int i = 0; i < length; ++i) {
        *(dataPtr[i]) = SPI.transfer(0xFF);
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
    if(addr >= EXT_ADDR) {
        Serial.println("Not a valid register!");
        return false;
    }

    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(addr)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    SPI.transfer(data);
    digitalWrite(CS, HIGH);
    return true;
}

/**
 * @brief Writes to registers on the CC1120 in burst access mode.
 * 
 * @param addr - The address of the first register to write to.
 * @param dataPtrs - Array of pointers to the variables storing the write data.
 * @param length - Size of the pointer array.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoWriteBurstSPI(uint8_t addr, uint8_t dataPtrs[], size_t length) {
    if(addr >= EXT_ADDR) {
        Serial.println("Not a valid register!");
        return false;
    }

    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(BURST_BIT | addr)) {
        digitalWrite(CS, HIGH);
        return false;
    }

    for (int i = 0; i < length; ++i) {
        SPI.transfer(dataPtrs[i]);
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
    if (!sendByteReceiveStatus(EXT_ADDR)) {
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
 * @brief Writes to and extended address space register on the CC1120 in burst access mode.
 * 
 * @param addr - The address of the register to write to, starting at 0x00.
 * @param dataPtrs - Array of pointers to the variables storing the write data.
 * @param length - Size of the pointer array.
 * @return true - If the write was successful.
 * @return false - If the register is not valid, or the status byte is invalid.
 */
bool arduinoWriteBurstExtAddrSPI(uint8_t addr, uint8_t dataPtrs[], size_t length) {
    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(BURST_BIT | EXT_ADDR)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    if (SPI.transfer(addr) != 0x00) {
        Serial.println("CC1120 write ext addr failed");
        digitalWrite(CS, HIGH);
        return false;
    }

    for (int i = 0; i < length; ++i) {
        SPI.transfer(dataPtrs[i]);
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
    if (!sendByteReceiveStatus(R_BIT | REG_FIFO_ACCESS)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    *data = SPI.transfer(0xff);
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
    if (!sendByteReceiveStatus(REG_FIFO_ACCESS)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    SPI.transfer(data);
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

    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(DIR_FIFO_ACCESS)) {
        digitalWrite(CS, HIGH);
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

    digitalWrite(CS, LOW);
    if (!sendByteReceiveStatus(DIR_FIFO_ACCESS)) {
        digitalWrite(CS, HIGH);
        return false;
    }
    SPI.transfer(addr);
    SPI.transfer(data);
    digitalWrite(CS, HIGH);
    return true;
}

bool sendByteReceiveStatus(uint8_t data) {
    union cc_st ccstatus;

    uint8_t i=1;
    for (; i <= 5; i++) {
        ccstatus.v = SPI.transfer(data);
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
