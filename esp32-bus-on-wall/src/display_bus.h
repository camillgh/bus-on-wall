#ifndef DISPLAY_BUS_H
#define DISPLAY_BUS_H

#include <U8g2lib.h>
#include <vector>

extern unsigned long lastFetchTime;

void displayBusInfo(U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI &u8g2, const std::vector<float> &timetableInMinutes, unsigned long elapsedSeconds);
void displayBusCountdown(U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI &u8g2, const std::vector<float> &timetableInMinutes);
String countdownUntil(String expectedArrivalTime);
void displayBorder(U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI &u8g2);
void displayNormal(U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI &u8g2, const String &mainTime, const String &nextTime);
void displayBlinking(U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI &u8g2, const String &mainTime, unsigned long lastToggleTime, unsigned long toggleInterval);
void centerText(U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI &u8g2, const char* inputString, int inputHeight);

#endif // DISPLAY_BUS_H
