#ifndef TASKS_H
#define TASKS_H

#include "freertos/FreeRTOS.h"  // Include FreeRTOS header for EventGroupHandle_t
#include "freertos/event_groups.h"  // Include EventGroup header for the event group

#define WIFI_CONNECTED_BIT  (1 << 0)
#define TIME_SYNCED_BIT     (1 << 1)
#define CONFIG_MODE_BIT     (1 << 2)

extern EventGroupHandle_t systemEvents;
extern QueueHandle_t gpsQueue;
extern SemaphoreHandle_t configMutex;

extern void WiFiTask(void *pvParameters);
extern void NtpTask(void *pvParameters);
extern void OtaTask(void *pvParameters);
extern void GpsPollTask(void *pvParameters);
extern void ReportTask(void *pvParameters);
extern void WebConfigTask(void *pvParameters);
extern void WebSetupTask(void *pvParameters);

struct GpsData {
  float lat;
  float lon;
  long time;
};
#endif