// ============================================================================
// File: installation.ino
// Author: Yanis Deplazes
// License: MIT License
// Copyright (c) 2025 Yanis Deplazes
// Description: Initializes HTTPS server, fetches sensor data from backend API,
//              and cycles through available devices on a timed interval.
// ============================================================================

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#include "WiFiSetup.h"
#include "DFPlayerManager.h"
#include "Client.h"
#include "Server.h"
#include "LEDManager.h"

// ---------------------------------------------------------------------------
// Pin & Serial Configuration
// ---------------------------------------------------------------------------

#define DFPLAYER_RX_PIN 16
#define DFPLAYER_TX_PIN 17
#define DFPLAYER_SERIAL_INDEX 2

// ---------------------------------------------------------------------------
// Application Constants
// ---------------------------------------------------------------------------

#define SERIAL_BAUD_RATE 115200
#define DEVICE_CYCLE_INTERVAL_MS 10000
#define SENSOR_FETCH_INTERVAL_MS 50000
#define INITIAL_INDEX 0
#define TIME_ZERO_MS 0

// ---------------------------------------------------------------------------
// State Variables
// ---------------------------------------------------------------------------

HardwareSerial mySerial(DFPLAYER_SERIAL_INDEX);
unsigned long lastSensorFetch = TIME_ZERO_MS;
unsigned long lastChange = TIME_ZERO_MS;

int currentIndex = INITIAL_INDEX;

/**
 * setup()
 * -------
 * Initializes all components: WiFi, LED strip, DFPlayer, server, and backend sync.
 */
void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  connectToWiFi();
  initLEDs();
  initDFPlayer(&mySerial, DFPLAYER_RX_PIN, DFPLAYER_TX_PIN);
  start_https_server();
  getDeviceCount();
  getLatestReadingForCurrentDevice();
}

/**
 * loop()
 * ------
 * Periodically cycles through devices by incrementing current index.
 */
void loop() {
  unsigned long now = millis();

  if (now - lastChange >= DEVICE_CYCLE_INTERVAL_MS) {
    next();
    lastChange = now;
  }
}
