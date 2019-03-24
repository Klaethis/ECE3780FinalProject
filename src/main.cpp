#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include <Wire.h>
#include <Adafruit_VEML6070.h>
#include <Adafruit_BME280.h>
#include <stdio.h>

int count;
double last;
ESP8266WebServer myServer(80);
Adafruit_VEML6070 uv = Adafruit_VEML6070();
Adafruit_BME280 bme;

void handleRootPath();

void setup() {
  Wire.begin(D2, D1);
  count = 0;
  last = millis();
  Serial.begin(115200);
  
  if (!bme.begin())
    Serial.println("Error finding BME");

  uv.begin(VEML6070_1_T);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP("MikeNet", "fruitpies");
  Serial.println(WiFi.softAPIP());
  
  myServer.on("/", handleRootPath);
  myServer.begin();
  Serial.println("Server initialized, waiting...");
}

void loop() {
  myServer.handleClient();
}

void handleRootPath() {
  char buff[200];
  
  float uvRead = uv.readUV();
  float temp = bme.readTemperature();
  float humid = bme.readHumidity();
  float pres = bme.readPressure();
  
  Serial.print("New connection! #");
  Serial.println(count++);

  sprintf(buff, "Hello World! uv:%f, temp:%f, humid:%f, pres:%f", uvRead, temp, humid, pres);
  
  String outStr = String(buff);

  myServer.send(200, "text/plain", outStr);
}