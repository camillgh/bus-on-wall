# entur_api.py
import requests


STOP_PLACE_NUMBER = "4179"

ENTUR_CLIENT_ID = "privat - camillgh/bus-on-wall"
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
