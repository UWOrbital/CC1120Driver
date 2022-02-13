#include "cc1120_spi.h"
#include "cc1120_regs.h"
#include "cc1120_ext_regs.h"
#include "arduino_unit_tests.h"
#include "Arduino.h"
#include <stdint.h>
#include <SPI.h>

extern "C" {



uint8_t arduinoReadSPI(uint8_t addr, union cc_st *status) {
    digitalWrite(CS, LOW);
    status->v = SPI.transfer(RW_BIT | addr);
    uint8_t v = SPI.transfer(0x00);
    digitalWrite(CS, HIGH);
    return v;
}

uint8_t arduinoReadExtAddrSPI(uint8_t addr) {
  /*
  uint8_t v;
  digitalWrite(CS, LOW);
  ccstatus.v = SPI.transfer(RW_BIT | EXT_ADDR);
  SPI.transfer(addr);
  v = SPI.transfer(0xff);
  digitalWrite(CS, HIGH);
  return v;
  */
}

}
