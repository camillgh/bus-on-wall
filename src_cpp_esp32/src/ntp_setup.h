#ifndef NTP_SETUP_H
#define NTP_SETUP_H

#include <Time.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

extern WiFiUDP ntpUDP;

void initNTP();

#endif  // NTP_SETUP_H
