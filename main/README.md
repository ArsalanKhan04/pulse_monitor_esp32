main/main.cpp — Overview

Purpose

This file is the program entry point for the firmware running on the ESP32. It initializes peripherals, sets up Wi‑Fi and creates FreeRTOS tasks for sensor readout, display and networking.

Key points

- Global objects: pulse (pulse_vars), gps_v (gps_vars), oled (oled_data) and display_vars (holds pointers into the live values for safe, low-cost passing to the network task).
- setup(): connects to Wi‑Fi, initializes OLED, pulse and GPS subsystems, and creates these tasks:
  - sendToServer — sends data to remote server (stack 4096)
  - get_pulse — reads MAX3010x and computes BPM/SpO2 (stack 8192)
  - displaySlideshow — updates OLED (stack 2048)
  - get_gps_data — reads GPS (stack 2048)
- loop(): simple periodic status print to serial and copies pulse values into the oled structure for display.

Notes

- Most work happens in FreeRTOS tasks; loop() runs only for debugging/status.
- The display_vars object created in main binds live variables for the HTTP task to read without copying large structures.