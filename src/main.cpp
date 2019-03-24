#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include <Wire.h>
#include <Adafruit_VEML6070.h>
#include <Adafruit_BME280.h>
#include <stdio.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>

int count;
double last;
ESP8266WebServer myServer(80);
Adafruit_VEML6070 uv = Adafruit_VEML6070();
Adafruit_BME280 bme;

void initOTAService();
void handleRootPath();

void setup() {
  count = 0;
  last = millis();
  Serial.begin(115200);
  
  if (!bme.begin())
    Serial.println("Error finding BME");

  uv.begin(VEML6070_1_T);
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("MikeNet", "fruitpies");
  Serial.print("\n\nAPIP: ");
  Serial.println(WiFi.softAPIP());

  initOTAService();
  
  myServer.on("/", handleRootPath);
  myServer.begin();
  Serial.println("Server initialized, waiting...");
}

void loop() {
  myServer.handleClient();
  ArduinoOTA.handle();
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

void initOTAService() {
  WiFi.begin("MerleIsMissing", "flamingo");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Waiting for OTA WiFi...");
    delay(500);
  }

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH){
      type = "sketch";
    } else {
      type = "filestream";
    }
    Serial.println("OTA! Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA END!");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    switch (error)
    {
      case OTA_AUTH_ERROR:
        Serial.println("Auth Failed");
        break;
      case OTA_BEGIN_ERROR:
        Serial.println("Begin Failed");
        break;
      case OTA_CONNECT_ERROR:
        Serial.println("Connect Failed");
        break;
      case OTA_RECEIVE_ERROR:
        Serial.println("Recieve Failed");
        break;
      case OTA_END_ERROR:
        Serial.println("End Failed");
        break;

      default:
        Serial.println("Unknown error!");
        break;
    }
  });
  
  ArduinoOTA.begin();
  Serial.print("OTA init complete! IP address: ");
  Serial.println(WiFi.localIP());
}