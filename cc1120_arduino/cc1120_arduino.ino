#include "cc1120_arduino.h"

extern "C" {
#include "cc1120_spi.h"
#include "cc1120_spi_tests.h"
#include "cc1120_txrx.h"
}

#include "cc1120_logging.h"
#include "cc1120_regs.h"
#include <SPI.h>

const uint8_t CC1120_RST = 49;
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
    cc1120_status_code status;
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
        } else {
            Serial.print("CC1120 tests failed. ");
            Serial.print("Error Code: ");
            Serial.println(status);
            Serial.print("Trying again... (");
            Serial.print(i+1);
            Serial.println("/3)");

            digitalWrite(CC1120_RST, LOW);
            delay(1000);
            digitalWrite(CC1120_RST, HIGH);
        }
    }

    if (cc1120_strobe_spi(CC1120_STROBE_SRES) != CC1120_ERROR_CODE_SUCCESS) {
        Serial.println("ERROR. CC1120 reset failed.");
        return;
    }

    if (cc1120_tx_init() != CC1120_ERROR_CODE_SUCCESS) {
        Serial.println("ERROR. TX initialization failed.");
        return;
    }


    uint8_t stateNum;
    uint8_t numPackets;
    cc1120_get_state(&stateNum);
    Serial.print("State number: ");
    Serial.println(stateNum);
    cc1120_get_packets_in_tx_fifo(&numPackets);
    Serial.print("Num packets in TX FIFO: ");
    Serial.println(numPackets);

    Serial.print("Sending 'Hello World' in 3..");
    delay(1000);
    Serial.print(" 2...");
    delay(1000);
    Serial.println(" 1...");

    
    for (int i=0; i<100; i++) {
        uint8_t testTxData[] = "Hello World";
        status = cc1120_send(testTxData, sizeof(testTxData)/sizeof(uint8_t));
        if (status) {
            Serial.print("Failed");
            Serial.println(status);
        }
    
        cc1120_get_state(&stateNum);
        Serial.print("State number: ");
        Serial.println(stateNum);
        cc1120_get_packets_in_tx_fifo(&numPackets);
        Serial.print("Num packets in TX FIFO: ");
        Serial.println(numPackets);

        delay(1000);
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
