#include "cc1120_arduino.h"

extern "C" {
#include "cc1120_spi.h"
#include "cc1120_spi_tests.h"
}

#include "cc1120_regs.h"
#include <SPI.h>

const uint8_t CC1120_RST = 9;
const uint8_t CC1120_CS = 10;
const uint8_t CC1120_MOSI = 11;
const uint8_t CC1120_MISO = 12;
const uint8_t CC1120_SCLK = 13;

/**
 * @brief Set up the SPI pins and the CS pin, run E2E tests.
 * 
 */
void setup() {
    Serial.begin(9600);

    pinMode(CC1120_CS, OUTPUT);
    digitalWrite(CC1120_CS, HIGH);

    pinMode(CC1120_RST, OUTPUT);
    digitalWrite(CC1120_RST, HIGH);

    SPI.begin();
    delay(1000);

    Serial.println("Starting E2E tests...");
    cc1120_error_code status;
    uint8_t i;
    for(i = 0; i < 3; i++) {
        status = cc1120_test_spi_strobe();

        if (status == CC1120_ERROR_CODE_SUCCESS)
            status = cc1120_test_spi_read();

        if (status == CC1120_ERROR_CODE_SUCCESS)
            status = cc1120_test_spi_write();

        if (status == CC1120_ERROR_CODE_SUCCESS)
            status = cc1120_test_fifo_read_write();

        if (status == CC1120_ERROR_CODE_SUCCESS) {
            Serial.println("All CC1120 tests passed. Resetting the chip...");
            break;
        } 
        
        else {
            Serial.print("CC1120 tests failed. Error Code: %i Trying again... (", status);
            Serial.print(i+1);
            Serial.println("/3)");

            digitalWrite(CC1120_RST, LOW);
            delay(1000);
            digitalWrite(CC1120_RST, HIGH);
        }
    }

    if (!cc1120_strobe_spi(CC1120_STROBE_SRES)) {
        Serial.println("ERROR. CC1120 reset failed.");
    }
}

/**
 * @brief Empty loop.
 * 
 */
void loop() {

}

/**
 * @brief Logs a string to the serial port.
 * 
 * @param level - The log level.
 * @param str - The string to log.
 */
void arduino_serial_log(cc1120_log_level_t level, char str[]) {
    Serial.print(str);
    return;
}

/**
 * @brief Simultaneously sends and receives a byte over CC1120 SPI interface
 * 
 * @param data - Data to transfer 
 * @return uint8_t - Data received from CC1120
 */
uint8_t arduino_cc1120_spi_transfer(uint8_t data) {
    return SPI.transfer(data);
}

/**
 * @brief Pulls the CS pin low.
 * 
 */
void arduino_cc1120_cs_assert() {
    digitalWrite(CC1120_CS, LOW);
    return;
}

/**
 * @brief Pulls the CS pin high.
 * 
 */
void arduino_cc1120_cs_deassert() {
    digitalWrite(CC1120_CS, HIGH);
    return;
}
