#define USE_LEDS

#ifdef USE_LEDS
#include <FastLED.h>
#endif

#include <Wire.h>
#include "Adafruit_SGP40.h"

#ifdef USE_LEDS
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    17
#define DATA_PIN    1
#define BRIGHTNESS  128

CRGB leds[NUM_LEDS];
#endif

Adafruit_SGP40 sgp;


void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); } // Wait for serial console to open!

  Serial.println("Initializing SGP40...");

  
  if (!sgp.begin()){
    Serial.println("Sensor not found :(");
    while (1);
  }
  Serial.print("Found SGP40 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);

#ifdef USE_LEDS
  Serial.println("Initializing LEDs...");
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 700);
  FastLED.clear();
  FastLED.show();
  Serial.println("LEDs initialized");
#endif
}

int counter = 0;
void loop() {
#ifdef USE_LEDS
  leds[0] = CRGB(255, 0, 0);
  leds[1] = CRGB(0, 255, 0);
  leds[2] = CRGB(0, 0, 255);
  leds[3] = CRGB::White;
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.show();
#endif

  int32_t raw = sgp.measureVocIndex();

  Serial.print("Measurement: ");
  Serial.println(raw);
  delay(1000);
}