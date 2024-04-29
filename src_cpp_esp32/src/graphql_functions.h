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
extern int MINUTES_TO_BUS_STOP;

String fetchFullTimetable();
time_t parseTimestamp(String timestamp);
double convertTimeToMinutes(String parsedTimestamp);
std::vector<float> parseOnlyRelevantBusInfo(JsonArray estimatedCalls);
unsigned long getCurrentEpochTime();

#endif  // GRAPHQL_FUNCTIONS_H

