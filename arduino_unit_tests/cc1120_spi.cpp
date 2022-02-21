#include "cc1120_spi.h"
#include "cc1120_regs.h"
#include "cc1120_ext_regs.h"
#include "arduino_unit_tests.h"
#include "Arduino.h"
#include <stdint.h>
#include <SPI.h>

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

bool arduinoReadFIFO(uint8_t addr, uint8_t *data) {
    if (addr <= FIFO_READ_START || addr >= FIFO_READ_END) {
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

bool arduinoWriteFIFO(uint8_t addr, uint8_t data) {
    if (addr <= FIFO_WRITE_START || addr >= FIFO_WRITE_END) {
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
