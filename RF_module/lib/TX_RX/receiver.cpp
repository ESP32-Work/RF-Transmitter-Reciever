#include "receiver.h"

Receiver::Receiver(uint16_t baud, uint8_t rxPin) : driver(baud, rxPin, -1) {}

void Receiver::initialize() {
    driver.init();
}

bool Receiver::receiveMessage(uint8_t *buf, uint8_t *buflen) {
    return driver.recv(buf, buflen);
}
