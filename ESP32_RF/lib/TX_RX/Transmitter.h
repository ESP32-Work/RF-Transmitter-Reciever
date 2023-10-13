#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <RH_ASK.h>
#include <SPI.h>

class Transmitter {
public:
    Transmitter();
    void initialize();
    void sendMessage(const char *msg);

private:
    RH_ASK rf_driver;
};

#endif
