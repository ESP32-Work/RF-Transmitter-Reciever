#include <Arduino.h>
#include "transmitter.h"
#include "receiver.h"
#include <SPI.h>

#define RADIOHEAD_BAUD 2000
#define RADIOHEAD_TX_PIN 2
#define RADIOHEAD_RX_PIN 7

Transmitter transmitter(RADIOHEAD_BAUD, RADIOHEAD_TX_PIN);
Receiver receiver(RADIOHEAD_BAUD, RADIOHEAD_RX_PIN);


void setup()
{
  Serial.begin(9600);
  transmitter.initialize();
  receiver.initialize();
}

void loop()
{
  const char *msg = "hello";
  transmitter.sendMessage(msg);
  Serial.println("Sent: ");
  delay(200);
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (receiver.receiveMessage(buf, &buflen))
  { // Non-blocking
    Serial.println("Received: ");
    receiver.printBuffer("Got:", buf, buflen);
    Serial.println((char*)buf); // Print the received message
  }
}
