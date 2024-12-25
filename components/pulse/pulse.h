#include <Arduino.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "spo2_algorithm.h"
#include <stdio.h>
#include <esp_log.h>



struct pulse_vars {
  float beatsPerMinute;
  int beatAvg;
  int SPO2;
  long irValue;
};

void pulse_setup();

void get_pulse(void *pvParameters);
