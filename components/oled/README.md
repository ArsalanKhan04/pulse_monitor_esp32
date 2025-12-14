oled component

Purpose

Controls the Adafruit SSD1306 OLED display to present BPM and SpO2 status. Offers two display modes: a full single-screen layout and a slideshow that alternates BPM and SpO2 screens.

API (oled.h)

- void oled_setup() — initializes the SSD1306 display (I2C address 0x3C).
- void oled_display(void *pvParameters) — example display loop (not the primary slideshow used at runtime).
- void displayTask(void *pvParameters) — single-screen rendering of BPM/O2 with status icons.
- void displaySlideshow(void *pvParameters) — alternates two screens (BPM and SpO2) every 2s.

Data

- struct oled_data contains bpm, oxy, longi, lati. The display tasks expect a pointer to this struct.

Notes

- The code uses Adafruit_SSD1306 and Adafruit_GFX. Ensure the correct I2C pins and that Wire is initialized by the platform.
- displaySlideshow is started in main and reads oled_data regularly; ensure oled.bpm and oled.oxy are updated by sensor tasks or by main.

Visual cues

- Icons (write(3), write(1), etc.) are used to represent heart, sad/normal faces, etc.; consult the font/bitmap definitions if you need custom icons.