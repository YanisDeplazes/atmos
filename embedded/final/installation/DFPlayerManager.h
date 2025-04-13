// ============================================================================
// File: DFPlayerManager.h
// Author: Yanis Deplazes
// License: MIT License
// Copyright (c) 2025 Yanis Deplazes
// Description: Header file for DFPlayer Mini logic: initialization, playback,
//              volume control, and track selection based on sensor data.
// ============================================================================

#pragma once
#include <Arduino.h>

#define DFPLAYER_VOLUME_MIN 0
#define DFPLAYER_VOLUME_MAX 30
#define DFPLAYER_VOLUME_INIT 20
#define DFPLAYER_BAUDRATE 9600
#define TRACK_RAINSTORM 1
#define TRACK_BLIZZARD_WIND 2
#define TRACK_COLD_WINTER_DAY 3
#define TRACK_COLD_SUNSET_WIND 4
#define TRACK_MILD_STORM_WIND 5
#define TRACK_SPRING_MORNING_BIRDS 6
#define TRACK_MILD_DAY_BREEZE 7
#define TRACK_SPRING_EVENING_CHILL 8
#define TRACK_CALM_NIGHT_CRICKETS 9
#define TRACK_HUMID_STORM_BUILD 10
#define TRACK_WARM_MORNING_BIRDS 11
#define TRACK_SUMMER_CICADAS 12
#define TRACK_SUMMER_NIGHT_CRICKETS 13
#define TRACK_DEFAULT TRACK_SPRING_MORNING_BIRDS
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


/**
 * Initializes the DFPlayer Mini over the given HardwareSerial interface.
 */
void initDFPlayer(HardwareSerial* serial, int rx, int tx);

/**
 * Sets volume within defined range.
  */
void setVolume(uint8_t vol);

/**
 * Increases volume by one step.
 */
void volumeUp();

/**
 * Decreases volume by one step.
 */
void volumeDown();

/**
 * Plays the given track if it differs from the last played.
 */
void playTrack(uint8_t track);

/**
 * Evaluates environmental sensor data to determine which ambient track to play.
 */
void playBasedOnSensorData(float temp, float hum, float lux, float water, float pressure);
