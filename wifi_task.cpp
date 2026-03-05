#include "config_storage.h"
#include "tasks.h"
#include <WiFi.h>

void WiFiTask(void *pvParameters) {
  for (;;) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("Trying to connect to ");
      Serial.println(ssid);

      WiFi.begin(ssid.c_str(), pass.c_str());
      int retries = 0;
      while (WiFi.status() != WL_CONNECTED && retries < 20) {
        vTaskDelay(pdMS_TO_TICKS(500));
        retries++;
      }

      if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Connected. ");
        Serial.println(WiFi.localIP().toString());
        xEventGroupSetBits(systemEvents, WIFI_CONNECTED_BIT);
      } else {
        Serial.print("Not connected to wi-fi");
        xEventGroupClearBits(systemEvents, WIFI_CONNECTED_BIT);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}