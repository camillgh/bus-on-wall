#ifndef GRAPHQL_FUNCTIONS_H
#define GRAPHQL_FUNCTIONS_H

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <vector>

extern const String ENTUR_GRAPHQL_ENDPOINT;
extern String ENTUR_GRAPHQL_QUERY;
extern String ENTUR_CLIENT_ID;
extern String STOP_PLACE_NUMBER;
extern String BUS_DIRECTION;
extern String BUS_NUMBER;

String fetchFullTimetable();
time_t parseTimestamp(String timestamp);
double convertTimeToMinutes(String parsedTimestamp);
std::vector<float> parseOnlyRelevantBusInfo(JsonArray estimatedCalls);

#endif  // GRAPHQL_FUNCTIONS_H








// #ifndef GRAPHQL_FUNCTIONS_H
// #define GRAPHQL_FUNCTIONS_H

// #include <HTTPClient.h>
// #include <ArduinoJson.h>

// const String ENTUR_GRAPHQL_ENDPOINT = "https://api.entur.io/journey-planner/v2/graphql";
// String ENTUR_GRAPHQL_QUERY = R"(
// {
//   stopPlace(id: "NSR:StopPlace:%(stop_id)s") {
//     name
//     estimatedCalls(timeRange: 72100, numberOfDepartures: 20) {
//       expectedArrivalTime
//       realtime
//       destinationDisplay {
//         frontText
//       }
//       serviceJourney {
//         directionType
//         line {
//           publicCode
//         }
//       }
//     }
//   }
// }
// )";

// String ENTUR_CLIENT_ID = "privat - camillgh/bus-on-wall";
// String STOP_PLACE_NUMBER = "4179";  // St. Olavs plass NSR:StopPlace:4179
// String BUS_DIRECTION = "outbound";  // towards Rykkinn
// String BUS_NUMBER = "160";

// String fetchFullTimetable();
// time_t parseTimestamp(String timestamp);
// double convertTimeToMinutes(String parsedTimestamp);
// std::vector<float> parseOnlyRelevantBusInfo(JsonArray estimatedCalls);

// #endif  // GRAPHQL_FUNCTIONS_H
