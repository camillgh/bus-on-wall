# Bus-on-Wall 🚌🚀

This code is running on an ESP32 that is connected to an OLED display, which will print the timetable for the bus stop St. Olavs Plass, and parse the timetable for only bus 160 going towards Rykkinn. Made for my cute baby sharktank.

## Configuration 🛠️

This is configurable to any stop place and any buses you desire. Change the `BUS_NUMBER` and `BUS_DIRECTION` to whatever you want to have a timetable for. I have generally found that _"outbound"_ are buses going towards the west, but that can be different for different routes.

Update the `STOP_PLACE_NUMBER` by visiting [Entur Stop Place](https://stoppested.entur.org/) and finding the ID of the bus stop you want a timetable for. It can be found inside the code: "NSR:StopPlace:**4179**"

```python
# config.py
STOP_PLACE_NUMBER = "your-stop-place-number"
BUS_DIRECTION = "inbound" #or "outbound"
BUS_NUMBER = "your-bus-number"
```
