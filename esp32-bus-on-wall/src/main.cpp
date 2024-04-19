#include <Arduino.h>
#include <WiFi.h>
#include <Time.h>
#include <U8g2lib.h>

#include "wifi_setup.h"
#include "ntp_setup.h"
#include "graphql_functions.h"
#include "display_bus.h"


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SCK_CLK    18
#define OLED_SDA_MOSI   23
#define OLED_RESET      17
#define OLED_DC_MISO    16
#define OLED_CS          5

U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI u8g2(U8G2_R0, OLED_CS, OLED_DC_MISO, OLED_RESET);

unsigned long lastFetchTime = 0;


void setup() {
  Serial.begin(115200);
  delay(1000);

  initWiFi();
  initNTP();

  u8g2.begin();

}


void loop() {

  String fullTimetable = fetchFullTimetable();

  // Now you can parse the JSON response using ArduinoJson library if needed
  DynamicJsonDocument doc(12288);
  DeserializationError error = deserializeJson(doc, fullTimetable);
  JsonArray estimatedCalls = doc["data"]["stopPlace"]["estimatedCalls"].as<JsonArray>();

  if (error.c_str() == "Ok") {
     std::vector<float> timetableInMinutes = parseOnlyRelevantBusInfo(estimatedCalls);

     Serial.print("Minutes until the bus comes: ");
     for (const auto &timeInMinutes : timetableInMinutes) {
       Serial.print(timeInMinutes);
       Serial.print(" ");
     }
      Serial.println();

      
      // Calculate the time difference since the last successful fetch
      unsigned long currentTime = millis();
      unsigned long elapsedTime = currentTime - lastFetchTime;
      unsigned long elapsedSeconds = elapsedTime / 1000; // Convert milliseconds to seconds

      // Update the timestamp of the last successful fetch
      lastFetchTime = currentTime;

      Serial.print("Last successful fetch: ");
      Serial.print(elapsedSeconds);
      Serial.println(" s ago");

    // Display bus information on the OLED display
    // displayBusInfo(u8g2, timetableInMinutes, elapsedSeconds);
  }
  delay(10000);

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,20,"whats updog");
  } while ( u8g2.nextPage() );
  delay(1000);


  
}