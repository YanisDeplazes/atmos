// ============================================================================
// File: LEDManager.cpp
// Author: Yanis Deplazes
// License: MIT License
// Copyright (c) 2025 Yanis Deplazes
// Description: Controls LED strip color based on temperature and humidity,
//              using color interpolation with FastLED.
// ============================================================================

#include "LEDManager.h"
#include <FastLED.h>

// ----------------------------------------------------------------------------
// Constants & Macros
// ----------------------------------------------------------------------------
#define LED_PIN 2
#define NUM_LEDS 144
#define LED_BRIGHTNESS 10
#define LED_TYPE WS2812B
#define LED_COLOR_ORDER GRB

#define TEMP_COLOR_MIN -10.0f
#define TEMP_COLOR_MAX 50.0f
#define TEMP_COLOR_THRESHOLD 18.0f
#define COLOR_LERP_MIN 0.0f
#define COLOR_LERP_MAX 1.0f
#define COLOR_LERP_SCALE 255

#define COLOR_COLD_MIN CRGB(7, 14, 136)   // Deep Blue
#define COLOR_COLD_MAX CRGB(0, 247, 255)  // Cyan
#define COLOR_WARM_MIN CRGB(255, 217, 0)  // Yellow
#define COLOR_WARM_MAX CRGB(255, 0, 171)  // Magenta

#define LOOP_START_INDEX 0

// ----------------------------------------------------------------------------
// LED Strip State
// ----------------------------------------------------------------------------
static CRGB leds[NUM_LEDS];
static uint16_t gradientOffset = 0;
static uint32_t lastUpdate = 0;



// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

/**
 * initLEDs()
 * ----------
 * Initializes the FastLED strip with predefined configuration.
 */
void initLEDs() {
  FastLED.addLeds<LED_TYPE, LED_PIN, LED_COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(LED_BRIGHTNESS);
}

/**
 * interpolateColor()
 * ------------------
 * Linearly interpolates between two CRGB colors based on a normalized value.
 *
 * @param value The value used for interpolation.
 * @param min Minimum expected value.
 * @param max Maximum expected value.
 * @param low Color for the minimum value.
 * @param high Color for the maximum value.
 * @return Interpolated color as CRGB.
 */
CRGB interpolateColor(float value, float min, float max, const CRGB& low, const CRGB& high) {
  float ratio = constrain((value - min) / (max - min), COLOR_LERP_MIN, COLOR_LERP_MAX);
  uint8_t r = lerp8by8(low.r, high.r, ratio * COLOR_LERP_SCALE);
  uint8_t g = lerp8by8(low.g, high.g, ratio * COLOR_LERP_SCALE);
  uint8_t b = lerp8by8(low.b, high.b, ratio * COLOR_LERP_SCALE);
  return CRGB(r, g, b);
}

/**
 * showColor()
 * -----------
 * Displays a solid color on the LED strip based on current temperature.
 *
 * @param temp Temperature value to influence LED color.
 */
void showColor(float temp) {
  CRGB color = (temp >= TEMP_COLOR_THRESHOLD)
                 ? interpolateColor(temp, TEMP_COLOR_THRESHOLD, TEMP_COLOR_MAX, COLOR_WARM_MIN, COLOR_WARM_MAX)
                 : interpolateColor(temp, TEMP_COLOR_MIN, TEMP_COLOR_THRESHOLD, COLOR_COLD_MIN, COLOR_COLD_MAX);

  for (int i = LOOP_START_INDEX; i < NUM_LEDS; i++) {
    leds[i] = color;
  }

  FastLED.show();

  Serial.printf("Static Temp Color → R: %d, G: %d, B: %d\n", color.r, color.g, color.b);
}
