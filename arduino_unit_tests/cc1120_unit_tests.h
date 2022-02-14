#ifndef UNIT_TESTS_CC1120_H
#define UNIT_TESTS_CC1120_H

#include <stdbool.h>
#include <stdint.h>

void setRegisterDefaults();
bool checkSPIRead();
bool checkSPIWrite();

#endif /* UNIT_TESTS_CC1120_H */
