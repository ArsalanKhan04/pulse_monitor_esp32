#ifndef OLED_H
#define OLED_H

#include <SPI.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C


struct oled_data {
  int bpm;
  int oxy;
  float longi;
  float lati;
};

void oled_display(void *pvParameters);
void oled_setup();
void oled_check();
void displaySlideshow(void *pvParameters);
void displayTask(void *pvParameters);

#endif


