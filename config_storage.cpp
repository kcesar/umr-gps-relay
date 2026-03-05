#include "config_storage.h"

Preferences prefs;
String ssid;
String pass;
String deviceId;
String umrAddress;
String umrPassword;
String trackKey;

// Load config from NVS
void loadConfig() {
  prefs.begin("config", true);
  ssid = prefs.getString("ssid", "");
  pass = prefs.getString("pass", "");
  deviceId = prefs.getString("device_id", "unset");
  umrAddress = prefs.getString("umr_ip", "");
  umrPassword = prefs.getString("umr_pass", "");
  trackKey = prefs.getString("track_key", "");
  prefs.end();
  Serial.print("UMR Address: ");
  Serial.println(umrAddress);
  Serial.print("Device ID: ");
  Serial.println(deviceId);
  Serial.print("Track Key: ");
  Serial.println(trackKey);
}

// Save config to NVS
void saveConfig(String s, String p, String id, String umrA, String umrP, String trackKey) {
  Serial.print("New Device ID: ");
  Serial.println(id);
  prefs.begin("config", false);
  prefs.putString("ssid", s);
  prefs.putString("pass", p);
  prefs.putString("device_id", id);
  prefs.putString("umr_ip", umrA);
  prefs.putString("umr_pass", umrP);
  prefs.putString("track_key", trackKey);
  prefs.end();
}