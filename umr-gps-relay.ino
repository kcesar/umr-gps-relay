#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "freertos/FreeRTOS.h"  // Include FreeRTOS header for EventGroupHandle_t
#include "freertos/event_groups.h"  // Include EventGroup header for the event group
#include "tasks.h"
#include "config_storage.h"

#define BOOT_BUTTON 0

EventGroupHandle_t systemEvents;
QueueHandle_t gpsQueue;
SemaphoreHandle_t configMutex;

void setup() {
  Serial.begin(115200);
  Serial.println("booting...");
  pinMode(BOOT_BUTTON, INPUT_PULLUP);

  systemEvents = xEventGroupCreate();
  gpsQueue = xQueueCreate(5, sizeof(GpsData));
  configMutex = xSemaphoreCreateMutex();

  loadConfig();

  if (digitalRead(BOOT_BUTTON) == LOW || ssid == NULL || ssid.isEmpty()) {
    Serial.println("Entering config only mode");
    xTaskCreatePinnedToCore(WebSetupTask, "WebConfig", 4096, NULL, 1, NULL, 1);
    return;
  }

  xTaskCreatePinnedToCore(WiFiTask, "WiFiTask", 4096, NULL, 3, NULL, 1);
  xTaskCreatePinnedToCore(NtpTask, "NtpTask", 4096, NULL, 3, NULL, 1);
  xTaskCreatePinnedToCore(WebConfigTask, "WebConfig", 4096, NULL, 1, NULL, 1);
  if (umrAddress != NULL && !umrAddress.isEmpty()) {
    xTaskCreatePinnedToCore(GpsPollTask, "GpsPoll", 8192, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(ReportTask, "Report", 8192, NULL, 1, NULL, 1);
  }
  Serial.println("done booting");
}

void loop() {
  // RTOS is running
}
