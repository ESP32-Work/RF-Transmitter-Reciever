#ifndef RECEIVER_H
#define RECEIVER_H

#include <RH_ASK.h>
#include <SPI.h>

class Receiver {
public:
    Receiver();
    void initialize();
    bool receiveMessage(uint8_t *buf, uint8_t *buflen);

private:
    RH_ASK rf_driver;
};

#endif
