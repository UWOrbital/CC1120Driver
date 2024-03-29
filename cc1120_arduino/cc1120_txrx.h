#ifndef CC1120_TXRX_H
#define CC1120_TXRX_H

#include <stdint.h>
#include "cc1120_regs.h"
#include "cc1120_logging.h"

#define CC1120_MAX_PACKET_LEN 255
#define CC1120_TX_FIFO_SIZE 128

typedef struct {
    uint8_t addr;
    uint8_t val;
} registerSetting_t;

/**
 * @brief Gets the number of packets queued in the TX FIFO
 * 
 * @param numPackets - A pointer to an 8-bit integer to store the number of packets in
 * @return cc1120_status_code - Whether or not the registe read was successful
 */
cc1120_status_code cc1120_get_packets_in_tx_fifo(uint8_t *numPackets);

/**
 * @brief Gets the state of the CC1120 from the MARCSTATE register
 * 
 * @param stateNum - A pointer to an 8-bit integer to store the state in
 * @return cc1120_status_code - Whether or not the register read was successful
 */
cc1120_status_code cc1120_get_state(uint8_t *stateNum);

/**
 * @brief Resets CC1120 & initializes transmit mode
 * 
 * @return cc1120_status_code - Whether or not the setup was a success
 */
cc1120_status_code cc1120_tx_init();

/**
 * @brief Adds the given data to the CC1120 FIFO buffer and transmits
 * 
 * @param data - An array of 8-bit data to transmit
 * @param len - The size of the provided array
 * @return cc1120_status_code 
 */
cc1120_status_code cc1120_send(uint8_t *data, uint32_t len);

#endif /* CC1120_TXRX_H */
