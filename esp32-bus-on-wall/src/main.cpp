
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <Time.h>
#include <math.h>
#include <Timelib.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// Define NTP variables
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

const String ENTUR_GRAPHQL_ENDPOINT = "https://api.entur.io/journey-planner/v2/graphql";

String ENTUR_GRAPHQL_QUERY = R"(
{
  stopPlace(id: "NSR:StopPlace:%(stop_id)s") {
    name
    estimatedCalls(timeRange: 72100, numberOfDepartures: 20) {
      expectedArrivalTime
      realtime
      destinationDisplay {
        frontText
      }
      serviceJourney {
        directionType
        line {
          publicCode
        }
      }
    }
  }
}
)";

String ENTUR_CLIENT_ID = "privat - camillgh/bus-on-wall";
String STOP_PLACE_NUMBER = "4179"; // St. Olavs plass NSR:StopPlace:4179
String BUS_DIRECTION = "outbound"; // towards Rykkinn
String BUS_NUMBER = "160";

unsigned long previousMillis = 0;
const long interval = 15000;  // Interval in milliseconds (1 seconds)

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.print("Starting babyyyyyyyyyyyyy: ");

 // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected to WiFi");

  timeClient.begin();
  timeClient.setTimeOffset(3600);

}




time_t parseTimestamp(String timestamp) {
  struct tm tm;
  memset(&tm, 0, sizeof(tm));
  
  // Parse the timestamp string
  sscanf(timestamp.c_str(), "%d-%d-%dT%d:%d:%d%*s",
         &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
         &tm.tm_hour, &tm.tm_min, &tm.tm_sec);

  // Adjust the year and month values
  tm.tm_year -= 1900;
  tm.tm_mon--;

  // Convert to timestamp
  time_t t = mktime(&tm);
  return t;
}

double convertTimeToMinutes(String parsedTimestamp) {
  time_t targetTime = parseTimestamp(parsedTimestamp);

  timeClient.update();

  // Get current time in seconds since 1970-01-01
  unsigned long currentTime = timeClient.getEpochTime();

  // Calculate the difference in minutes
  int minutesUntil = (targetTime - currentTime) / 60;

  return minutesUntil;
}




// Function to parse relevant bus information
std::vector<float> parseOnlyRelevantBusInfo(JsonArray estimatedCalls) {
  std::vector<float> timetableInMinutes;

  for (const auto &busComing : estimatedCalls) {
    if (busComing["serviceJourney"]["line"]["publicCode"] == BUS_NUMBER &&
        busComing["serviceJourney"]["directionType"] == BUS_DIRECTION) {
      String expectedArrivalTime = busComing["expectedArrivalTime"].as<String>();

      Serial.print("expectedArrivalTime: ");
      Serial.print(expectedArrivalTime);
      Serial.print("\n");

      timetableInMinutes.push_back(convertTimeToMinutes(expectedArrivalTime));
    }
  }

  return timetableInMinutes;
}


String fetchFullTimetable() {
  HTTPClient http;

  // Prepare the query string
  String qry = ENTUR_GRAPHQL_QUERY;
  qry.replace("%(stop_id)s", STOP_PLACE_NUMBER);

  // Make the HTTP request
  http.begin(ENTUR_GRAPHQL_ENDPOINT);
  http.addHeader("Accept", "application/json");
  http.addHeader("ET-Client-Name", "privat - camillgh/bus-on-wall");
  http.addHeader("ET-Client-Id", ENTUR_CLIENT_ID);

  int httpCode = http.POST(qry);

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    

    return payload;
  } else {
    Serial.printf("HTTP request failed, error: %s\n", http.errorToString(httpCode).c_str());
    return "";
  }

  http.end();
}



void loop() {
  unsigned long currentMillis = millis();

  // Check if ten seconds have passed
  if (currentMillis - previousMillis >= interval) {
    // Save the current time
    previousMillis = currentMillis;

    // Perform the fetch operation
    String fullTimetable = fetchFullTimetable();

    Serial.print("--------------------\n");
    // Now you can parse the JSON response using ArduinoJson library if needed
    DynamicJsonDocument doc(12288);
    DeserializationError error = deserializeJson(doc, fullTimetable);
    JsonArray estimatedCalls = doc["data"]["stopPlace"]["estimatedCalls"].as<JsonArray>();

    Serial.print("HTTP OK?: ");
    Serial.println(error.c_str());

    if (error.c_str() == "Ok") {
      
      // Call the function to parse relevant bus information
      std::vector<float> timetableInMinutes = parseOnlyRelevantBusInfo(estimatedCalls);

      Serial.print("Minutter til bussen kommer: ");
      for (const auto &timeInMinutes : timetableInMinutes) {
        Serial.print(timeInMinutes);
        Serial.print(" ");
      }
      Serial.println();
  }
  
  }
}