#ifndef RECEIVER_H
#define RECEIVER_H

#include <Arduino.h>
#include <RH_ASK.h>

class Receiver {
public:
    Receiver(uint16_t baud, uint8_t rxPin);
    void initialize();
    bool receiveMessage(uint8_t *buf, uint8_t *buflen);

private:
    RH_ASK driver;
};

#endif // RECEIVER_H
