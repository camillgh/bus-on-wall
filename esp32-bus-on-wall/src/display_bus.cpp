#include "display_bus.h"


void displayBusInfo(U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI &u8g2, const std::vector<float> &timetableInMinutes, unsigned long elapsedSeconds) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  
  // Display the bus stop
  u8g2.drawStr(0, 10, "St. Olavs plass");

  // Display bus information and time until arrival
  for (size_t i = 0; i < timetableInMinutes.size() && i < 3; ++i) {
    String busInfo = "160 Rykkinn"; // Replace with your actual bus number and route
    String timeStr = String((int)timetableInMinutes[i]) + " min";
    u8g2.drawStr(0, 20 + i * 10, busInfo.c_str());
    u8g2.drawStr(90, 20 + i * 10, timeStr.c_str());
  }

  // Calculate and display the time since the last successful fetch
  String lastFetchStr = "Sist henta for " + String(elapsedSeconds) + "s sia";
  u8g2.drawStr(0, 60, lastFetchStr.c_str());

  u8g2.sendBuffer();
}

void displayBusCountdown(U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI &u8g2){ //, const std::vector<float> &timetableInMinutes, unsigned long elapsedSeconds) {
  u8g2.clearBuffer();

  // Display the main time
  u8g2.setFont(u8g2_font_ncenB24_tr);
  u8g2.drawStr(18, 40, "01:30");

  // Display the second time
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(45, 60, "03:35");

  u8g2.sendBuffer();
}