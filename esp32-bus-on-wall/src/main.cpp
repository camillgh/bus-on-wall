#include <Arduino.h>
#include <WiFi.h>
#include <Time.h>
#include <NTPClient.h>
#include <U8g2lib.h>

#include "wifi_setup.h"
#include "ntp_setup.h"
#include "graphql_functions.h"
#include "display_bus.h"


U8G2_SSD1309_128X64_NONAME2_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
unsigned long lastFetchTime = 0;  // Variable to store the timestamp of the last successful fetch


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

    // Update the timestamp of the last successful fetch
    lastFetchTime = millis();

    // Display bus information on the OLED display
    displayBusInfo(u8g2, timetableInMinutes);
  }

  delay(10000);
}
