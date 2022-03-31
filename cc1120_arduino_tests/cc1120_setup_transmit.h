#ifndef SETUP_TRANSMIT_CC1120_H
#define SETUP_TRANSMIT_CC1120_H

#define FXOSC 32000000
void setGSFK();
bool setSymbolRate(int symbolRate);
bool configureTransition(char mode[], char state[]);
bool flushRX();
bool flushTX();
bool setRFBandAndLODivider(int setting);
bool sendSTXStrobe();
bool sendSRXStrobe();
bool sendSPWDStrobe();
bool sendSWORStrobe();
bool setCheckSumConfig(int setting);
bool setAddressCheckConfig(int setting);
bool configLengthField(int setting, int numberOfBits);
bool configSync(int syncMode, int syncNumError, long int syncWord);
bool configPreamble(int preambleWord, int numPreamble);
bool setVCOFrequency(int frequency);
bool setRFFrequency(int frequency);

#endif /* SETUP_TRANSMIT_CC1120_H */
