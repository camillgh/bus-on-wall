#include "display_bus.h"


void displayBusInfo(U8G2_SSD1309_128X64_NONAME2_1_HW_I2C &u8g2, const std::vector<float> &timetableInMinutes) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  
  // Display the bus stop
  u8g2.drawStr(0, 10, "St. Olavs plass");

  // Display bus information and time until arrival
  for (size_t i = 0; i < timetableInMinutes.size() && i < 3; ++i) {
    String busInfo = "160 Rykkinn"; // Replace with your actual bus number and route
    String timeStr = String(timetableInMinutes[i]) + " min";
    u8g2.drawStr(0, 20 + i * 10, busInfo.c_str());
    u8g2.drawStr(100, 20 + i * 10, timeStr.c_str());
  }

  // Calculate and display the time since the last successful fetch
  unsigned long currentTime = millis();
  unsigned long secondsSinceLastFetch = (currentTime - lastFetchTime) / 1000;
  String lastFetchStr = "Sist hentet for " + String(secondsSinceLastFetch) + " sekunder siden";
  u8g2.drawStr(0, 60, lastFetchStr.c_str());

  u8g2.sendBuffer();
}