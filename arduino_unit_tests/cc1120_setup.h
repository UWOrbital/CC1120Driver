#ifndef SETUP_CC1120_H
#define SETUP_CC1120_H

int NtoKBits(int number, int start, int end);
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

#endif /* SETUP_CC1120_H */
