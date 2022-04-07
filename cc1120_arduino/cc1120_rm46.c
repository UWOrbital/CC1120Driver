#include "cc1120_rm46.h"

/**
 * @brief Logs a string to the serial port.
 * 
 * @param level - The log level.
 * @param str - The string to log.
 */
void rm46_serial_log(cc1120_log_level_t level, char str[]) {
    /* Fill in later */
    return;
}

/**
 * @brief Logs a string to a file.
 * 
 * @param level  - The log level.
 * @param str - The string to log.
 */
void rm46_file_log(cc1120_log_level_t level, char str[]) {
    /* Fill in later */    
    return;
}

/**
 * @brief Simultaneously sends and receives a byte over CC1120 SPI interface
 * 
 * @param data - Data to transfer 
 * @return uint8_t - Data received from CC1120
 */
uint8_t rm46_cc1120_spi_transfer(uint8_t data) {
    /* Fill in later */
    return 0;
}

/**
 * @brief Pulls the CS pin low.
 * 
 */
void rm46_cc1120_cs_assert() {
    /* Fill in later */
    return;
}

/**
 * @brief Pulls the CS pin high.
 * 
 */
void rm46_cc1120_cs_deassert() {
    /* Fill in later */
    return;
}
