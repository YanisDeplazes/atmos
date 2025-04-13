// ============================================================================
// File: Client.h
// Author: Yanis Deplazes
// License: MIT License
// Copyright (c) 2025 Yanis Deplazes
// Description: Declares the HTTPS client interface for registering devices
//              and uploading sensor readings to the remote API.
// ============================================================================

#pragma once
#include <Arduino.h>

// ----------------------------------------------------------------------------
// Constants & Macros (shared with Client.cpp to avoid magic numbers)
// ----------------------------------------------------------------------------
#define API_HOST "yanisdeplazes.loca.lt"
#define API_PORT 443
#define API_DEVICES_PATH "/api/device"
#define API_READING_PATH "/api/reading"
#define API_SENSORDATA_PATH "/api/sensordata"

#define DOC_BUFFER_SIZE 4096
#define DOC_POST_SIZE 128
#define DOC_SENSOR_ARRAY_SIZE 1024
#define DOC_READING_LIST_SIZE 2048

#define JSON_ARRAY_OPEN_INDEX 0
#define JSON_ARRAY_OFFSET 2
#define LOOP_START_INDEX 0
#define HEADER_END_LINE "\r"

#define ERROR_READING_ID -1
#define PAYLOAD_DELAY_MS 200
#define FLOAT_DECIMAL_PRECISION 2

// ----------------------------------------------------------------------------
// Data Structure
// ----------------------------------------------------------------------------
struct SensorData {
  int sensorId;
  float value;
};

// ----------------------------------------------------------------------------
// HTTPS Client Function Declarations
// ----------------------------------------------------------------------------

/**
 * Performs a HTTPS GET request to the specified API path.
 */
String httpGET(const String& path);

/**
 * Performs a HTTPS POST request with a JSON payload.
 */
String httpPOST(const String& path, const String& payload, const String& contentType = "application/json");

/**
 * Retrieves and deserializes the list of registered devices.
 */
bool getDevices();

/**
 * Finds a device entry matching the given MAC address.
 */
bool getDeviceIdForMac(const String& mac);

/**
 * Registers a new device in the backend.
 */
void addDevice(const String& mac);

/**
 * Sets up the device by fetching or creating its backend entry.
 */
void setupDevice();

/**
 * Sends a reading and all associated sensor data in a single HTTPS POST request
 * using the /api/reading-with-sensordata endpoint.
 */
void addReading(SensorData sensorData[], size_t count);


// ----------------------------------------------------------------------------
// Shared State
// ----------------------------------------------------------------------------

extern int deviceId;
