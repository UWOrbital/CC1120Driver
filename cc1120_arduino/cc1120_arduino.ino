#include "cc1120_arduino.h"

extern "C" {
#include "cc1120_spi.h"
#include "cc1120_spi_tests.h"
}

#include "cc1120_regs.h"
#include <SPI.h>

const uint8_t CC1120_RST = 48;
const uint8_t CC1120_CS = 53;
const uint8_t CC1120_MOSI = 51;
const uint8_t CC1120_MISO = 50;
const uint8_t CC1120_SCLK = 52;

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
    bool status = true;
    uint8_t i;
    for(i = 0; i < 3; i++) {
        status &= cc1120_test_spi_strobe();
        status &= cc1120_test_spi_read();
        status &= cc1120_test_spi_write();
        status &= cc1120_test_fifo_read_write();
        if (status) {
            Serial.println("All CC1120 tests passed. Resetting the chip...");
            break;
        } else {
            Serial.print("CC1120 tests failed. Trying again... (");
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
 * @param csHold - Boolean that represents whether to hold CS low for consecutive sends and receives
 * @return uint8_t - Data received from CC1120
 */
uint8_t arduino_cc1120_spi_transfer(uint8_t data, bool csHold) {
    digitalWrite(CC1120_CS, LOW);
    uint8_t readData = SPI.transfer(data);
    if (!csHold) 
        digitalWrite(CC1120_CS, HIGH);
    return readData;
}