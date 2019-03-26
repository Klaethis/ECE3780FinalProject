#include <Arduino.h>

int counter;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  counter = 0;
}

void loop() {
  Serial.printf("Tick! #%d \r\n", counter++);
  delay(1000);
}