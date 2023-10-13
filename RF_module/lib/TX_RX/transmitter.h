#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <Arduino.h>
#include <RH_ASK.h>

class Transmitter {
public:
    Transmitter(uint16_t baud, uint8_t txPin);
    void initialize();
    void sendMessage(const char *msg);

private:
    RH_ASK driver;
    uint8_t txPin;
};

#endif // TRANSMITTER_H
