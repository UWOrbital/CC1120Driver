#ifndef CC1120_TXRX_H
#define CC1120_TXRX_H

#include <stdint.h>
#include "cc1120_regs.h"
#include "cc1120_logging.h"

typedef struct {
    uint8_t addr;
    uint8_t val;
} registerSetting_t;

cc1120_status_code cc1120_tx_init();

cc1120_status_code cc1120_send();

#endif /* CC1120_TXRX_H */
