#ifndef SENDDATA_H
#define SENDDATA_H

#include <WiFi.h>
#include "esp_http_client.h"

void connectToWiFi();
void sendToWebhook();

struct display_vars {
  int* bpm;
  float* lon;
  float* lat;
  int* oxy;

  display_vars(int& bpm, float& lon, float& lat, int& oxy) : bpm(&bpm), lon(&lon), lat(&lat), oxy(&oxy) {}
};

void sendToServer(void *pvParameters);

#endif
