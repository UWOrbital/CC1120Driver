#include "cc1120_unit_tests.h"
#include "cc1120_spi.h"
#include "cc1120_regs.h"
#include "cc1120_ext_regs.h"
#include "Arduino.h"
#include <stdint.h>

extern "C" {

static uint8_t REGS_DEFAULTS[EXT_ADDR];

REGS_DEFAULTS[IOCFG0] = 0x06;
REGS_DEFAULTS[IOCFG1] = 0x07;
REGS_DEFAULTS[IOCFG2] = 0x3C;
REGS_DEFAULTS[SYNC3] = 0x93;
REGS_DEFAULTS[SYNC2] = 0x0B;
REGS_DEFAULTS[SYNC1] = 0x51;
REGS_DEFAULTS[SYNC0] = 0xDE;
REGS_DEFAULTS[SYNC_CFG1] = 0x0A;
REGS_DEFAULTS[SYNC_CFG0] = 0x17;
REGS_DEFAULTS[DEVIATION_M] = 0x06;
REGS_DEFAULTS[MODCFG_DEV_E] = 0x03;
REGS_DEFAULTS[DCFILT_CFG] = 0x0C;
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

uint8_t DEFAULT_STATUS_BYTE = 0x00;

union cc_st ccstatus;

bool checkSPIRead() {
    uint8_t addr = 0x00;
    
    while (addr < EXT_ADDR) {
        uint8_t v = arduinoReadSPI(addr, &ccstatus);
        if (ccstatus.ccst.chip_ready == 1) {
            Serial.print("CC1120 chip not ready");
            return false;
        } else if (v != REGS_DEFAULTS[addr]) {
            Serial.print("CC1120 reg ");
            Serial.print(addr, HEX);
            Serial.print(" read ");
            Serial.print(v, HEX);
            Serial.print(" expected ");
            Serial.println(REGS_DEFAULTS[addr], HEX);
            return false;
        } else {
            addr++;
        }
    }
    
    Serial.println("CC1120 SPI read test passed");

    return true;
}

bool checkSPIWrite() {
    return false;
}

}
