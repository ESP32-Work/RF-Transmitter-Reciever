#include "Receiver.h"

Receiver::Receiver() : rf_driver(2000, 21, 22) {}

void Receiver::initialize() {
    if (!rf_driver.init()) {
        Serial.println("Receiver init failed");
        while (1)
            delay(10000);
    }
    Serial.println("Receiver: rf_driver initialised");
}

bool Receiver::receiveMessage(uint8_t *buf, uint8_t *buflen) {
    return rf_driver.recv(buf, buflen);
}
