#ifndef WiFiList
#define WiFiList
#include <Arduino.h>

typedef struct WiFiCred {
    String SSID;
    String Password;
}WiFiCred;

WiFiCred knownNetworks[] = {
    {"WeatherNet", "MakeItRain"},
    {"MerleIsMissing", "flamingo"},
    {"MikeNet", "fruitpies"}
};

#endif