#ifndef SETUP_TRANSMIT_CC1120_H
#define SETUP_TRANSMIT_CC1120_H

#include <stdbool.h>
#include <stdint.h>

bool setGSFK();
bool setSymbolRate(uint32_t symbolRate);
bool configureTransition(char mode[], char state[]);
bool flushRX();
bool flushTX();
bool setRFBandAndLODivider(uint8_t setting);
bool sendSTXStrobe();
bool sendSRXStrobe();
bool sendSPWDStrobe();
bool sendSWORStrobe();
bool setCheckSumConfig(uint8_t setting);
bool setAddressCheckConfig(uint8_t setting);
bool configLengthField(uint8_t setting, uint8_t numberOfBits);
bool configSync(uint8_t syncMode, uint8_t syncNumError, uint32_t syncWord);
bool configPreamble(uint8_t preambleWord, uint8_t numPreamble);
bool setVCOFrequency(uint32_t frequency);
bool setRFFrequency(uint32_t frequency);

#endif /* SETUP_TRANSMIT_CC1120_H */
