// ============================================================================
// File: WiFiSetup.h
// Author: Yanis Deplazes
// License: MIT License
// Copyright (c) 2025 Yanis Deplazes
// Description: Declares the WiFi connection logic using WiFiManager.
//              Ensures connection with captive portal fallback and restart on failure.
// ============================================================================

#pragma once

#include <Arduino.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager

/**
 * connectToWiFi()
 * ----------------
 * Initializes WiFi using WiFiManager with captive portal fallback.
 * Reboots the device if no connection is established within the timeout.
 */
void connectToWiFi();
