Components overview

This directory contains modular components used by the firmware. Each component implements a single subsystem and exposes a small API via its header.

- senddata: Wi‑Fi and HTTP client (connectToWiFi, sendToServer)
- pulse: MAX30105 sensor reading and heart-rate/SpO2 processing (pulse_setup, get_pulse)
- oled: SSD1306 display rendering and slideshow (oled_setup, displaySlideshow)
- gps: TinyGPS++ based GPS reader (gps_setup, get_gps_data)
- max30105: upstream sensor library; used by pulse component

Read the README.md in each component folder for pinouts, initialization sequence and usage notes.