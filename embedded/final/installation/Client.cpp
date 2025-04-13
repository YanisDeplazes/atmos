// ============================================================================
// File: Client.cpp
// Author: Yanis Deplazes
// License: MIT License
// Copyright (c) 2025 Yanis Deplazes
// Description: Implementation of HTTPS client logic to communicate with API
//              for retrieving latest sensor data from all registered devices.
// ============================================================================

#include "Client.h"
#include "Server.h"
#include "LEDManager.h"
#include "DFPlayerManager.h"
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// ----------------------------------------------------------------------------
// Constants & Macros
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
// State
// ----------------------------------------------------------------------------

WiFiClientSecure secureClient;
static StaticJsonDocument<DOC_BUFFER_SIZE> deviceListBuffer;
static StaticJsonDocument<DOC_BUFFER_SIZE> readingBuffer;

int deviceCount = 0;
float latestTempDHT = 0;
float latestHumDHT = 0;
float latestLux = 0;
float latestWater = 0;
float latestTempBMP = 0;
float latestPressure = 0;

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
    if (line == HEADER_END_LINE) break;
  }
}

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

  secureClient.setInsecure();  // Skip cert validation
  if (!secureClient.connect(API_HOST, API_PORT)) {
    Serial.println("Connection failed");
    return "";
  }

  writeCommonHeaders("GET", path);
  skipHttpHeaders();

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
 * @param contentType Content type header value.
 * @return Response body as String.
 */
String httpPOST(const String& path, const String& payload, const String& contentType) {
  Serial.println("POST " + path);

  secureClient.setInsecure();
  if (!secureClient.connect(API_HOST, API_PORT)) {
    Serial.println("Connection failed");
    return "";
  }

  writeCommonHeaders("POST", path, contentType, payload.length());
  secureClient.println(payload);
  skipHttpHeaders();

  String responseBody;
  while (secureClient.available()) {
    responseBody += (char)secureClient.read();
  }

  secureClient.stop();
  return responseBody;
}

/**
 * getDeviceCount()
 * ----------------
 * Loads the list of devices and populates deviceListBuffer.
 * 
 * @return True if successful, false otherwise.
 */
bool getDeviceCount() {
  String raw = httpGET(API_DEVICES_PATH);

  int start = raw.indexOf("[{");
  int end = raw.lastIndexOf("}]") + JSON_ARRAY_OFFSET;

  if (start == JSON_ARRAY_OPEN_INDEX || end <= start) {
    Serial.println("JSON array not found");
    return false;
  }

  String json = raw.substring(start, end);
  DeserializationError err = deserializeJson(deviceListBuffer, json);
  if (err) {
    Serial.print("JSON parse error: ");
    Serial.println(err.c_str());
    return false;
  }

  deviceCount = deviceListBuffer.size();
  return true;
}

/**
 * getLatestReadingForCurrentDevice()
 * ----------------------------------
 * Fetches latest sensor readings for the current device.
 * 
 * @return True if successful, false otherwise.
 */
bool getLatestReadingForCurrentDevice() {
  if (deviceCount == NO_DEVICES || !deviceListBuffer[currentIndex].containsKey("id")) {
    Serial.println("No device loaded");
    return false;
  }

  String deviceId = deviceListBuffer[currentIndex]["id"].as<String>();
  String raw = httpGET(API_LATEST_READING_PATH + deviceId);

  int start = raw.indexOf("[{");
  int end = raw.lastIndexOf("}]") + JSON_ARRAY_OFFSET;

  if (start == JSON_ARRAY_OPEN_INDEX || end <= start) {
    Serial.println("JSON array not found");
    return false;
  }

  String json = raw.substring(start, end);
  DeserializationError err = deserializeJson(readingBuffer, json);
  if (err) {
    Serial.print("JSON parse error: ");
    Serial.println(err.c_str());
    return false;
  }

  JsonArray arr = readingBuffer.as<JsonArray>();
  for (JsonObject obj : arr) {
    String name = obj["sensor_name"].as<String>();
    float value = obj["value"].as<float>();

    if (name == SENSOR_NAME_TEMP) {
      latestTempDHT = value;
    } else if (name == SENSOR_NAME_HUM) {
      latestHumDHT = value;
    } else if (name == SENSOR_NAME_LUX) {
      latestLux = value;
    } else if (name == SENSOR_NAME_WATER) {
      latestWater = value;
    } else if (name == SENSOR_NAME_TEMP_BMP) {
      latestTempBMP = value;
    } else if (name == SENSOR_NAME_PRESS) {
      latestPressure = value;
    }
  }

  sendCurrentIndexUpdate(currentIndex);
  playBasedOnSensorData(latestTempDHT, latestHumDHT, latestLux, latestWater, latestPressure);
  showColor(latestTempDHT);
  return true;
}

/**
 * next()
 * ------
 * Moves to the next device and retrieves its latest reading.
 */
void next() {
  if (deviceCount == NO_DEVICES) return;
  currentIndex = (currentIndex + INDEX_INCREMENT) % deviceCount;
  getLatestReadingForCurrentDevice();
  sendCurrentIndexUpdate(currentIndex);
}

/**
 * previous()
 * ----------
 * Moves to the previous device and retrieves its latest reading.
 */
void previous() {
  if (deviceCount == NO_DEVICES) return;
  currentIndex = (currentIndex - INDEX_DECREMENT + deviceCount) % deviceCount;
  getLatestReadingForCurrentDevice();
}

/**
 * sendCurrentIndexUpdate(currentIndex)
 * ------------------------------------
 * Sends the current index to the server via WebSocket publisher endpoint.
 * 
 * @param currentIndex Index to publish.
 */
void sendCurrentIndexUpdate(int currentIndex) {
  String payload = "{\"index\": " + String(currentIndex) + "}";
  String response = httpPOST(API_INDEX_UPDATE_PATH, payload, "application/json");
}
