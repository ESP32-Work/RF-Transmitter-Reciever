#include <RH_ASK.h>
#include <SPI.h>
#include <TimeLib.h>

RH_ASK rf_driver(2000, 21, 22);

const int MAX_MESSAGES = 10;
String messages[MAX_MESSAGES];
int messageIndex = 0;

const double PROPAGATION_SPEED = 3e8; // meters per second
const int NUM_MESSAGES_TO_PROCESS = 5; // Set the number of messages to process before displaying distance

void calculateDistanceTask(void *pvParameters) {
  int messagesProcessed = 0;
  double totalDistance = 0;

  for (;;) {
    if (messagesProcessed == NUM_MESSAGES_TO_PROCESS) {
      // Display the average distance after processing a set number of messages
      double averageDistance = totalDistance / NUM_MESSAGES_TO_PROCESS;
      Serial.print("Average Distance: ");
      Serial.print(averageDistance);
      Serial.println(" inches");

      // Reset variables for the next set of messages
      messagesProcessed = 0;
      totalDistance = 0;
    }

    // Process received messages and calculate distances here
    for (int i = 0; i < NUM_MESSAGES_TO_PROCESS; i++) {
      // Calculate the correct index for the current message
      int currentIndex = (messageIndex - NUM_MESSAGES_TO_PROCESS + MAX_MESSAGES + i) % MAX_MESSAGES;

      String timeStampReceived = messages[currentIndex];

      // Convert timestamp to time_t
      tmElements_t receivedTime;
      sscanf(timeStampReceived.c_str(), "%d-%d-%d %d:%d:%d",
             &receivedTime.Year, &receivedTime.Month, &receivedTime.Day,
             &receivedTime.Hour, &receivedTime.Minute, &receivedTime.Second);

      time_t receivedTimeT = makeTime(receivedTime);

      // Calculate the time difference in seconds (use double)
      double timeDifference = difftime(now(), receivedTimeT);

      // Calculate the distance using the formula: Distance = Propagation Speed * Time Difference
      double distance = PROPAGATION_SPEED * timeDifference;

      // Convert distance to inches
      distance *= 39.3701; // 1 meter = 39.3701 inches

      // Accumulate total distance
      totalDistance += distance;

      // Increment messagesProcessed
      messagesProcessed++;
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay to avoid high CPU load
  }
}

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
      calculateDistanceTask,  // Function that should be called
      "CalculateDistance",    // Name of the task
      10000,                   // Stack size (bytes)
      NULL,                    // Parameter to pass
      1,                       // Task priority
      NULL);                   // Task handle
}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN] = {0};
  uint8_t buflen = sizeof(buf);

  if (rf_driver.recv(buf, &buflen)) {
    // Message received with valid checksum
    Serial.print("Message Received: ");
    Serial.println((char *)buf);

    // Parse the message to extract timestamp
    String timeStampReceived((char *)buf);

    // Save the received message in the array
    messages[messageIndex] = timeStampReceived;
    messageIndex = (messageIndex + 1) % MAX_MESSAGES;
  }
  vTaskDelay(10 / portTICK_PERIOD_MS); // Delay to avoid high CPU load
}
