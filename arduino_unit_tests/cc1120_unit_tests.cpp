#include "cc1120_unit_tests.h"
#include "cc1120_spi.h"
#include "cc1120_regs.h"
#include "cc1120_ext_regs.h"
#include "Arduino.h"
#include <stdint.h>

static uint8_t REGS_DEFAULTS[EXT_ADDR];
union cc_st ccstatus;

/**
 * @brief Set the default values for CC1120 registers.
 * 
 */
void setRegisterDefaults() {
    REGS_DEFAULTS[IOCFG3] = 0x06;
    REGS_DEFAULTS[IOCFG2] = 0x07;
    REGS_DEFAULTS[IOCFG1] = 0x30;
    REGS_DEFAULTS[IOCFG0] = 0x3C;
    REGS_DEFAULTS[SYNC3] = 0x93;
    REGS_DEFAULTS[SYNC2] = 0x0B;
    REGS_DEFAULTS[SYNC1] = 0x51;
    REGS_DEFAULTS[SYNC0] = 0xDE;
    REGS_DEFAULTS[SYNC_CFG1] = 0x0A;
    REGS_DEFAULTS[SYNC_CFG0] = 0x17;
    REGS_DEFAULTS[DEVIATION_M] = 0x06;
    REGS_DEFAULTS[MODCFG_DEV_E] = 0x03;
    REGS_DEFAULTS[DCFILT_CFG] = 0x4C;
    REGS_DEFAULTS[PREAMBLE_CFG1] = 0x14;
    REGS_DEFAULTS[PREAMBLE_CFG0] = 0x2A;
    REGS_DEFAULTS[FREQ_IF_CFG] = 0x40;
    REGS_DEFAULTS[IQIC] = 0xC4;
    REGS_DEFAULTS[CHAN_BW] = 0x14;
    REGS_DEFAULTS[MDMCFG1] = 0x46;
    REGS_DEFAULTS[MDMCFG0] = 0x0D;
    REGS_DEFAULTS[SYMBOL_RATE2] = 0x43;
    REGS_DEFAULTS[SYMBOL_RATE1] = 0xA9;
    REGS_DEFAULTS[SYMBOL_RATE0] = 0x2A;
    REGS_DEFAULTS[AGC_REF] = 0x36;
    REGS_DEFAULTS[AGC_CS_THR] = 0x00;
    REGS_DEFAULTS[AGC_GAIN_ADJUST] = 0x00;
    REGS_DEFAULTS[AGC_CFG3] = 0x91;
    REGS_DEFAULTS[AGC_CFG2] = 0x20;
    REGS_DEFAULTS[AGC_CFG1] = 0xAA;
    REGS_DEFAULTS[AGC_CFG0] = 0xC3;
    REGS_DEFAULTS[FIFO_CFG] = 0x80;
    REGS_DEFAULTS[DEV_ADDR] = 0x00;
    REGS_DEFAULTS[SETTLING_CFG] = 0x0B;
    REGS_DEFAULTS[FS_CFG] = 0x02;
    REGS_DEFAULTS[WOR_CFG1] = 0x08;
    REGS_DEFAULTS[WOR_CFG0] = 0x21;
    REGS_DEFAULTS[WOR_EVENT0_MSB] = 0x00;
    REGS_DEFAULTS[WOR_EVENT0_LSB] = 0x00;
    REGS_DEFAULTS[PKT_CFG2] = 0x04;
    REGS_DEFAULTS[PKT_CFG1] = 0x05;
    REGS_DEFAULTS[PKT_CFG0] = 0x00;
    REGS_DEFAULTS[RFEND_CFG1] = 0x0F;
    REGS_DEFAULTS[RFEND_CFG0] = 0x00;
    REGS_DEFAULTS[PA_CFG2] = 0x7F;
    REGS_DEFAULTS[PA_CFG1] = 0x56;
    REGS_DEFAULTS[PA_CFG0] = 0x7C;
    REGS_DEFAULTS[PKT_LEN] = 0x03;
}

/**
 * @brief Unit test for SPI read function.
 * Reads through all registers up to the extended register space,
 * and compares values to default values.
 * 
 * @return true - If all registers are read correctly and have the right value.
 * @return false - If any register does not have the expected value,
 *                 or status byte is invalid.
 */
bool checkSPIRead() {
    setRegisterDefaults();
    uint8_t addr = 0x00;
    uint8_t data;
    
    while (addr < EXT_ADDR) {
        if (!arduinoReadSPI(addr, &data))
            return false;

        /* DEBUG
        Serial.print(addr);
        Serial.print(" : ");
        Serial.println(v, HEX);
        Serial.print("Expected: ");
        Serial.println(REGS_DEFAULTS[addr], HEX);
        */
        
        if (data != REGS_DEFAULTS[addr]) {
            Serial.print("ERROR. CC1120 read test failed.\n Register ");
            Serial.print(addr, HEX);
            Serial.print(" read ");
            Serial.print(data, HEX);
            Serial.print(", expected ");
            Serial.println(REGS_DEFAULTS[addr], HEX);
            return false;
        } else {
            addr++;
        }
    }
    
    if (!arduinoReadExtAddrSPI(MARCSTATE, &data) || data != 0x41) {
        Serial.print("ERROR. CC1120 read test failed.\n Extended address space register ");
        Serial.print(MARCSTATE, HEX);
        Serial.print(" read ");
        Serial.print(data, HEX);
        Serial.print(", expected ");
        Serial.println(0x41, HEX);
        return false;
    }

    Serial.println("CC1120 SPI read test passed");

    return true;
}

/**
 * @brief Unit test for SPI write function.
 * Writes to frequency offset register and extended address space RNDGEN register.
 * Reads the registers to see if the write was successful.
 * 
 * @return true - If both writes are successful.
 * @return false - If any register does not have the expected value,
 *                 or status byte is invalid.
 */
bool checkSPIWrite() {
    uint8_t data;
    if (!(arduinoWriteSPI(FREQOFF0, 0xFF) && arduinoReadSPI(FREQOFF0, &data) && data == 0xFF)) {
        Serial.println("ERROR. CC1120 SPI write test failed");
        Serial.println("FREQOFF0 read ");
        Serial.print(data, HEX);
        Serial.print(", expected ");
        Serial.println(0xFF, HEX);
        return false;
    }
    
    if (!(arduinoWriteExtAddrSPI(RNDGEN, 0xFF) && arduinoReadSPI(RNDGEN, &data) && data == 0xFF)) {
        Serial.println("ERROR. CC1120 SPI write test failed");
        Serial.println("RNDGEN read ");
        Serial.print(data, HEX);
        Serial.print(", expected ");
        Serial.println(0xFF, HEX);
        return false;
    }
    
    Serial.println("CC1120 SPI write test passed");
    return true;
}

/**
 * @brief Unit test for SPI strobe functionality.
 * Runs the reset strobe and checks the MARCSTATE register.
 * 
 * @return true - If MARCSTATE is 0x41 after reset.
 * @return false - If MARCSTATE is not 0x41 after reset, or status byte is invalid.
 */
bool checkStrobe() {
    uint8_t data;
    if (!(arduinoStrobeSPI(SRES) && arduinoReadSPI(MARCSTATE, &data) || data != 0x41)) {
        Serial.println("ERROR. CC1120 SPI strobe test failed");
        Serial.println("MARCSTATE read ");
        Serial.print(data, HEX);
        Serial.print(", expected ");
        Serial.println(0x41, HEX);
        return false;
    }
    
    Serial.println("CC1120 SPI strobe test passed");
    return true;    
}
