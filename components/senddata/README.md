senddata component

Purpose

Provides Wi‑Fi connection and HTTP client functionality. Implements a FreeRTOS task (sendToServer) that periodically builds a JSON payload with bpm, lon, lat and oxy and POSTs it to a configured server.

Public API (in senddata.h)

- void connectToWiFi() — blocks until Wi‑Fi connects and stores the local IP in ipStr.
- void sendToWebhook() — example helper that POSTs a test JSON to a webhook URL.
- void sendToServer(void *pvParameters) — FreeRTOS task entry point. Expects a pointer to display_vars which holds pointers to bpm, lon, lat and oxy.

Behavior and notes

- Wi‑Fi credentials and serverUrl are hard-coded in senddata.cpp for testing; move to secure storage before production.
- sendToServer uses esp_http_client and intentionally configures transport as TCP (no SSL). It sets headers including X-Real-IP (the device's local IP).
- The task formats a JSON string into a fixed-size buffer (256 bytes) and posts it in a loop with a delay (vTaskDelay).

Integration

Pass an instance of display_vars to sendToServer (main.cpp demonstrates creating display_vars with pointers to live variables). The task reads values through these pointers each iteration.