#ifndef SETUP_RECIEVE_CC1120_H
#define SETUP_RECIEVE_CC1120_H

#include <stdbool.h>
#include <stdint.h>

bool set_rx_filter_bandwidth(uint32_t RXFilterBandwidth);
bool get_rx_filter_bandwidth(uint32_t *RXFilterBandwidth);
bool set_agc_ref(uint8_t RSSIOffset);

#endif /* SETUP_RECIEVE_CC1120_H */
