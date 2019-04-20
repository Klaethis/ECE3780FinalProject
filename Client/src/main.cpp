#include <ESP8266HTTPClient.h>
#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include "WiFiList.h"
#include <Arduino.h>
#include <stdio.h>
#include <Wire.h>
#include <stdio.h>
#include <string.h>

int counter;
int findKnownWiFi();
LiquidCrystal lcd(5, 9, 14, 12, 13, 10);

void setup() {
  lcd.begin(16,2);
  lcd.clear();
  lcd.home();
  lcd.print("Init Begin...");

  Serial.begin(115200);
  Serial.println("\r\n\r\nInit Begin...");


  WiFi.mode(WIFI_STA);

  findKnownWiFi();

  counter = 0;
}

String host;
String test;

void loop() {
  host = "http://192.168.4.1/raw";

  test = "";
  WiFiClient client;
  HTTPClient http;

  if (http.begin(client, host)) {
    http.GET();
    test += http.getString();
    http.end();
  }
  else
    test += "Failed!";
  lcd.clear();
  lcd.home();
  char conv[300];
  test.toCharArray(conv, 300);
  char * broken = strtok(conv, ",");
  while (broken != NULL)
  {
    Serial.println(broken);
    lcd.print(broken);
    lcd.setCursor(0,1);
    broken = strtok(NULL, ",");
  }
  delay(1000);
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
          Serial.println("\r\n\nKnown WiFi found!");
          Serial.println("Connecting to " + knownNetworks[j].SSID);
          lcd.clear();
          lcd.home();
          lcd.print("Known WiFi found");
          lcd.setCursor(0,1);
          lcd.print(knownNetworks[j].SSID);
          WiFi.begin(knownNetworks[j].SSID, knownNetworks[j].Password);
          Serial.print("Waiting to connect");
          while (WiFi.status() != WL_CONNECTED)
          {
            Serial.print(".");
            lcd.print(".");
            delay(1000);
          }
          Serial.println(" ");
        }
      }
    }
  }
  return 0;
}