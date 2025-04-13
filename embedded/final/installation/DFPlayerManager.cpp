// ============================================================================
// File: DFPlayerManager.cpp
// Author: Yanis Deplazes
// License: MIT License
// Copyright (c) 2025 Yanis Deplazes
// Description: Manages DFPlayer Mini logic including initialization, volume
//              control, and playback of indexed ambient audio files.
// ============================================================================

#include "DFPlayerManager.h"
#include "DFRobotDFPlayerMini.h"

// ----------------------------------------------------------------------------
// Constants & Macros
// ----------------------------------------------------------------------------
#define TRACK_DEFAULT 6
#define RAIN_THRESHOLD 500.0f
#define TEMP_VERY_COLD_THRESHOLD 5.0f
#define TEMP_MILD_DAY_THRESHOLD 25.0f
#define TEMP_WARM_MIN 20.0f
#define TEMP_HOT_MIN 28.0f
#define PRESSURE_LOW 1000.0f
#define PRESSURE_MILD 1015.0f
#define LUX_NIGHT_MAX 50.0f
#define LUX_SUNSET_MAX 200.0f
#define LUX_DAY_MIN 400.0f
#define LUX_DAY_MAX 1000.0f
#define HUMIDITY_HIGH 80.0f
#define DFPLAYER_BAUDRATE 9600

// ----------------------------------------------------------------------------
// Volume Configuration Macros
// ----------------------------------------------------------------------------
#define DFPLAYER_VOLUME_MIN 0
#define DFPLAYER_VOLUME_MAX 30
#define DFPLAYER_VOLUME_INIT 20

// ----------------------------------------------------------------------------
// Static State
// ----------------------------------------------------------------------------
static DFRobotDFPlayerMini player;
static HardwareSerial* dfSerial;
static uint8_t currentVolume = DFPLAYER_VOLUME_INIT;
static uint8_t lastTrackPlayed = 0;
static int rxPin, txPin;

// ----------------------------------------------------------------------------
// DFPlayer Initialization
// ----------------------------------------------------------------------------

/**
 * initDFPlayer()
 * --------------
 * Initializes the DFPlayer Mini via a hardware serial connection.
 *
 * @param serial Pointer to HardwareSerial instance.
 * @param rx Receive pin number.
 * @param tx Transmit pin number.
 */
void initDFPlayer(HardwareSerial* serial, int rx, int tx) {
  dfSerial = serial;
  rxPin = rx;
  txPin = tx;

  dfSerial->begin(DFPLAYER_BAUDRATE, SERIAL_8N1, rxPin, txPin);

  if (!player.begin(*dfSerial)) {
    Serial.println("DFPlayer init failed");
    while (true)
      ;  // Halt if initialization fails
  }

  setVolume(currentVolume);
}

/**
 * setVolume()
 * -----------
 * Sets the volume level within valid range.
 *
 * @param vol Volume level (0–30).
 */
void setVolume(uint8_t vol) {
  currentVolume = constrain(vol, DFPLAYER_VOLUME_MIN, DFPLAYER_VOLUME_MAX);
  player.volume(currentVolume);
}

/**
 * volumeUp()
 * ----------
 * Increases volume by one step, respecting maximum limit.
 */
void volumeUp() {
  if (currentVolume < DFPLAYER_VOLUME_MAX) {
    setVolume(++currentVolume);
  }
}

/**
 * volumeDown()
 * ------------
 * Decreases volume by one step, respecting minimum limit.
 */
void volumeDown() {
  if (currentVolume > DFPLAYER_VOLUME_MIN) {
    setVolume(--currentVolume);
  }
}

/**
 * playTrack()
 * -----------
 * Plays a specific ambient track in loop, only if different from the last.
 *
 * @param track Track index to play.
 */
void playTrack(uint8_t track) {
  if (track != lastTrackPlayed) {
    Serial.println("Playing new Track");
    Serial.println(track);
    player.loop(track);
    lastTrackPlayed = track;
  }
}

/**
 * playBasedOnSensorData()
 * -----------------------
 * Determines appropriate ambient track based on sensor input.
 *
 * @param temp Temperature in °C.
 * @param hum Relative humidity in %.
 * @param lux Ambient light in lux.
 * @param water Analog water level.
 * @param pressure Atmospheric pressure in hPa.
 */
void playBasedOnSensorData(float temp, float hum, float lux, float water, float pressure) {
  uint8_t trackToPlay = TRACK_DEFAULT;

  if (water > RAIN_THRESHOLD) {
    trackToPlay = TRACK_RAINSTORM;
  } else if (temp < TEMP_VERY_COLD_THRESHOLD && pressure < PRESSURE_LOW) {
    trackToPlay = TRACK_BLIZZARD_WIND;
  } else if (temp < TEMP_VERY_COLD_THRESHOLD && pressure >= PRESSURE_LOW && lux <= LUX_SUNSET_MAX) {
    trackToPlay = TRACK_COLD_SUNSET_WIND;
  } else if (temp < TEMP_VERY_COLD_THRESHOLD && pressure >= PRESSURE_LOW) {
    trackToPlay = TRACK_COLD_WINTER_DAY;
  } else if (temp <= TEMP_WARM_MIN && lux < LUX_NIGHT_MAX) {
    trackToPlay = TRACK_CALM_NIGHT_CRICKETS;
  } else if (temp < TEMP_WARM_MIN && pressure < PRESSURE_LOW) {
    trackToPlay = TRACK_MILD_STORM_WIND;
  } else if (temp < TEMP_WARM_MIN && pressure >= PRESSURE_LOW && lux > LUX_DAY_MIN) {
    trackToPlay = TRACK_SPRING_MORNING_BIRDS;
  } else if (temp < TEMP_WARM_MIN && lux < LUX_SUNSET_MAX && lux >= LUX_NIGHT_MAX) {
    trackToPlay = TRACK_SPRING_EVENING_CHILL;
  } else if (temp < TEMP_MILD_DAY_THRESHOLD && pressure <= PRESSURE_MILD && hum < HUMIDITY_HIGH) {
    trackToPlay = TRACK_MILD_DAY_BREEZE;
  } else if (temp < TEMP_HOT_MIN && pressure < PRESSURE_LOW && hum > HUMIDITY_HIGH) {
    trackToPlay = TRACK_HUMID_STORM_BUILD;
  } else if (temp >= TEMP_WARM_MIN && temp < TEMP_HOT_MIN && lux >= LUX_SUNSET_MAX && lux < LUX_DAY_MAX) {
    trackToPlay = TRACK_WARM_MORNING_BIRDS;
  } else if (temp >= TEMP_HOT_MIN && lux > LUX_DAY_MAX) {
    trackToPlay = TRACK_SUMMER_CICADAS;
  } else if (temp >= TEMP_HOT_MIN && lux < LUX_SUNSET_MAX) {
    trackToPlay = TRACK_SUMMER_NIGHT_CRICKETS;
  }

  playTrack(trackToPlay);
}
