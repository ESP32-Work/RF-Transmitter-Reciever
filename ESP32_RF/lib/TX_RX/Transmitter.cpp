#include "Transmitter.h"

Transmitter::Transmitter() : rf_driver(2000, 21, 22) {}

void Transmitter::initialize() {
    if (!rf_driver.init()) {
        Serial.println("Transmitter init failed");
        while (1)
            delay(10000);
    }
    Serial.println("Transmitter: rf_driver initialised");
}

void Transmitter::sendMessage(const char *msg) {
    rf_driver.send((uint8_t *)msg, strlen(msg) + 1);
    rf_driver.waitPacketSent();
}
