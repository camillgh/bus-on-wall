# Personalized bus time table 🚌🚀

![Personalized bus time table](/assets/demo.jpg)

The code from the **src_cpp_esp32** is running on an ESP32 that is connected to an 128x64 2.42 inch SPI OLED display, which will print the timetable for the bus stop St. Olavs Plass, and parse the timetable for only bus 160 going towards Rykkinn.

## Configuration 🛠️

This is configurable to any stop place and any buses you desire. Change the `BUS_NUMBER` and `BUS_DIRECTION` to whatever you want to have a timetable for. I have generally found that _"outbound"_ are buses going towards the west, but that can be different for different routes.

Update the `STOP_PLACE_NUMBER` by visiting [Entur Stop Place](https://stoppested.entur.org/) and finding the ID of the bus stop you want a timetable for. It can be found inside the code: "NSR:StopPlace:**4179**"

```python
# config.py
STOP_PLACE_NUMBER = "your-stop-place-number"
BUS_DIRECTION = "inbound" #or "outbound"
BUS_NUMBER = "your-bus-number"
```

## Details 📝

The API-fetch is done every 10 seconds, and the times displayed are the times fetched from ENTUR with the time it takes to walk to the bus stop, `MINUTES_TO_BUS_STOP`, subtracted from it. When it's 0 minutes remaining until the next bus comes, the display will blink periodically.

The ESP32 is in a deep sleep mode until the button is pressed, and then it will be on for 15 minutes.

The 3D-files were made based on [this case from Thingiverse](https://www.thingiverse.com/thing:5943234), and [this is the delicious button that was used](https://www.kjell.com/no/produkter/elektro-og-verktoy/el-produkter/svakstrom/strombrytere-for-svakstrom/trykkstrombryter-fra-til-rod-belysning-p36064).

The first version of the backend was first made in Python, in the **src_python** folder, then it was developed in C++ to run on the ESP32.

## Electric connections⚡

| OLED pins | ESP32 Pin |
| --------- | --------- |
| VCC       | V5        |
| SCK       | G18       |
| SDA       | G23       |
| RES       | G17       |
| DC        | G16       |
| CS        | G5        |

And the button was on pin G4 of the ESP32.

I bought this dev kit pretty cheap, so it did not match perfectly, but I based the connections of off [this data sheet](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf).

![ESP32 dev kit used](/assets/esp32.png)

_ESP32 dev kit used_
