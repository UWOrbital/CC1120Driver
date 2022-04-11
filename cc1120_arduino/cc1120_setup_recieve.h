#ifndef SETUP_RECIEVE_CC1120_H
#define SETUP_RECIEVE_CC1120_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Sets RX Filter Bandwidth.
 *
 * @param RXFilterBandwidth - The RX Filter Bandwidth to set to.
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool set_rx_filter_bandwidth(uint32_t RXFilterBandwidth);

/**
 * @brief Gets the RX Filter Bandwidth.
 *
 * @param RXFilterBandwidth - The address to write the result the value of the RX Filter Bandwidth to.
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool get_rx_filter_bandwidth(uint32_t *RXFilterBandwidth);

/**
 * @brief Sets optimal AGC Reference using Equation 12, based off of RSSI Offset.
 *
 * @param RSSIOffset - The RSSI Offset.
 * @return true - If the operation was successful
 * @return false - If the operation was not successful.
 */
bool set_agc_ref(uint8_t RSSIOffset);

#endif /* SETUP_RECIEVE_CC1120_H */
