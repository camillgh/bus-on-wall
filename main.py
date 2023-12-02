# main.py
from entur_api import fetch_full_timetable
from data_parser import parse_only_relevant_bus_info, convert_list_of_times_to_minutes

def main():
    full_timetable = fetch_full_timetable()
    parsed_timetable = parse_only_relevant_bus_info(full_timetable)
    print(parsed_timetable)

if __name__ == "__main__":
    main()




