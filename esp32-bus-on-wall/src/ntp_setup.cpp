#include "ntp_setup.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void initNTP() {
  timeClient.begin();
  timeClient.setTimeOffset(3600);
}
