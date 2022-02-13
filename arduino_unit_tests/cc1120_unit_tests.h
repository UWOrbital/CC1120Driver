#ifndef UNIT_TESTS_CC1120_H
#define UNIT_TESTS_CC1120_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool checkSPIRead();
bool checkSPIWrite();

#ifdef __cplusplus
}
#endif

#endif /* UNIT_TESTS_CC1120_H */
