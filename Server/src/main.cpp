#include <Arduino.h>
#include <Wire.h>
#include <stdio.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Adafruit_BME280.h>
#include <Adafruit_VEML6070.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include "WiFiList.h"

int count;
double last;
ESP8266WebServer myServer(80);
Adafruit_VEML6070 uv = Adafruit_VEML6070();
Adafruit_BME280 bme;

int initOTAService();
int findKnownWiFi();

void handleRootPath();
void handleTest();

void setup() {
  count = 0;
  last = millis();
  Serial.begin(115200);
  
  if (!bme.begin())
    Serial.println("Error finding BME");

  uv.begin(VEML6070_1_T);
  
  WiFi.mode(WIFI_AP_STA);
  
  if (initOTAService()) {
    Serial.print("\r\nOTA init complete! IP address: ");
    Serial.println(WiFi.localIP());
  }

  WiFi.softAP(APCred.SSID, APCred.Password);
  Serial.print("\r\n\nAP SSID: " + APCred.SSID + " AP IP: ");
  Serial.println(WiFi.softAPIP());

  myServer.on("/", handleRootPath);
  myServer.on("/PublicIP", handleTest);
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

  sprintf(buff, "uv:%f,temp:%f,humid:%f,pres:%f", uvRead, temp, humid, pres);
  
  String outStr = String(buff);

  myServer.send(200, "text/plain", outStr);
}

void handleTest()
{
  String host = "http://api.ipify.org/?format=text";

  String test = "";
  WiFiClient client;
  HTTPClient http;

  if (http.begin(client, host)) {
    http.GET();
    test += http.getString();
    http.end();
  }
  else
    test += "Failed!";

  myServer.send(200, "text/plain", test);
}

int initOTAService() {
  findKnownWiFi();

  if (WiFi.status() != WL_CONNECTED)
    return -1;

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
  return 1;
}

int findKnownWiFi() {
  int netTotal = WiFi.scanNetworks();

  if (netTotal == 0) {
    Serial.println("\r\n\nNo networks found");
    return -1;
  }
  else {
    for (int i = 0; i < netTotal && WiFi.status() != WL_CONNECTED; i++)
    {
      for (unsigned int j = 0; j < sizeof(knownNetworks) && WiFi.status() != WL_CONNECTED; j++)
      {
        if (WiFi.SSID(i) == knownNetworks[j].SSID){
          Serial.println("\r\n\nKnown network found!");
          Serial.println("Connecting to " + knownNetworks[j].SSID);
          WiFi.begin(knownNetworks[j].SSID, knownNetworks[j].Password);
          Serial.print("Waiting to connect");
          while (WiFi.status() != WL_CONNECTED)
          {
            Serial.print(".");
            delay(500);
          }
          Serial.println(" ");
        }
      }
    }
  }
  return 0;
}