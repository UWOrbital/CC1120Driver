#include "cc1120_spi.h"
#include "cc1120_regs.h"
#include "cc1120_ext_regs.h"
#include "arduino_unit_tests.h"
#include "Arduino.h"
#include <stdint.h>
#include <SPI.h>

bool arduinoReadSPI(uint8_t addr, uint8_t *data) {
    union cc_st ccstatus;
    digitalWrite(CS, LOW);
    ccstatus.v = SPI.transfer(R_BIT | addr);
    if (ccstatus.ccst.chip_ready == 1) {
        Serial.print("CC1120 chip not ready");
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
        Serial.print("CC1120 chip not ready");
        return false;
    }
    SPI.transfer(addr);
    *data = SPI.transfer(0xff);
    digitalWrite(CS, HIGH);
    return true;
}

bool arduinoWriteSPI(uint8_t addr, uint8_t data) {
    union cc_st ccstatus;
    digitalWrite(CS, LOW);
    ccstatus.v = SPI.transfer(W_BIT | addr);
    if (ccstatus.ccst.chip_ready == 1) {
        Serial.print("CC1120 chip not ready");
        return false;
    }
    SPI.transfer(data);
    digitalWrite(CS, HIGH);
    return true;
}

bool arduinoWriteExtAddrSPI(uint8_t addr, uint8_t data) {
    union cc_st ccstatus;
    digitalWrite(CS, LOW);
    ccstatus.v = SPI.transfer(W_BIT | EXT_ADDR);
    if (ccstatus.ccst.chip_ready == 1) {
        Serial.print("CC1120 chip not ready");
        return false;
    }
    if (SPI.transfer(addr) != 0x00) {
        Serial.print("CC1120 write ext addr failed");
        return false;
    }
    ccstatus.v = SPI.transfer(data);
    if (ccstatus.ccst.chip_ready == 1) {
        Serial.print("CC1120 chip not ready");
        return false;
    }
    digitalWrite(CS, HIGH);
    return true;
}
