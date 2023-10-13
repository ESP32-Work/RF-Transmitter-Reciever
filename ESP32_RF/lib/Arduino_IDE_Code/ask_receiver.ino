// ESP32 433MHz receiver test 1

#include <RH_ASK.h>  // Include RadioHead Amplitude Shift Keying Library
#include <SPI.h>     // Include dependant SPI Library

RH_ASK rf_driver(2000, 21, 22);   // ESP32 Create Amplitude Shift Keying Object

void setup() {
  Serial.begin(115200);
  delay(4000);
  Serial.println("ESP32 433MHz receiver");
  if (RH_PLATFORM == RH_PLATFORM_ESP32)
    Serial.println("RH_PLATFORM_ESP32");
  delay(5000);
  Serial.println("Receiver: rf_driver initialising");
  if (!rf_driver.init()) {
    Serial.println("init failed");
    while (1) delay(1000);
  }
  Serial.println("Receiver: rf_driver initialised");
}

void loop() {
  uint8_t buf[20]={0};  // Set buffer to size of expected message
  uint8_t buflen = sizeof(buf);
  // Check if received packet is correct size
  if (rf_driver.recv(buf, &buflen)) {
    // Message received with valid checksum
    Serial.print("Message Received: ");
    Serial.println((char*)buf);
  }
}