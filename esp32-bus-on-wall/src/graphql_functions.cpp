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

String ENTUR_CLIENT_ID = "privat - camillgh/bus-on-wall";
String STOP_PLACE_NUMBER = "4179";  // St. Olavs plass NSR:StopPlace:4179
String BUS_DIRECTION = "outbound";  // towards Rykkinn
String BUS_NUMBER = "160";

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

