# RF Communication with ESP32

## Table of Contents
- [Introduction](#introduction)
- [Libraries](#libraries)
- [Setup](#setup)
  - [Transmitter](#transmitter-setup)
  - [Receiver](#receiver-setup)
- [Functionality](#functionality)
  - [Transmitter](#transmitter-functionality)
  - [Receiver](#receiver-functionality)
- [Usage](#usage)
- [Code Snippets](#code-snippets)
  - [main.cpp](#maincpp)
  - [Receiver.h](#receiverh)
  - [Transmitter.h](#transmitterh)
  - [ask_receiver.ino](#ask-receiver-ino)
  - [ask_transmitter.ino](#ask-transmitter-ino)
  - [receiver_toa_calculation.ino](#receiver-toa-calculation-ino)
  - [transmitter_timestamp.ino](#transmitter-timestamp-ino)
- [License](#license)

---

## Introduction

This project demonstrates how to establish RF (Radio Frequency) communication between two ESP32 devices using the RadioHead library. Originally designed for basic message exchange, the project has evolved to include location-based information transmission.

---

## Libraries

This project utilizes the following libraries:

- **RadioHead Library**: The RadioHead library is used for encoding and decoding data for RF communication. It supports various RF transceivers, and in this project, we use it to send and receive messages wirelessly.

---

## Setup

### Transmitter Setup

- Connect a 433MHz transmitter module to your ESP32.
- Ensure that the transmitter's data pin is connected to the specified pin (`RADIOHEAD_TX_PIN`) in the code.

### Receiver Setup

- Connect a 433MHz receiver module to your ESP32.
- Ensure that the receiver's data pin is connected to the specified pin (`RADIOHEAD_RX_PIN`) in the code.

---

## Functionality

### Transmitter Functionality

- The transmitter sends a specific location message in a continuous loop: "Location:X=10.0,Y=no."
- The message is sent using the RadioHead library, and the driver waits until the packet is sent successfully before proceeding.

### Receiver Functionality

- The receiver continuously listens for incoming RF transmissions.
- When a valid message is received, it is printed to the serial monitor.

---

## Usage

1. Clone or download this repository to your local computer.
2. Open the Platform io IDE and load the `main.cpp` sketch.
3. Upload the sketch to your transmitter and receiver ESP32s, ensuring the appropriate pin connections.
4. Open the Serial Monitor for the receiver ESP32 to view received messages.
5. The transmitter will continuously send the "Location:X=10.0,Y=no" message to the receiver, and you will see the messages displayed on the Serial Monitor.

---

## Code Snippets

### main.cpp

```cpp
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
```

### Receiver.h
```cpp
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


```

### Transmitter.h
```cpp
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

```

### ask_reciever.ino
```ino
// ESP32 433MHz receiver test 1
#include <RH_ASK.h>  
#include <SPI.h>     

RH_ASK rf_driver(2000, 21, 22);  

void setup() {
  Serial.begin(115200);
  delay(4000);
  Serial.println("ESP32 433MHz receiver");
  if (RH_PLATFORM == RH_PLATFORM_ESP32)
    Serial.println("RH_PLATFORM_ESP32");
  delay(5000);
  Serial.println("Receiver: rf_driver initialising");
  if (!rf_driver.init()) {
    Serial.println("init failed");
    while (1) delay(1000);
  }
  Serial.println("Receiver: rf_driver initialised");
}

void loop() {
  uint8_t buf[20]={0};  
  uint8_t buflen = sizeof(buf);
  if (rf_driver.recv(buf, &buflen)) {
    Serial.print("Message Received: ");
    Serial.println((char*)buf);
  }
}

```

### ask_transmitter.ino

```ino
// ESP8266 433MHz transmitter test 1
#include <RH_ASK.h>  
#include <SPI.h>     

RH_ASK rf_driver(2000, 21, 22);  

void setup() {
  Serial.begin(115200);
  delay(4000);
  Serial.println("ESP8266 433MHz transmitter");
  if (!rf_driver.init()) {
    Serial.println("init failed");
    while (1) delay(10000);
  }
  Serial.println("Transmitter: rf_driver initialised");
}

void loop() {
  Serial.println("Transmitting packet");
  const char *msg = "Location:X=10.0,Y=20.0";
  rf_driver.send((uint8_t *)msg, strlen(msg)+1);
  rf_driver.waitPacketSent();
  delay(5000);
}

```
### receiver_toa_calculation.ino

[View Code](./ESP32_RF/lib/Arduino_IDE_Code/receiver_toa_calculation.ino)

This Arduino sketch is designed for an ESP32-based 433MHz receiver with Time of Arrival (TOA) calculation capabilities. Here's a summary of its functionality:

1. **Libraries and Initialization**:
   - The code includes the necessary libraries such as `RH_ASK` for ASK communication, `SPI` for Serial Peripheral Interface, and `TimeLib` for timestamp handling.
   - The `rf_driver` object is created for ASK communication with specific pins (2000, 21, 22).
   - Constants and variables for TOA calculation, including `MAX_MESSAGES`, `PROPAGATION_SPEED`, `NUM_MESSAGES_TO_PROCESS`, `messages`, and `messageIndex`, are defined.

2. **Setup Function**:
   - Serial communication is initiated.
   - The platform information is printed to the serial monitor.
   - The `rf_driver` is initialized, and if unsuccessful, the code enters an error loop.
   - A FreeRTOS task (`calculateDistanceTask`) is created for TOA calculations.

3. **Loop Function**:
   - The loop continuously checks for incoming messages using `rf_driver.recv()`.
   - When a message is received, it is printed to the serial monitor.
   - The received messages are saved in an array (`messages`) for TOA calculations.
   - A short delay is introduced to avoid high CPU load.
```ino
// ESP32 433MHz receiver with Time of Arrival (TOA) calculation
#include <RH_ASK.h>
#include <SPI.h>
#include <TimeLib.h>

RH_ASK rf_driver(2000, 21, 22);

const int MAX_MESSAGES = 10;
String messages[MAX_MESSAGES];
int messageIndex = 0;

const double PROPAGATION_SPEED = 3e8; 
const int NUM_MESSAGES_TO_PROCESS = 5; 

// ... (Explanations of the TOA calculation task)

void setup() {
  Serial.begin(115200);
  delay(4000);
  Serial.println("ESP32 433MHz receiver");
  if (RH_PLATFORM == RH_PLATFORM_ESP32)
    Serial.println("RH_PLATFORM_ESP32");
  delay(5000);
  Serial.println("Receiver: rf_driver initialising");
  if (!rf_driver.init()) {
    Serial.println("init failed");
    while (1) delay(1000);
  }
  Serial.println("Receiver: rf_driver initialised");

  xTaskCreate(
      calculateDistanceTask,  
      "CalculateDistance",    
      10000,                   
      NULL,                    
      1,                       
      NULL);                   
}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN] = {0};
  uint8_t buflen = sizeof(buf);

  if (rf_driver.recv(buf, &buflen)) {
    Serial.print("Message Received: ");
    Serial.println((char *)buf);

    // ... (Saving received messages for TOA calculation)
  }
  vTaskDelay(10 / portTICK_PERIOD_MS); 
}
```
### transmitter_timestamp.ino

[View Code](./ESP32_RF/lib/Arduino_IDE_Code/transmitter_timestamp.ino)

This Arduino sketch serves as an ESP8266-based 433MHz transmitter that sends messages with a timestamp. Here's a summary of its functionality:

1. **Libraries and Initialization**:
   - The code includes the necessary libraries such as `RH_ASK` for ASK communication, `SPI` for Serial Peripheral Interface, and `TimeLib` for timestamp handling.
   - The `rf_driver` object is created for ASK communication with specific pins (2000, 21, 22).

2. **Setup Function**:
   - Serial communication is initiated for debugging purposes.
   - The `rf_driver` is initialized, and if unsuccessful, the code enters an error loop.
   - A message indicating successful initialization is printed to the serial monitor.

3. **Timestamp Generation Function**:
   - The `getCurrentTimeStamp` function retrieves the current time and converts it into a timestamp string in the format "YYYY-MM-DD HH:mm:ss".

4. **Loop Function**:
   - In the loop function, a message is created by concatenating a custom identifier ("landmark1") with the timestamp obtained from `getCurrentTimeStamp`.
   - The message is printed to the serial monitor for debugging.
   - The message is then sent using `rf_driver.send()`, and the code waits for the packet to be successfully sent using `rf_driver.waitPacketSent()`.
   - A delay of 500 milliseconds is introduced to control the transmission rate.


```ino
// ESP8266 433MHz transmitter with timestamp
#include <RH_ASK.h>
#include <SPI.h>
#include <TimeLib.h>

RH_ASK rf_driver(2000, 21, 22);

void setup() {
  Serial.begin(115200);
  delay(4000);
  Serial.println("ESP8266 433MHz transmitter");
  if (!rf_driver.init()) {
    Serial.println("init failed");
    while (1) delay(10000);
  }
  Serial.println("Transmitter: rf_driver initialised");
}

String getCurrentTimeStamp() {
  // Get the current time
  time_t currentTime = now();

  // Convert the time to a string
  char timeStamp[20];
  sprintf(timeStamp, "%04d-%02d-%02d %02d:%02d:%02d", year(currentTime), month(currentTime), day(currentTime), hour(currentTime), minute(currentTime), second(currentTime));

  return String(timeStamp);
}

void loop() {
  // Create a message with the current time stamp
  String msg = "landmark1,Time=" + getCurrentTimeStamp();

  // Print the message before transmitting
  Serial.print("Transmitting packet: ");
  Serial.println(msg);

  // Send the message
  rf_driver.send((uint8_t *)msg.c_str(), msg.length() + 1);
  rf_driver.waitPacketSent();

  delay(500);
}

```

## License

This project is open-source and released under the [MIT License](LICENSE). You are free to use and modify it for your purposes.

Feel free to contribute, report issues, or suggest improvements by creating a pull request or opening an issue.

---

**Happy RF Communication with ESP32!**
