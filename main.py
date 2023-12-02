import requests
import json
import time
from datetime import datetime

ENTUR_CLIENT_ID = "privat - camillgh/bus-on-wall"
STOP_PLACE_NUMBER = "4179" #St. Olavs plass NSR:StopPlace:4179
BUS_DIRECTION = "outbound" #outbound = mot Rykkinn

ENTUR_GRAPHQL_ENDPOINT = "https://api.entur.io/journey-planner/v2/graphql"
ENTUR_GRAPHQL_QUERY = """
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
"""

def main():
  full_timetable = fetch_full_timetable()

  parsed_timetable = parse_only_relevant_bus_info(full_timetable)

  print(parsed_timetable)

def fetch_full_timetable():

  headers = {
      "Accept": "application/json",
      "ET-Client-Name": "privat - camillgh/bus-on-wall",
      "ET-Client-Id": ENTUR_CLIENT_ID,
  }
  qry = ENTUR_GRAPHQL_QUERY % dict(stop_id=STOP_PLACE_NUMBER)
  res = requests.post(
      ENTUR_GRAPHQL_ENDPOINT,
      headers=headers,
      timeout=5,
      json=dict(query=qry, variables={}),
  )
  res.raise_for_status()

  full_timetable = res.json()["data"]["stopPlace"]["estimatedCalls"]

  return full_timetable


def parse_only_relevant_bus_info(full_timetable):

  parsed_timetable = []
  for bus_coming in full_timetable:
    if bus_coming["serviceJourney"]["line"]["publicCode"] == "160" and bus_coming["serviceJourney"]["directionType"] == BUS_DIRECTION:
      parsed_timetable.append(bus_coming["expectedArrivalTime"])

  timetable_in_minutes = convert_list_of_times_to_minutes(parsed_timetable)

  return timetable_in_minutes




def convert_list_of_times_to_minutes(parsed_timetable):
  # Convert timestamps to datetime objects
  datetime_objects = [datetime.strptime(ts, '%Y-%m-%dT%H:%M:%S%z') for ts in parsed_timetable]

  # Get the current time in the same timezone as the timestamp
  current_time = datetime.now(datetime_objects[0].tzinfo)

  # Calculate the time difference in minutes and round to one decimal place
  time_difference_in_minutes = [round((dt - current_time).total_seconds() / 60, 1) for dt in datetime_objects]
  return time_difference_in_minutes


main()