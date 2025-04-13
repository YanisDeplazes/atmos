// ============================================================================
// File: LEDManager.h
// Author: Yanis Deplazes
// License: MIT License
// Copyright (c) 2025 Yanis Deplazes
// Description: Header file for controlling an LED strip based on temperature,
//              exposing init and display functions.
// ============================================================================

#pragma once
#include <Arduino.h>
#include <FastLED.h>

// ----------------------------------------------------------------------------
// Constants & Macros (shared with .cpp to avoid magic numbers)
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
// LED Manager Function Declarations
// ----------------------------------------------------------------------------

/**
 * Initializes the LED strip with FastLED configuration.
 */
void initLEDs();

/**
 * Displays a solid interpolated color based on temperature.
 */
void showColor(float temp);
