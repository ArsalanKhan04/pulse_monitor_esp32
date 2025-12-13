gps component

Purpose

Reads NMEA sentences from a GPS module over UART (HardwareSerial 2), decodes them using TinyGPS++ and updates a gps_vars struct with latitude and longitude.

API (gps.h)

- void gps_setup() — opens Serial2 at GPS_BAUD (9600) on RXD2=16, TXD2=17.
- void get_gps_data(void *pvParameters) — FreeRTOS task entry; reads available UART bytes, feeds TinyGPS++ and updates gps_vars when a valid location is available.

Notes

- Ensure GPS module TX is wired to RXD2 (pin 16) and module is powered and outputting NMEA at 9600 baud.
- TinyGPS++ stores parsed satellite/time/location fields; this project only uses location.lat() and location.lng().
- The task delays 3 seconds between reads to avoid busy-waiting.