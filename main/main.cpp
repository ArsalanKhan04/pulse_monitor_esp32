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


pulse_vars pulse;
oled_data oled;
float lon = 60, lat = 70;
int oxy = 50;
display_vars displayvars(pulse.beatAvg, lon, lat, oxy);

void setup()
{
  printf("Starting ESP32 Application\n");
  connectToWiFi();

  oled_setup();
  pulse_setup();

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
      oled_display,
      "Oled Display",
      2048,
      &oled,
      1,
      NULL
    );
}

void loop() {
  printf("IR=");
  printf("%ld", pulse.irValue);
  printf(", BPM=");
  printf("%f", pulse.beatsPerMinute);
  printf(", Avg BPM=");
  printf("%d", pulse.beatAvg);
  oled.bpm = pulse.beatAvg;
  oled.lati = 70;
  oled.longi = 60;

  if (pulse.irValue < 50000)
    printf(" No finger?");

  printf("\n");
  delay(2000);
  // This sketch displays information every time a new sentence is correctly encoded.
}
