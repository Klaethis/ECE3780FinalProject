#ifndef WiFiList
#define WiFiList
#include <Arduino.h>

typedef struct WiFiCred {
    String SSID;
    String Password;
}WiFiCred;

WiFiCred knownNetworks[] = {
    {"MerleIsMissing", "flamingo"},
    {"MikeNet", "fruitpies"}
};

WiFiCred APCred = {"WeatherNet", "MakeItRain"};

#endif