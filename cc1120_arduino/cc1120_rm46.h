#ifndef CC1120_RM46_H
#define CC1120_RM46_H

#include "cc1120_logging.h"
#include <stdarg.h>
#include <stdint.h>

/**
 * @brief Logs a string to the serial port.
 * 
 * @param level - The log level.
 * @param str - The string to log.
 */
void rm46_serial_log(cc1120_log_level_t level, char str[]);

/**
 * @brief Logs a string to a file.
 * 
 * @param level  - The log level.
 * @param str - The string to log.
 */
void rm46_file_log(cc1120_log_level_t level, char str[]);

/**
 * @brief Simultaneously sends and receives a byte over CC1120 SPI interface
 * 
 * @param data - Data to transfer 
 * @return uint8_t - Data received from CC1120
 */
uint8_t rm46_cc1120_spi_transfer(uint8_t data);

#endif /* CC1120_RM46_H */
