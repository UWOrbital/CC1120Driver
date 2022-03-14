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
    for(uint8_t i=0; i<3; i++) {
        status &= checkStrobe();
        status &= checkSPIRead();
        status &= checkSPIWrite();
        status &= checkFIFOReadWrite();
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

    if (!arduinoStrobeSPI(CC1120_REGS_STROBE_SRES)) {
        Serial.println("ERROR. CC1120 reset failed.");
    }
}

/**
 * @brief Empty loop.
 * 
 */
void loop() {

}
