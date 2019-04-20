#ifndef Website
#define Website
#include <Arduino.h>

const char* HumanSite = "\
<!DOCTYPE html><html>\
<head>\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
<link rel=\"icon\" href=\"data:,\">\
<style>\
html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\
.button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;\
text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}\
.button2 {background-color: #77878A;}\
</style>\
</head>\
<body>\
<h1 style=\"color: #5e9ca0; text-align: center;\">ECE-3350-001 Final Project</h1>\
<h2 style=\"color: #2e6c80; text-align: center;\">Current Temperature:</h2>\
<p style=\"text-align: center;\">%.2f Fahrenheit</p>\
<h2 style=\"color: #2e6c80; text-align: center;\">Current Humidity:</h2>\
<p style=\"text-align: center;\">%.2f%%</p>\
<h2 style=\"color: #2e6c80; text-align: center;\">Current Pressure:</h2>\
<p style=\"text-align: center;\">%.3f inHg</p>\
<p><a href=\"/\"><button class=\"button\">Refresh</button></a></p>\
</body>\
</html>";

#endif