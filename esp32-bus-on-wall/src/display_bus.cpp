#include "display_bus.h"
#include "graphql_functions.h"


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

void displayBusCountdown(U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI &u8g2, const std::vector<float> &timetableInMinutes) {
  u8g2.clearBuffer();

  static unsigned long lastToggleTime = 0;
  const unsigned long toggleInterval = 1000;
  
  String mainTime = String((int)timetableInMinutes[0]) + " min";
  String nextTime = String((int)timetableInMinutes[1]) + " min";

  // Alternative font that the user also liked:
  // u8g2_font_lubR24_tf (den tynne)

  // Check if main time is 1 minute or less
  if (timetableInMinutes[0] == 0) {
    // displayBlinking(u8g2, mainTime, lastToggleTime, toggleInterval);
    if (millis() - lastToggleTime >= toggleInterval) {
      lastToggleTime = millis();
      u8g2.setDrawColor(1 - u8g2.getDrawColor()); // Toggle draw color between 0 (black) and 1 (white)
    }

    displayBorder(u8g2);

    u8g2.setFont(u8g2_font_ncenB24_tr);
    u8g2.drawStr(18, 40, mainTime.c_str());

  } else {
    displayNormal(u8g2, mainTime, nextTime);
  }

  u8g2.sendBuffer();
}

void displayBorder(U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI &u8g2) {
    u8g2.drawFrame(0, 0, 126, 64);
    u8g2.drawFrame(1, 1, 124, 62);
    u8g2.drawFrame(2, 2, 122, 60);
    u8g2.drawFrame(3, 3, 120, 58);
    u8g2.drawFrame(4, 4, 118, 56);
}

void displayBlinking(U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI &u8g2, const String &mainTime, unsigned long lastToggleTime, unsigned long toggleInterval) {
    if (millis() - lastToggleTime >= toggleInterval) {
      lastToggleTime = millis();
      u8g2.setDrawColor(1 - u8g2.getDrawColor()); // Toggle draw color between 0 (black) and 1 (white)
    }

    displayBorder(u8g2);

    u8g2.setFont(u8g2_font_ncenB24_tr);
    centerText(u8g2, mainTime.c_str(), 40);
}

void displayNormal(U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI &u8g2, const String &mainTime, const String &nextTime){

    u8g2.setFont(u8g2_font_ncenB08_tr);
    centerText(u8g2, "160 Rykkinn", 10);

    u8g2.setFont(u8g2_font_ncenB24_tr);
    centerText(u8g2, mainTime.c_str(), 40);

    u8g2.setFont(u8g2_font_ncenB08_tr);
    centerText(u8g2, nextTime.c_str(), 60);
}

void centerText(U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI &u8g2, const char* inputString, int inputHeight){
    const int screenWidth = 128;

    int busStopNameWidth = u8g2.getStrWidth(inputString);
    int busStopNameX = (screenWidth - busStopNameWidth) / 2; 
    u8g2.drawStr(busStopNameX, inputHeight, inputString);
}

String countdownUntil(String expectedArrivalTime) {
  time_t targetTime = parseTimestamp(expectedArrivalTime);

  // Get current time in seconds since 1970-01-01
  unsigned long currentTime = getCurrentEpochTime();

  // Calculate the difference in seconds
  long secondsUntil = targetTime - currentTime;

  // Convert seconds to minutes and seconds
  long minutes = secondsUntil / 60;
  long seconds = secondsUntil % 60;

  // Format the countdown as "minutes:seconds"
  String countdown = String(minutes) + ":" + ((seconds < 10) ? "0" : "") + String(seconds);

  return countdown;
}