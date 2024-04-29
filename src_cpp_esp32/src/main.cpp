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

#define BUTTON_TURN_ON_SYSTEM 4

#define TIME_FIFTEEN_MINUTES 15 * 60 * 1000

U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI u8g2(U8G2_R0, OLED_CS, OLED_DC_MISO, OLED_RESET);

unsigned long lastFetchTime = 0;
std::vector<float> timetableInMinutes(3, 0.00);

unsigned long timeSinceWakeUp = 0;
bool systemActive = false; 

void setup() {
  Serial.begin(115200);
  delay(1000);


  pinMode(BUTTON_TURN_ON_SYSTEM, OUTPUT);
  digitalWrite(BUTTON_TURN_ON_SYSTEM, HIGH);
  }

void initializeAfterWakeUp(){
  
  u8g2.begin();

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0,20,"Hello there!");
    delay(1000);
  } while ( u8g2.nextPage() );


  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0,20,"Connecting to Wifi...");
    delay(1000);
  } while ( u8g2.nextPage() );

  initWiFi();
  initNTP();


  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0,20,"Connected to Wifi!");
    u8g2.drawStr(0,40,"LETS GO!!");
    delay(1000);
  } while ( u8g2.nextPage() );

}


std::vector<float> updateBusInfo(std::vector<float> existingTimetableInMinutes) {
    std::vector<float> newTimetableInMinutes;

    // Calculate the time difference since the last successful fetch
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - lastFetchTime;
    unsigned long elapsedSeconds = elapsedTime / 1000; // Convert milliseconds to seconds


    // Check if it's time to update bus info (every 10 seconds)
    if (elapsedSeconds >= 10 || lastFetchTime == 0) {
        // Update bus info
        String fullTimetable = fetchFullTimetable();
        DynamicJsonDocument doc(12288);
        DeserializationError error = deserializeJson(doc, fullTimetable);
        JsonArray estimatedCalls = doc["data"]["stopPlace"]["estimatedCalls"].as<JsonArray>();

      if (error.c_str() == "Ok") {
         // Update the timestamp of the last successful fetch
          lastFetchTime = currentTime;
          newTimetableInMinutes = parseOnlyRelevantBusInfo(estimatedCalls);

        // Display full bus information
        // displayBusInfo(u8g2, newTimetableInMinutes, elapsedSeconds);

        return newTimetableInMinutes;
      }
      return existingTimetableInMinutes;
    }
    return existingTimetableInMinutes;
}


void loop() {
  if (digitalRead(BUTTON_TURN_ON_SYSTEM) == LOW) { // Activate the system - SETUP
    Serial.print("Button pressed!");
    systemActive = true; 
    timeSinceWakeUp = millis();
    initializeAfterWakeUp();
  }

  int wakeUpDuration = millis() - timeSinceWakeUp;

  if (systemActive && (wakeUpDuration < TIME_FIFTEEN_MINUTES)) { // System active - loop in timespan
      
      timetableInMinutes = updateBusInfo(timetableInMinutes);
      displayBusCountdown(u8g2, timetableInMinutes);
    
    } else { // Deactivate system
      Serial.print("Deactivate system");

      systemActive = false; 
      u8g2.clearBuffer(); 
      u8g2.sendBuffer(); 
      esp_deep_sleep(0);
    }  

}

