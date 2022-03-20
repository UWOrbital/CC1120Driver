#include "cc1120_arduino_tests.h"
#include "cc1120_spi.h"
#include "cc1120_spi_tests.h"
#include "cc1120_regs.h"

/**
 * @brief Set up the SPI pins and the CS pin, run E2E tests.
 * 
 */
void setup() {
    Serial.begin(9600);

    pinMode(CS, OUTPUT);
    digitalWrite(CS, HIGH);

    pinMode(RST, OUTPUT);
    digitalWrite(RST, HIGH);

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

            digitalWrite(RST, LOW);
            delay(1000);
            digitalWrite(RST, HIGH);
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
