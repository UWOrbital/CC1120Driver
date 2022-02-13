#ifndef SPI_CC1120_H
#define SPI_CC1120_H

#include <stdint.h>

#define RW_BIT 1 << 7
#define BURST_BIT 1 << 6

struct cc_status {
  uint8_t res : 4;
  uint8_t state : 3;
  uint8_t chip_ready : 1;
};

union cc_st {
  struct cc_status ccst;
  uint8_t v;
};

uint8_t arduinoReadSPI(uint8_t addr, union cc_st *status);
uint8_t arduinoReadExtAddrSPI(uint8_t addr);
uint8_t arduinoWriteSPI(uint8_t addr, uint8_t data);
uint8_t arduinoWriteExtAddrSPI(uint8_t addr, uint8_t data);
uint8_t arduinoStrobeSPI(uint8_t addr);
uint8_t arduinoReadFIFO(uint8_t addr);

#endif /* SPI_CC1120_H */
