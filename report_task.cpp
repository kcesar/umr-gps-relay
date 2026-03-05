#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "tasks.h"
#include "config_storage.h"

void ReportTask(void *pvParameters) {
  GpsData data;
  char url[256];
  for (;;) {
    if (xQueueReceive(gpsQueue, &data, portMAX_DELAY)) {
      sprintf(url, "https://caltopo.com/api/v1/position/report/%s?id=%s&lat=%.5f&lng=%.5f&time=%d000", trackKey.c_str(), deviceId.c_str(), data.lat, data.lon, data.time);
      Serial.println(url);

      WiFiClientSecure client;
      client.setInsecure();
      HTTPClient https;
      https.begin(client, url);
      Serial.println(https.GET());
      https.end();
    }
  }
}
