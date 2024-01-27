#ifndef DISPLAY_BUS_H
#define DISPLAY_BUS_H

#include <U8g2lib.h>
#include <vector>

extern unsigned long lastFetchTime;

void displayBusInfo(U8G2_SSD1309_128X64_NONAME2_1_HW_I2C &u8g2, const std::vector<float> &timetableInMinutes, unsigned long elapsedSeconds);

#endif // DISPLAY_BUS_H
