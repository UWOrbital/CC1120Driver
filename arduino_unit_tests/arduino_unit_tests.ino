/**
 * @file arduino_unit_tests.ino
 * @author Aryan Kalia
 * @brief File containing the arduino unit tests for the cc1120_spi.cpp file.
 * @version 0.1
 * @date 2022-02-13
 * 
 */

#include <SPI.h>
#include "arduino_unit_tests.h"
#include "cc1120_spi.h"
#include "cc1120_unit_tests.h"
#include "arduino_unit_tests.h"

/**
 * @brief Set up the SPI pins and the CS pin, run unit tests.
 * 
 */
void setup() {
    Serial.begin(9600);

    pinMode(CS, OUTPUT);
    digitalWrite(CS, HIGH);
    SPI.begin();
    delay(1000);

    Serial.println("Starting unit tests...");
    bool status = true;
    status &= checkSPIRead();
    status &= checkSPIWrite();
    status &= checkStrobe();
    if(!status)
        Serial.println("CC1120 tests failed.");
    else
        Serial.println("All CC1120 tests passed.");
}

/**
 * @brief Empty loop.
 * 
 */
void loop() {

}
