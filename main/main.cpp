#include <SPI.h>
#include <Arduino.h>
#include "oled.h"
#include <stdio.h>
#include <esp_log.h>
#include "pulse.h"
#include <WiFi.h>
#include <TinyGPS++.h>
#include "esp_http_client.h"
#include "senddata.h"
#include "gps.h"


// Output variables
pulse_vars pulse;
gps_vars gps_v;
oled_data oled;
float lon = 72.987799, lat = 33.644099;
display_vars displayvars(pulse.beatAvg, gps_v.lon, gps_v.lat, pulse.SPO2);

void setup()
{
  printf("Starting ESP32 Application\n");
  connectToWiFi();

  oled_setup();
  pulse_setup();
  gps_setup();

  pulse.irValue = 0;
  pulse.beatAvg = 0;
  pulse.beatsPerMinute = 0;

  xTaskCreate(
      sendToServer,
      "Send Data Task",
      4096,
      &displayvars,
      1,
      NULL
    );
  xTaskCreate(
      get_pulse,
      "Pulse Sensor Task",
      8192,
      &pulse,
      1,
      NULL
    );
  xTaskCreate(
      displaySlideshow,
      "Oled Display",
      2048,
      &oled,
      1,
      NULL
    );
  xTaskCreate(
      get_gps_data,
      "GPS Task",
      2048,
      &gps_v,
      1,
      NULL
    );
}

void loop() {
  printf("IR=");
  printf("%ld", pulse.irValue);
  printf(", Avg BPM=");
  printf("%d", pulse.beatAvg);
  oled.bpm = pulse.beatAvg;
  oled.oxy = pulse.SPO2;

  if (pulse.irValue < 50000)
    printf(" No finger?");

  printf("\n");
  delay(2000);
  // This sketch displays information every time a new sentence is correctly encoded.
}
