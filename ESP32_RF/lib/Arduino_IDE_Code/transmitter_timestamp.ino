#include <RH_ASK.h>
#include <SPI.h>
#include <TimeLib.h>

RH_ASK rf_driver(2000, 21, 22);

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

String getCurrentTimeStamp() {
  // Get the current time
  time_t currentTime = now();

  // Convert the time to a string
  char timeStamp[20];
  sprintf(timeStamp, "%04d-%02d-%02d %02d:%02d:%02d", year(currentTime), month(currentTime), day(currentTime), hour(currentTime), minute(currentTime), second(currentTime));

  return String(timeStamp);
}

void loop() {
  // Create a message with the current time stamp
  String msg = "landmark1,Time=" + getCurrentTimeStamp();

  // Print the message before transmitting
  Serial.print("Transmitting packet: ");
  Serial.println(msg);

  // Send the message
  rf_driver.send((uint8_t *)msg.c_str(), msg.length() + 1);
  rf_driver.waitPacketSent();

  delay(500);
}
