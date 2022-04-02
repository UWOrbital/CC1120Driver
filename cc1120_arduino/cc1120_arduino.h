#ifndef CC1120_ARDUINO_H
#define CC1120_ARDUINO_H

#include "cc1120_logging.h"
#include <stdint.h>

extern const uint8_t CC1120_RST;
extern const uint8_t CC1120_CS;
extern const uint8_t CC1120_MOSI;
extern const uint8_t CC1120_MISO;
extern const uint8_t CC1120_SCLK;

/**
 * @brief Set up the SPI pins and the CS pin, run E2E tests.
 * 
 */
void setup();

/**
 * @brief Empty loop.
 * 
 */
void loop();

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Logs a string to the serial port.
 * 
 * @param level - The log level.
 * @param str - The string to log.
 */
void arduino_serial_log(cc1120_log_level_t level, char str[]);

/**
 * @brief Simultaneously sends and receives a byte over CC1120 SPI interface
 * 
 * @param data - Data to transfer 
 * @return uint8_t - Data received from CC1120
 */
uint8_t arduino_cc1120_spi_transfer(uint8_t data);

/**
 * @brief Pulls the CS pin low.
 * 
 */
void arduino_cc1120_cs_assert();

/**
 * @brief Pulls the CS pin high.
 * 
 */
void arduino_cc1120_cs_deassert();

#ifdef __cplusplus
}
#endif

#endif /* CC1120_ARDUINO_H */
