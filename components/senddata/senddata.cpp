#include "senddata.h"


const char* ssid = "Galaxy A7159B4";
const char* password = "bruh1234";

const char* webhookUrl = "http://webhook.site/200b0e63-4ae5-4455-b10e-f4de3e972ee4";
const char* serverUrl = "http://da9a-2a09-bac1-5b40-20-00-3c4-43.ngrok-free.app/post_sensor_data";
const char* TAG = "HTTP_CLIENT";

String ipStr;

void connectToWiFi() {
    WiFi.begin(ssid, password);
    printf("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        printf(".");
    }
    printf("\nConnected to Wi-Fi\n");
    printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
    ipStr = WiFi.localIP().toString();
}


void sendToWebhook() {
    esp_log_level_set("*", ESP_LOG_VERBOSE);  // Enable verbose logging for all ESP32 components
    if (WiFi.status() == WL_CONNECTED) {  // Check Wi-Fi connection
        esp_http_client_config_t config = {
          .host = serverUrl,
          .path = "/post",
          .timeout_ms = 5000,
          .skip_cert_common_name_check = true,
        };

        esp_http_client_handle_t client = esp_http_client_init(&config);
        const char *post_data = "{\"message\":\"Hello, webhook!\",\"value\":42}";
        esp_http_client_set_url(client, webhookUrl);
        esp_http_client_set_method(client, HTTP_METHOD_POST);
        esp_http_client_set_header(client, "Content-Type", "application/json");
        esp_http_client_set_post_field(client, post_data, strlen(post_data));
        esp_err_t err = esp_http_client_perform(client);

        if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP HEAD Status = %d, content_length = %"PRId64,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
        } else {
            ESP_LOGI(TAG, "HTTP HEAD request failed: %s", esp_err_to_name(err));
        }

        esp_http_client_cleanup(client);
    } else {
        printf("Wi-Fi not connected\n");
    }
}


void sendToServer(void *pvParameters) {
    display_vars *vars = (display_vars*) pvParameters;
    esp_http_client_config_t config = {
      .url = serverUrl,
      .cert_pem = NULL,  // Make sure no certificates are provided (no SSL)
      .timeout_ms = 5000,
      .transport_type = HTTP_TRANSPORT_OVER_TCP,  // Explicitly set to TCP, to avoid SSL/TLS
      .skip_cert_common_name_check = true  // Skip SSL certificate checks
    };

    while(1){
      char payload[256];  // Adjust the size as needed

      snprintf(payload, sizeof(payload),
               "{\"bpm\":%d, \"lon\":%.6f, \"lat\":%.6f, \"oxy\":%d}",
               *(vars->bpm), *(vars->lon), *(vars->lat), *(vars->oxy));  // Format JSON string


      esp_http_client_handle_t client = esp_http_client_init(&config);

      esp_http_client_set_method(client, HTTP_METHOD_POST);
      // Set the headers for the HTTP request
      esp_http_client_set_header(client, "X-Real-IP", ipStr.c_str());
      esp_http_client_set_header(client, "Content-Type", "application/json");
      esp_http_client_set_header(client, "accept", "application/json");
      esp_http_client_set_header(client, "accept-encoding", "gzip, deflate");

      // Set the POST data (JSON payload)
      esp_http_client_set_post_field(client, payload, strlen(payload));

      // Perform the HTTP POST request
      esp_err_t err = esp_http_client_perform(client);

      // Check the response status
      if (err == ESP_OK) {
          printf("Data sent successfully! Response code: %d\n", esp_http_client_get_status_code(client));
      } else {
          printf("Failed to send data. Error: %s\n", esp_err_to_name(err));
      }
      esp_http_client_cleanup(client);
      vTaskDelay(5000 / portTICK_PERIOD_MS);  // Delay for 10 seconds
  }
}




