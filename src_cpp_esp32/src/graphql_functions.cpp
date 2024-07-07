#include "graphql_functions.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "ntp_setup.h"

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

String ENTUR_CLIENT_ID = "privat-camillghbusonwall";
String STOP_PLACE_NUMBER = "4076";  // St. Olavs plass NSR:StopPlace:4179 // Nationaltheateret, buss NSR:StopPlace:4076
String BUS_DIRECTION = "outbound";  // towards Rykkinn and Fornebu: outbound
String BUS_NUMBER = "81";
int MINUTES_TO_BUS_STOP = 10; //3 to St. Olavs plass

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
  tm.tm_hour--; //DST Daylight Summer Time 

  // Convert to timestamp
  time_t t = mktime(&tm);
  return t;
}

unsigned long getCurrentEpochTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return 0; // Return 0 or any other suitable value to indicate failure
  }

  time_t epochTime = mktime(&timeinfo);

  return (unsigned long)epochTime;
}

double convertTimeToMinutes(String parsedTimestamp) {
  time_t targetTime = parseTimestamp(parsedTimestamp);

  // Get current time in seconds since 1970-01-01
  unsigned long currentTime = getCurrentEpochTime();

  // Add walking time to bus stop minutes to the current time
  currentTime += MINUTES_TO_BUS_STOP * 60;

  // Calculate the difference in minutes
  int minutesUntil = (targetTime - currentTime) / 60;

  return minutesUntil;
}




std::vector<float> parseOnlyRelevantBusInfo(JsonArray estimatedCalls) {
  std::vector<float> timetableInMinutes(3, -1.00); // Initialize with -1.00

  size_t index = 0;
  for (const auto &busComing : estimatedCalls) {
      if (busComing["serviceJourney"]["line"]["publicCode"] == BUS_NUMBER &&
          busComing["serviceJourney"]["directionType"] == BUS_DIRECTION) {
          String expectedArrivalTime = busComing["expectedArrivalTime"].as<String>();
          float timeInMinutes = convertTimeToMinutes(expectedArrivalTime);

          if (timeInMinutes >= 0 && timeInMinutes < 1000) {
              timetableInMinutes[index] = timeInMinutes;
              if (++index >= 3) {
                  break;
              }
          }
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
  http.addHeader("ET-Client-Name", ENTUR_CLIENT_ID);  
  http.addHeader("Content-Type", "application/json"); 

  // Create JSON object for the request body
  DynamicJsonDocument requestBody(512);
  requestBody["query"] = qry;

  // Serialize JSON object to string
  String requestBodyString;
  serializeJson(requestBody, requestBodyString);

  // Make the POST request with the JSON body
  int httpCode = http.POST(requestBodyString);

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    

    return payload;
  } else {
    Serial.printf("HTTP request failed, error: %s\n", http.errorToString(httpCode));
    return "";
  }

  http.end();
}

