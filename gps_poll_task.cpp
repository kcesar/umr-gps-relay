#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "tasks.h"
#include "config_storage.h"
#include <ArduinoJson.h>

void GpsPollTask(void *pvParameters) {
  xEventGroupWaitBits(systemEvents,
                      WIFI_CONNECTED_BIT | TIME_SYNCED_BIT,
                      false,
                      true,
                      portMAX_DELAY);

  for (;;) {
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient https;
    https.setTimeout(30000);
    char url[42];
    
    // Step 1: get token
    
    sprintf(url, "https://%s/ubus/call/session", umrAddress);
    https.begin(client, url);
    https.addHeader("Content-Type", "application/json");

    DynamicJsonDocument doc(256);
    deserializeJson(doc, "{\"jsonrpc\":\"2.0\",\"method\":\"login\",\"params\":{\"username\":\"ui\",\"timeout\":2129920}}");
    doc["params"]["password"] = umrPassword;
    String body;
    serializeJson(doc, body);
    
    if (https.POST(body) == HTTP_CODE_OK) {
      DynamicJsonDocument resp(512);
      deserializeJson(resp, https.getString());
      https.end();

      String token = resp["result"]["ubus_rpc_session"];
      Serial.println("Have UMR token");

      sprintf(url, "https://%s/ubus/call/uimqtt", umrAddress);
      https.begin(client, url);
      https.addHeader("Content-Type", "application/json");
      sprintf(url, "Bearer %s", token.c_str());
      https.addHeader("Authorization", url);
      body = "{\"jsonrpc\":\"2.0\",\"method\":\"InfoGpsDump\",\"params\":{}}";

      int status = https.POST(body);
      if (status == HTTP_CODE_OK) {
        GpsData data;
        Serial.print("GPS: ");
        long start = millis();
        deserializeJson(resp, https.getString());
        Serial.print(millis() - start);
        Serial.println("ms");
        data.lat = resp["result"]["latitude"];
        data.lon = resp["result"]["longitude"];
        data.time = resp["result"]["timestamp"];
        xQueueSend(gpsQueue, &data, 0);
      } else {
        Serial.println(status);
      }
    }
    https.end();
    vTaskDelay(pdMS_TO_TICKS(30000));
  }
}
