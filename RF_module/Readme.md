# RF Communication with ESP32

## Table of Contents
- [Introduction](#introduction)
- [Libraries](#libraries)
- [Setup](#setup)
- [Functionality](#functionality)
- [Usage](#usage)
- [License](#license)

---

## Introduction

This project demonstrates how to establish RF (Radio Frequency) communication between two ESP32 devices using the RadioHead library. It consists of both a transmitter and a receiver module for wireless data transmission.

---

## Libraries

This project utilizes the following libraries:

- **RadioHead Library**: The RadioHead library is used for encoding and decoding data for RF communication. It supports various RF transceivers, and in this project, we use it to send and receive messages wirelessly.

---

## Setup

### Transmitter
- Connect a 433MHz transmitter module to your ESP32.
- Ensure that the transmitter's data pin is connected to the specified pin (RADIOHEAD_TX_PIN) in the code.

### Receiver
- Connect a 433MHz receiver module to your ESP32.
- Ensure that the receiver's data pin is connected to the specified pin (RADIOHEAD_RX_PIN) in the code.

---

## Functionality

### Transmitter
- The transmitter sends a simple message, "hello," in a continuous loop.
- The message is sent using the RadioHead library, and the driver waits until the packet is sent successfully before proceeding.

### Receiver
- The receiver continuously listens for incoming RF transmissions.
- When a valid message is received, it is printed to the serial monitor.

---

## Usage

1. Clone or download this repository to your local computer.
2. Open the Platform io IDE and load the `main.cpp` sketch.
3. Upload the sketch to your transmitter and receiver ESP32s, ensuring the appropriate pin connections.
4. Open the Serial Monitor for the receiver ESP32 to view received messages.
5. The transmitter will continuously send the "hello" message to the receiver, and you will see the messages displayed on the Serial Monitor.

---

## License

This project is open-source and released under the [MIT License](LICENSE). You are free to use and modify it for your purposes.

Feel free to contribute, report issues, or suggest improvements by creating a pull request or opening an issue.

---

**Happy RF Communication with ESP32!**
