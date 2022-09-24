#ifndef CC1120_MCU_H
#define CC1120_MCU_H

#include "cc1120_logging.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    CC1120_MCU_UNKNOWN = 0,
    CC1120_MCU_ARDUINO,
    CC1120_MCU_RM46,
} cc1120_mcu_t;

extern cc1120_mcu_t CC1120_MCU;
/**
 * @brief Calls serial and file log functions. Appends log info to string.
 * 
 * @param level - The log level.
 * @param str - The string to log.
 * @param ... - The arguments to log.
 */
void mcu_log(cc1120_log_level_t level, char str[], ...);

/**
 * @brief Calls the correct serial log function based on the MCU selected.
 * 
 * @param level - The log level.
 * @param str - The string to log.
 */
void mcu_serial_log(cc1120_log_level_t level, char str[]);

/**
 * @brief Calls the correct file log function based on the MCU selected.
 * 
 * @param level - The log level.
 * @param str - The string to log.
 */
void mcu_file_log(cc1120_log_level_t level, char str[]);

/**
 * @brief Simultaneously sends and receives a byte over CC1120 SPI interface
 * 
 * @param data - Data to transfer 
 * @param csHold - Boolean that represents whether to hold CS low for consecutive sends and receives
 * @return uint8_t - Data received from CC1120
 */
uint8_t mcu_cc1120_spi_transfer(uint8_t data, bool csHold);

#endif /* CC1120_MCU_H */