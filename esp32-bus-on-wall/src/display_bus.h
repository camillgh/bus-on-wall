#ifndef DISPLAY_BUS_H
#define DISPLAY_BUS_H

#include <U8g2lib.h>
#include <vector>

extern unsigned long lastFetchTime;

void displayBusInfo(U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI &u8g2, const std::vector<float> &timetableInMinutes, unsigned long elapsedSeconds);
void displayBusCountdown(U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI &u8g2, const std::vector<float> &timetableInMinutes);
String countdownUntil(String expectedArrivalTime);

#endif // DISPLAY_BUS_H
