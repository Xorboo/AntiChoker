#include <FastLED.h>


#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    17
#define DATA_PIN    1
#define BRIGHTNESS  128


CRGB leds[NUM_LEDS];


bool setupLeds() {
  Serial.println("Initializing LEDs...");
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 700);
  FastLED.clear();
  FastLED.show();
  Serial.println("LEDs initialized");
  return true;
}

bool renderLeds() {
  if (!sgpInitialized) {
    Serial.print("ERROR: Can't render LEDs before initialization");
    return 0;
  }

  leds[0] = CRGB(255, 0, 0);
  leds[1] = CRGB(0, 255, 0);
  leds[2] = CRGB(0, 0, 255);
  leds[3] = CRGB::White;
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.show();
}