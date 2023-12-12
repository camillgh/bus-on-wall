#include <Arduino.h>
#include <WiFi.h>
#include <Time.h>
#include <NTPClient.h>
#include "wifi_setup.h"
#include "ntp_setup.h"
#include "graphql_functions.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  initWiFi();
  initNTP();
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
  }

  delay(15000);
}
