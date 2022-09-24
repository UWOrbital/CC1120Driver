#include "cc1120_mcu.h"
#include "cc1120_arduino.h"
#include <stdio.h>
#include <stdbool.h>
#define MAX_LOG_SIZE 500U

cc1120_mcu_t CC1120_MCU = CC1120_MCU_ARDUINO;

/**
 * @brief Calls serial and file log functions. Appends log info to string.
 * 
 * @param level - The log level.
 * @param str - The string to log.
 * @param ... - The arguments to log.
 */
void mcu_log(cc1120_log_level_t level, char str[], ...) {
    va_list args;
    va_start(args, str);

    char buf[MAX_LOG_SIZE];
    vsnprintf(buf, MAX_LOG_SIZE, str, args);

    if (level <= CC1120_SERIAL_LOG_LEVEL)
        mcu_serial_log(level, buf);
    if (level <= CC1120_FILE_LOG_LEVEL)
        mcu_file_log(level, buf);

    va_end(args);
}

/**
 * @brief Calls the correct serial log function based on the MCU selected.
 * 
 * @param level - The log level.
 * @param str - The string to log.
 */
void mcu_serial_log(cc1120_log_level_t level, char str[]) {
    #ifdef CC1120_ARDUINO_H
    arduino_serial_log(level, str);
    #endif
    #ifdef CC1120_RM46_H
    rm46_serial_log(level, str);
    #endif
}

/**
 * @brief Calls the correct file log function based on the MCU selected.
 * 
 * @param level - The log level.
 * @param str - The string to log.
 */
void mcu_file_log(cc1120_log_level_t level, char str[]) {
    #ifdef CC1120_RM46_H
    rm46_file_log(level, str);
    #endif
}

/**
 * @brief Simultaneously sends and receives a byte over CC1120 SPI interface
 * 
 * @param data - Data to transfer 
 * @param csHold - Boolean that represents whether to hold CS low for consecutive sends and receives
 * @return uint8_t - Data received from CC1120
 */
uint8_t mcu_cc1120_spi_transfer(uint8_t data, bool csHold) {
    uint8_t received;
    #ifdef CC1120_ARDUINO_H
    received = arduino_cc1120_spi_transfer(data, csHold);
    #endif
    #ifdef CC1120_RM46_H
    received = rm46_cc1120_spi_transfer(data);
    #endif

    return received;
}