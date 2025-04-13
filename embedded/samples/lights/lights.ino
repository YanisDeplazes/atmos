#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    144
#define BRIGHTNESS  10
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  static uint8_t baseHue = 0;

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(baseHue + i, 255, 255); // Hue shifts along the strip
  }

  FastLED.show();
  baseHue++; // animate the gradient over time
  delay(20); // adjust speed here
}
