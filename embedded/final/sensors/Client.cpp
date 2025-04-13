// ============================================================================
// File: Client.cpp
// Author: Yanis Deplazes
// License: MIT License
// Copyright (c) 2025 Yanis Deplazes
// Description: Implementation of HTTPS client logic to communicate with API
//              for registering devices and uploading sensor data.
// ============================================================================

#include "Client.h"
#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// ----------------------------------------------------------------------------
// Constants & Macros
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
#define HEADER_END_LINE "\r"
#define PAYLOAD_DELAY_MS 200

#define ERROR_READING_ID -1
#define JSON_ARRAY_OFFSET 2

#define LOOP_START_INDEX 0
#define FLOAT_DECIMAL_PRECISION 2

// ----------------------------------------------------------------------------
// Internal Utilities
// ----------------------------------------------------------------------------

WiFiClientSecure secureClient;
static StaticJsonDocument<DOC_BUFFER_SIZE> jsonBuffer;
int deviceId = ERROR_READING_ID;

/**
 * Writes standard HTTP headers for GET/POST requests.
 */
void writeCommonHeaders(const String& method, const String& path, const String& contentType = "", size_t contentLength = 0) {
  secureClient.print(method + " ");
  secureClient.print(path);
  secureClient.println(" HTTP/1.1");

  secureClient.print("Host: ");
  secureClient.println(API_HOST);
  secureClient.println("User-Agent: ESP8266");
  secureClient.println("bypass-tunnel-reminder: true");
  secureClient.println("Connection: close");

  if (method == "POST") {
    secureClient.print("Content-Type: ");
    secureClient.println(contentType);
    secureClient.print("Content-Length: ");
    secureClient.println(contentLength);
  }

  secureClient.println();  // End of headers
}

/**
 * Skips HTTP headers in the response stream.
 */
void skipHttpHeaders() {
  while (secureClient.connected()) {
    String line = secureClient.readStringUntil('\n');
    if (line == HEADER_END_LINE) break;  // Empty line marks end of headers
  }
}

/**
 * Extracts a JSON array string from raw HTTP body.
 */
String extractJsonArray(const String& raw) {
  int start = raw.indexOf("[{");
  int end = raw.lastIndexOf("}]") + JSON_ARRAY_OFFSET;
  if (start == JSON_ARRAY_OPEN_INDEX || end <= start) return "";
  return raw.substring(start, end);
}

// ----------------------------------------------------------------------------
// HTTPS Client Functions
// ----------------------------------------------------------------------------

/**
 * httpGET(path)
 * -------------
 * Performs a HTTPS GET request to the specified API path.
 * 
 * @param path Relative API path to request.
 * @return Response body as String.
 */
String httpGET(const String& path) {
  Serial.println("GET " + path);

  secureClient.setInsecure();  // Skip cert validation (for dev)
  if (!secureClient.connect(API_HOST, API_PORT)) {
    Serial.println("Connection failed");
    return "";
  }

  writeCommonHeaders("GET", path);
  skipHttpHeaders();  // Skip response headers

  String responseBody;
  while (secureClient.available()) {
    responseBody += (char)secureClient.read();
  }

  secureClient.stop();
  return responseBody;
}

/**
 * httpPOST(path, payload, contentType)
 * ------------------------------------
 * Performs a HTTPS POST request with a JSON payload.
 * 
 * @param path Relative API path.
 * @param payload JSON-formatted string.
 * @param contentType Content type header value (default = application/json).
 * @return Response body as String.
 */
String httpPOST(const String& path, const String& payload, const String& contentType) {
  Serial.println("POST " + path);

  secureClient.setInsecure();  // Skip cert validation
  if (!secureClient.connect(API_HOST, API_PORT)) {
    Serial.println("Connection failed");
    return "";
  }

  writeCommonHeaders("POST", path, contentType, payload.length());
  secureClient.println(payload);  // Send body
  skipHttpHeaders();

  String responseBody;
  while (secureClient.available()) {
    responseBody += (char)secureClient.read();
  }

  secureClient.stop();
  return responseBody;
}

/**
 * getDevices()
 * ------------
 * Retrieves all registered devices from the server and parses the JSON array.
 * 
 * @return True if successful, false otherwise.
 */
bool getDevices() {
  String raw = httpGET(API_DEVICES_PATH);

  String json = extractJsonArray(raw);
  if (json == "") {
    Serial.println("JSON array not found");
    return false;
  }

  DeserializationError err = deserializeJson(jsonBuffer, json);
  if (err) {
    Serial.print("JSON parse error: ");
    Serial.println(err.c_str());
    return false;
  }

  return true;
}

/**
 * getDeviceIdForMac(mac)
 * -----------------------
 * Matches the device MAC address to a known device entry.
 * 
 * @param mac Device MAC address.
 * @return True if match is found, false otherwise.
 */
bool getDeviceIdForMac(const String& mac) {
  for (JsonObject device : jsonBuffer.as<JsonArray>()) {
    if (device["key"].as<String>() == mac) {
      deviceId = device["id"].as<int>();
      return true;
    }
  }
  return false;
}

/**
 * addDevice(mac)
 * --------------
 * Registers the device in the remote database by MAC address.
 * 
 * @param mac Device MAC address.
 */
void addDevice(const String& mac) {
  StaticJsonDocument<DOC_POST_SIZE> postDoc;
  postDoc["key"] = mac;
  postDoc["name"] = "Weather Station";

  String requestBody;
  serializeJson(postDoc, requestBody);
  httpPOST(API_DEVICES_PATH, requestBody);
}

/**
 * setupDevice()
 * -------------
 * Registers or retrieves the device ID for the current hardware.
 */
void setupDevice() {
  String mac = WiFi.macAddress();

  if (!getDevices()) {
    Serial.println("Failed to fetch devices");
    return;
  }

  if (getDeviceIdForMac(mac)) {
    Serial.println("Device already registered.");
    Serial.print("Device ID: ");
    Serial.println(deviceId);
  } else {
    Serial.println("Registering new device...");
    addDevice(mac);

    if (getDevices() && getDeviceIdForMac(mac)) {
      Serial.println("Device successfully registered.");
      Serial.print("Device ID: ");
      Serial.println(deviceId);
    } else {
      Serial.println("Failed to register and fetch device ID");
    }
  }
}

/**
 * addReading(sensorData, count)
 * -----------------------------
 * Sends a reading and all associated sensor data in a single HTTPS POST request
 * using the /api/reading-with-sensordata endpoint.
 *
 * @param sensorData Array of SensorData structs.
 * @param count Number of sensor data points.
 */
void addReading(SensorData sensorData[], size_t count) {
  if (deviceId == ERROR_READING_ID) {
    Serial.println("Device ID not set.");
    return;
  }

  StaticJsonDocument<DOC_SENSOR_ARRAY_SIZE> doc;
  doc["device_id"] = deviceId;

  JsonArray arr = doc.createNestedArray("sensor_data");
  for (size_t i = LOOP_START_INDEX; i < count; i++) {
    JsonObject obj = arr.createNestedObject();
    obj["sensor_id"] = sensorData[i].sensorId;
    obj["value"] = String(sensorData[i].value, FLOAT_DECIMAL_PRECISION);
  }

  String payload;
  serializeJson(doc, payload);
  httpPOST("/api/reading-with-sensordata", payload);
}
