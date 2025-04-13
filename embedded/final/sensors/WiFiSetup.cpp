// ============================================================================
// File: WiFiSetup.cpp
// Author: Yanis Deplazes
// License: MIT License
// Copyright (c) 2025 Yanis Deplazes
// Description: Implements WiFi connection logic using WiFiManager with
//              timeout, fallback access point, and restart on failure.
// ============================================================================

#include "WiFiSetup.h"

// WiFiManager configuration constants
#define WIFI_AP_NAME "yanisdeplazes_IOT_Portal"
#define WIFI_AP_PASSWORD "superSecret123"
#define PORTAL_TIMEOUT_SEC 180
#define RESTART_DELAY_MS 3000

// Instance of WiFiManager
WiFiManager wifiManager;

void connectToWiFi() {
  wifiManager.setConfigPortalTimeout(PORTAL_TIMEOUT_SEC);

  if (!wifiManager.autoConnect(WIFI_AP_NAME, WIFI_AP_PASSWORD)) {
    Serial.println("WiFi connection failed. Restarting...");
    delay(RESTART_DELAY_MS);
    ESP.restart();
  }

  Serial.println("WiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
