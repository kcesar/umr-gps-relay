#ifndef CONFIG_STORAGE_H
#define CONFIG_STORAGE_H

#include <Preferences.h>

extern Preferences prefs;
extern String ssid;
extern String pass;
extern String deviceId;
extern String umrAddress;
extern String umrPassword;
extern String trackKey;

// Function declarations
void loadConfig();
void saveConfig(String s, String p, String id, String umrA, String umrP, String trackKey);

#endif