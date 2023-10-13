#include "transmitter.h"

Transmitter::Transmitter(uint16_t baud, uint8_t txPin) : driver(baud, -1, txPin), txPin(txPin) {}

void Transmitter::initialize() {
    driver.init();
}

void Transmitter::sendMessage(const char *msg) {
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
}
