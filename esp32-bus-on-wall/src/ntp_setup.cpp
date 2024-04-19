#include "ntp_setup.h"
#include "time.h"

WiFiUDP ntpUDP;
void initNTP() {
  configTime(0, 0, "at.pool.ntp.org"); 

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  setenv("TZ", "CET-1CEST,M3.5.0/02,M10.5.0/03", 1); 
  tzset();

}
