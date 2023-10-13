// ESP8266 433MHz transmitter test 1

#include <RH_ASK.h>  // Include RadioHead Amplitude Shift Keying Library
#include <SPI.h>     // Include dependant SPI Library

//RH_ASK(uint16_t speed = 2000, uint8_t rxPin = 11, uint8_t txPin = 12, uint8_t pttPin = 10, bool pttInverted = false);
RH_ASK rf_driver(2000, 21, 22);  // ESP8266 Create Amplitude Shift Keying Object

void setup() {
  Serial.begin(115200);
  delay(4000);
  Serial.println("ESP8266 433MHz transmitter");
  if (!rf_driver.init()) {
    Serial.println("init failed");
    while (1) delay(10000);
  }
  Serial.println("Transmitter: rf_driver initialised");
}

// transmit packet every 5 seconds
void loop() {
  Serial.println("Transmitting packet");
  const char *msg = "Location:X=10.0,Y=20.0";
  rf_driver.send((uint8_t *)msg, strlen(msg)+1);
  rf_driver.waitPacketSent();
  delay(5000);
}