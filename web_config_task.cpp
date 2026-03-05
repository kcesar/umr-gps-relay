#include "tasks.h"
#include <Update.h>
#include <WiFi.h>
#include <WebServer.h>
#include "config_storage.h"

// Create a simple HTML form for Wi-Fi and device settings
String htmlForm1 = R"rawliteral(
<html>
<body>
<h2>GPS Relay Configuration</h2>
<form action="/save" method="post">
  SSID:<br><input name="ssid" required><br>
  Password:<br><input name="pass" type="password" required><br>
  Device ID:<br><input name="deviceid" required><br>
  Modem IP:<br><input name="umr_ip" required><br>
  Modem Password:<br><input name="umr_pass" type="password" required><br>
  CalTopo Trackable Key:<br><input name="track_key" required><br>
  <input type="submit" value="Save">
</form>
<h3>Firmware Update</h3>
<form method="POST" action="/update" enctype="multipart/form-data">
  <input type="file" name="firmware">
  <input type="submit" value="Upload Firmware">
</form>
</body>
</html>
)rawliteral";

// The web server object to handle requests
WebServer server(80);

void handleRoot() {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", htmlForm1); // Show the HTML form
  server.sendContent("");
}

// Handle the form submission to save settings and reboot
void handleSave() {
  String ssid = server.arg("ssid");
  String pass = server.arg("pass");
  String deviceid = server.arg("deviceid");
  String devicepwd = server.arg("devicepwd");

  // Save the configuration to NVS
  saveConfig(
    ssid,
    pass,
    deviceid,
    server.arg("umr_ip"),
    server.arg("umr_pass"),
    server.arg("track_key")
  );
  
  // Inform the user and restart the device
  server.send(200, "text/plain", "Settings saved! Rebooting...");
  delay(1000);
  ESP.restart(); // Reboot to apply new Wi-Fi settings
}

void handleUpdateUpload() {
  HTTPUpload& upload = server.upload();

  if (upload.status == UPLOAD_FILE_START) {

    Serial.println("OTA upload start");

    if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
      Update.printError(Serial);
    }

  } else if (upload.status == UPLOAD_FILE_WRITE) {

    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Update.printError(Serial);
    }

  } else if (upload.status == UPLOAD_FILE_END) {

    if (Update.end(true)) {
      Serial.printf("OTA Success: %u bytes\n", upload.totalSize);
    } else {
      Update.printError(Serial);
    }
  }
}

void handleUpdateFinished() {

  if (Update.hasError()) {
    server.send(500, "text/plain", "OTA Failed");
  } else {
    server.send(200, "text/plain", "OTA Success. Rebooting...");
    delay(1000);
    ESP.restart();
  }
}

void startConfigPortal() {
  // Define routes for handling requests
  server.on("/", handleRoot); // Root page to display the HTML form
  server.on("/save", HTTP_POST, handleSave); // Form submission handler

  server.on(
    "/update",
    HTTP_POST,
    handleUpdateFinished,
    handleUpdateUpload
  );

  // Start the web server
  server.begin();
  Serial.println("Config portal started.");

  for (;;) {
    server.handleClient(); // Continuously handle client requests (this is the web server loop)
    vTaskDelay(pdMS_TO_TICKS(10)); // Yield the task for a while
  }
}

void WebSetupTask(void *pvParameters) {
  Serial.println("Request for config mode or config not found. Connect to 'GPS-Setup' to configure.");
  // Start an AP mode with the SSID "ESP32-Setup"
  WiFi.softAP("GPS-Setup");
  startConfigPortal();
}

void WebConfigTask(void *pvParameters) {
    xEventGroupWaitBits(systemEvents,
                      WIFI_CONNECTED_BIT,
                      false,
                      true,
                      portMAX_DELAY);
  Serial.println("Starting web server");
  startConfigPortal(); // Start the config portal (network mode)
}