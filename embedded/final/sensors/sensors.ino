// ============================================================================
// File: sensors.ino
// Author: Yanis Deplazes
// License: MIT License
// Copyright (c) 2025 Yanis Deplazes
// Description: Reads sensor data (DHT11, BMP180, BH1750, analog water sensor),
//              sends data to remote API, and prints current sensor values.
// ============================================================================

#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <BH1750.h>
#include <DHT.h>
#include <WiFiManager.h>

#include "WifiSetup.h"
#include "Client.h"

// ---------------------------------------------------------------------------
// Pin Configuration
// ---------------------------------------------------------------------------

#define DHTPIN 0
#define DHTTYPE DHT11
#define WATERPIN A0
#define WIFI_LED_PIN 12

// ---------------------------------------------------------------------------
// Sensor ID Mapping (matches backend database IDs)
// ---------------------------------------------------------------------------

#define SENSOR_ID_DHT11_TEMPERATURE 1
#define SENSOR_ID_DHT11_HUMIDITY 2
#define SENSOR_ID_BH1750_LUX 3
#define SENSOR_ID_ANALOG_WATER 4
#define SENSOR_ID_BMP180_TEMPERATURE 5
#define SENSOR_ID_BMP180_PRESSURE 6

// ---------------------------------------------------------------------------
// Internal Index Mapping for sensorData[]
// ---------------------------------------------------------------------------

#define IDX_DHT11_TEMP 0
#define IDX_DHT11_HUMID 1
#define IDX_BH1750_LUX 2
#define IDX_WATER_ANALOG 3
#define IDX_BMP180_TEMP 4
#define IDX_BMP180_PRESSURE 5
#define SENSOR_COUNT 6
#define LOOP_START_INDEX 0

// ---------------------------------------------------------------------------
// Application Constants
// ---------------------------------------------------------------------------

#define SERIAL_BAUD_RATE 115200
#define LOOP_DELAY_MS 2000
#define MIN_VALID_LUX 0
#define PA_TO_HPA_DIVISOR 100.0
#define FLOAT_DISPLAY_PRECISION 2

// ---------------------------------------------------------------------------
// Sensor Instances
// ---------------------------------------------------------------------------

BH1750 lightMeter;
Adafruit_BMP085 bmp;
DHT dht(DHTPIN, DHTTYPE);
bool bmpReady = false;

// ---------------------------------------------------------------------------
// SensorData array for transmitting values
// ---------------------------------------------------------------------------

SensorData sensorData[SENSOR_COUNT] = {
  { SENSOR_ID_DHT11_TEMPERATURE, 0 },
  { SENSOR_ID_DHT11_HUMIDITY, 0 },
  { SENSOR_ID_BH1750_LUX, 0 },
  { SENSOR_ID_ANALOG_WATER, 0 },
  { SENSOR_ID_BMP180_TEMPERATURE, 0 },
  { SENSOR_ID_BMP180_PRESSURE, 0 }
};

/**
 * setup()
 * -------
 * Initializes serial, WiFi, sensors, and API device registration.
 */
void setup() {
  pinMode(WIFI_LED_PIN, OUTPUT);
  digitalWrite(WIFI_LED_PIN, LOW);

  Serial.begin(SERIAL_BAUD_RATE);
  connectToWiFi();
  setupDevice();

  Wire.begin();
  bmpReady = bmp.begin();
  if (!bmpReady) {
    Serial.println("BMP180 not detected!");
  }

  dht.begin();

  bool bh1750Ready = lightMeter.begin();
  if (!bh1750Ready) {
    Serial.println("⚠️ BH1750 sensor not detected!");
  }


  digitalWrite(WIFI_LED_PIN, HIGH);
}

/**
 * loop()
 * ------
 * Polls all sensors, uploads values, prints results, and waits.
 */
void loop() {
  readDht11();
  readBh1750();
  readWaterSensor();
  readBmp180();
  addReading(sensorData, SENSOR_COUNT);
  echoReadings();
  delay(LOOP_DELAY_MS);
}

/**
 * readDht11()
 * -----------
 * Reads temperature and humidity from DHT11 sensor.
 */
void readDht11() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (!isnan(temp)) sensorData[IDX_DHT11_TEMP].value = temp;
  if (!isnan(hum)) sensorData[IDX_DHT11_HUMID].value = hum;
}

/**
 * readBh1750()
 * ------------
 * Reads lux from BH1750 light sensor.
 */
void readBh1750() {
  float lux = lightMeter.readLightLevel();
  if (lux >= MIN_VALID_LUX) {
    sensorData[IDX_BH1750_LUX].value = lux;
  }
}

/**
 * readWaterSensor()
 * -----------------
 * Reads analog value from water sensor.
 */
void readWaterSensor() {
  sensorData[IDX_WATER_ANALOG].value = analogRead(WATERPIN);
}

/**
 * readBmp180()
 * ------------
 * Reads temperature and pressure from BMP180 sensor.
 */
void readBmp180() {
  if (!bmpReady) return;

  sensorData[IDX_BMP180_TEMP].value = bmp.readTemperature();
  sensorData[IDX_BMP180_PRESSURE].value = bmp.readPressure() / PA_TO_HPA_DIVISOR;
}

/**
 * echoReadings()
 * --------------
 * Prints current sensor readings to serial monitor.
 */
void echoReadings() {
  Serial.println("--- Sensor ---");
  for (size_t i = LOOP_START_INDEX; i < SENSOR_COUNT; i++) {
    Serial.print("Sensor ID ");
    Serial.print(sensorData[i].sensorId);
    Serial.print(": ");
    Serial.println(sensorData[i].value, FLOAT_DISPLAY_PRECISION);
  }
  Serial.println("------------------------");
}
