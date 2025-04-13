// ============================================================================
// File: Client.h
// Author: Yanis Deplazes
// License: MIT License
// Copyright (c) 2025 Yanis Deplazes
// Description: Declares the HTTPS client interface for retrieving the latest
//              sensor readings from all registered devices via a remote API.
// ============================================================================

#pragma once
#include <Arduino.h>

// ----------------------------------------------------------------------------
// Constants & Macros (shared with Client.cpp to avoid magic numbers)
// ----------------------------------------------------------------------------
#define API_HOST "yanisdeplazes.loca.lt"
#define API_PORT 443
#define API_DEVICES_PATH "/api/device"
#define API_LATEST_READING_PATH "/api/views/LatestDeviceReadings?device_id="
#define API_INDEX_UPDATE_PATH "/api/installation/publish"

#define DOC_BUFFER_SIZE 4096
#define HEADER_END_LINE "\r"

#define JSON_ARRAY_OFFSET 2
#define JSON_ARRAY_OPEN_INDEX 0
#define LOOP_START_INDEX 0

#define FLOAT_DECIMAL_PRECISION 2

#define SENSOR_NAME_TEMP "DHT11 Temperature Sensor"
#define SENSOR_NAME_HUM "DHT11 Humidity Sensor"
#define SENSOR_NAME_LUX "BH1750 Light Sensor"
#define SENSOR_NAME_WATER "Analog Water Sensor"
#define SENSOR_NAME_PRESS "BMP180 Pressure Sensor"
#define SENSOR_NAME_TEMP_BMP "BMP180 Temperature Sensor"

#define NO_DEVICES 0
#define INDEX_INCREMENT 1
#define INDEX_DECREMENT 1

// ----------------------------------------------------------------------------
// HTTPS Client Function Declarations
// ----------------------------------------------------------------------------

/**
 * Performs a HTTPS GET request to the specified API path.
 */
String httpGET(const String& path);

/**
 * Performs a HTTPS POST request with a payload and content-type.
 */
String httpPOST(const String& path, const String& payload, const String& contentType);

/**
 * Loads and parses all registered devices.
 */
bool getDeviceCount();

/**
 * Loads and parses the latest readings for the current device.
 */
bool getLatestReadingForCurrentDevice();

/**
 * Advances to the next device (cyclic).
 */
void next();

/**
 * Goes back to the previous device (cyclic).
 */
void previous();

/**
 * Publishes the current device index to the server.
 */
void sendCurrentIndexUpdate(int currentIndex);

// ----------------------------------------------------------------------------
// Shared State
// ----------------------------------------------------------------------------

extern int currentIndex;