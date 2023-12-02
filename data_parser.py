from config import BUS_DIRECTION
from datetime import datetime

def parse_only_relevant_bus_info(full_timetable):
    parsed_timetable = []
    for bus_coming in full_timetable:
        if (
            bus_coming["serviceJourney"]["line"]["publicCode"] == "160"
            and bus_coming["serviceJourney"]["directionType"] == BUS_DIRECTION
        ):
            parsed_timetable.append(bus_coming["expectedArrivalTime"])
    timetable_in_minutes = convert_list_of_times_to_minutes(parsed_timetable)
    return timetable_in_minutes

def convert_list_of_times_to_minutes(parsed_timetable):
    # Convert timestamps to datetime objects
    datetime_objects = [datetime.strptime(ts, "%Y-%m-%dT%H:%M:%S%z") for ts in parsed_timetable]

    # Get the current time in the same timezone as the timestamp
    current_time = datetime.now(datetime_objects[0].tzinfo)

    # Calculate the time difference in minutes and round to one decimal place
    time_difference_in_minutes = [
        round((dt - current_time).total_seconds() / 60, 1) for dt in datetime_objects
    ]
    return time_difference_in_minutes
