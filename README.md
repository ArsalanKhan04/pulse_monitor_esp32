# Project Overview — sc_proj (ESP32 Sensor Collection)

This repository contains an ESP32/Arduino-based application that reads heart-rate and SpO2 from a MAX3010x optical sensor, displays status on an SSD1306 OLED, reads GPS coordinates from a serial GPS module, and periodically posts JSON sensor data to a remote HTTP server. The firmware is structured using FreeRTOS tasks: separate tasks collect pulse data, read GPS, render the OLED display, and send data over Wi‑Fi.

This README documents how the pieces fit together, where to find component-specific documentation (in components/*), and how to run/debug the firmware on an ESP32 board.

High-level flow

1. Device boots and runs setup() in main/main.cpp.
2. Wi‑Fi is connected (components/senddata) and the OLED, pulse sensor and GPS subsystems are initialized.
3. Four FreeRTOS tasks are created:
   - sendToServer (components/senddata) — packages bpm/SpO2/GPS into JSON and POSTs to server periodically.
   - get_pulse (components/pulse) — reads MAX3010x FIFO, computes heart rate and SpO2 and updates shared pulse_vars.
   - displaySlideshow (components/oled) — updates the SSD1306 with current bpm and SpO2 values.
   - get_gps_data (components/gps) — reads NMEA from UART, decodes with TinyGPS++ and updates shared gps_vars.

Important files and directories

- main/main.cpp — program entry point; creates tasks and contains the main loop that prints basic telemetry to serial.
- components/senddata/ — Wi‑Fi connection, HTTP client and the sendToServer task. (See components/senddata/README.md)
- components/pulse/ — MAX30105 driver usage, signal buffering, and heart rate/SpO2 calculation (uses the included MAX30105 library). (See components/pulse/README.md)
- components/oled/ — SSD1306 display code and slideshow / single-screen render tasks. (See components/oled/README.md)
- components/gps/ — TinyGPS++ integration and UART configuration. (See components/gps/README.md)
- components/max30105/ — upstream MAX30105 library used for sensor access; contains example/algorithm code. (See components/max30105/COMPONENT_DOC.md)

How the FreeRTOS tasks share data

- pulse_vars (components/pulse/pulse.h): holds beatAvg, SPO2, and irValue. get_pulse writes to this struct and oled/display task & sendToServer read its values indirectly (main copies beatAvg and SPO2 into oled_data).
- gps_vars (components/gps/gps.h): holds lat/lon updated by get_gps_data and read by sendToServer via display_vars pointers.
- display_vars (components/senddata/senddata.h): a small helper used to pass pointers into sendToServer so it can read live bpm, lon, lat and oxy.

Building and flashing

Use the ESP-IDF or Arduino build commands appropriate to this project setup. Typical flow (ESP-IDF):

1. idf.py set-target <chip>
2. idf.py menuconfig (adjust serial port / Wi‑Fi settings if exposed)
3. idf.py build
4. idf.py -p /dev/ttyUSB0 flash monitor

Note: the code currently contains Wi‑Fi SSID/password and a public server URL in components/senddata/senddata.cpp for testing; remove or secure these before production use.

Troubleshooting & tips

- If OLED doesn't initialize, confirm I2C pins and address (0x3C). The OLED code uses Wire and Adafruit_SSD1306.
- If MAX3010x is not detected, check I2C (uses TwoWire(1) on pins 18/19) and sensor power/wiring.
- GPS uses UART2 with RXD2=16, TXD2=17 at 9600 baud; confirm GPS module wiring and baud.
- Network issues: senddata sets a non-SSL HTTP transport; ensure the serverUrl is reachable and remove hardcoded credentials.

Where to find more docs

- component-level READMEs: components/*/README.md (created by this commit) for per-component usage and pin mappings.
- max30105 library README is in components/max30105/README.md and a local COMPONENT_DOC.md provides integration notes.

Contact / Next steps

If more detail is needed (e.g., sequence diagrams, shared-memory synchronization, or converting HTTP to HTTPS), open an issue describing the specific area to expand.
