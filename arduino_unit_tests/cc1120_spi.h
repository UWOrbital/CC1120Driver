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
  uint8_t v;
};

bool arduinoReadSPI(uint8_t addr, uint8_t *data);
bool arduinoReadExtAddrSPI(uint8_t addr, uint8_t *data);
bool arduinoWriteSPI(uint8_t addr, uint8_t data);
bool arduinoWriteExtAddrSPI(uint8_t addr, uint8_t data);
bool arduinoStrobeSPI(uint8_t addr);
bool arduinoReadFIFO(uint8_t *data);
bool arduinoWriteFIFO(uint8_t *data);
bool arduinoReadFIFODirect(uint8_t addr, uint8_t *data);
bool arduinoWriteFIFODirect(uint8_t addr, uint8_t data);
bool sendByteReceiveStatus(uint8_t data);

#endif /* SPI_CC1120_H */
