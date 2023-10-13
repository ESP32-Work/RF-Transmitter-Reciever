#include <Arduino.h>
#include "Transmitter.h"
#include "Receiver.h"

Transmitter transmitter;
Receiver receiver;

void setup() {
    // Initialization code
    Serial.begin(115200);
    delay(4000);
    // Initialize the transmitter and receiver
    transmitter.initialize();
    receiver.initialize();
}

void loop() {
    // Transmitter code
    const char *msg = "Location:X=10.0,Y=no";
    transmitter.sendMessage(msg);
    delay(5000);

    // Receiver code
    uint8_t buf[20] = {0};
    uint8_t buflen = sizeof(buf);
    if (receiver.receiveMessage(buf, &buflen)) {
        Serial.print("Message Received: ");
        Serial.println((char *)buf);
    }
}
