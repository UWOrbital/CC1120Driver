#include <SPI.h>

const int CS = 10;
const int MOSI_PIN = 11;
const int MISO_PIN = 12;
const int SCLK = 13;

#define RW_BIT 1 << 7
#define EXT_ADDR 0x2f

struct cc_status {
  uint8_t res : 4;
  uint8_t state : 3;
  uint8_t chip_ready : 1;
};
union cc_st {
  struct cc_status ccst;
  uint8_t v;
};

union cc_st ccstatus;
uint8_t readSPI(uint8_t addr);
uint8_t readExtAddrSPI(uint8_t addr);
void setup() {
  Serial.begin(9600);
  
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  delay(1000);

  Serial.println("Start");
  SPI.begin();
  uint8_t addr = 0x00;
  while (addr < EXT_ADDR) {
    uint8_t v = readSPI(addr);
    Serial.print(addr);
    Serial.print(": ");
    Serial.println(v);
    addr++;
  }

  Serial.println(readExtAddrSPI(0x73));
  
}

void loop() {

}

uint8_t readSPI(uint8_t addr) {
  digitalWrite(CS, LOW);
  ccstatus.v = SPI.transfer(RW_BIT | addr);
  uint8_t v = SPI.transfer(0x00);
  digitalWrite(CS, HIGH);
  Serial.print("Status: ");
  Serial.println(ccstatus.v);
  return v;
}

uint8_t readExtAddrSPI(uint8_t addr) {
  uint8_t v;
  digitalWrite(CS, LOW);
  ccstatus.v = SPI.transfer(RW_BIT | EXT_ADDR);
  SPI.transfer(addr);
  v = SPI.transfer(0xff);
  digitalWrite(CS, HIGH);
  return v;
}
