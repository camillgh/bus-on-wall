#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFi.h>

extern const char* ssid;
extern const char* password;

void initWiFi();

#endif  // WIFI_SETUP_H
