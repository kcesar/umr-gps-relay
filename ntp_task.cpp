#include "tasks.h"
#include <WiFi.h>
#include <time.h>

void NtpTask(void *pvParameters) {
  // Wait for WiFi
  xEventGroupWaitBits(systemEvents,
                      WIFI_CONNECTED_BIT,
                      false,
                      true,
                      portMAX_DELAY);
  Serial.println("Getting current time...");
  configTime(0, 0, "pool.ntp.org");

  struct tm timeinfo;

  while (!getLocalTime(&timeinfo)) {
    vTaskDelay(pdMS_TO_TICKS(500));
  }

  Serial.print("Time (UTC): ");
  char locTime[9];
  sprintf(locTime, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  Serial.println(locTime);

  xEventGroupSetBits(systemEvents, TIME_SYNCED_BIT);
  vTaskDelete(NULL); // Done forever
}