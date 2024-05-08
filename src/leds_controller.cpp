#include "leds_controller.h"

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS_REAL 17
#define NUM_LEDS_EXTRA 7
#define NUM_LEDS (NUM_LEDS_REAL + NUM_LEDS_EXTRA)
#define DATA_PIN 1
#define BRIGHTNESS 128

#define COLOR_NONE CRGB::Black
#define COLOR_ERROR CRGB::Red
#define COLOR_BOOT CRGB::Yellow

#define BOOT_LENGTH 10

bool ledsInitialized = false;

bool airQualityLoaded = false;
CRGB leds[NUM_LEDS];
uint16_t pivotIndex = 0;

void renderError();
void renderBoot();
void renderQuality(int32_t airQuality);

bool setupLeds()
{
    Serial.println("Initializing LEDs...");
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 700);
    FastLED.clear();
    FastLED.show();
    Serial.println("LEDs initialized");
    ledsInitialized = true;
    return ledsInitialized;
}

void renderLeds(int32_t airQuality)
{
    if (!ledsInitialized)
    {
        Serial.print("ERROR: Can't render LEDs before initialization");
        return;
    }

    pivotIndex = (pivotIndex + 1) % NUM_LEDS;
    if (airQuality < 0)
    {
        renderError();
    }
    else if (!airQualityLoaded)
    {
        if (airQuality > 80)
        {
            airQualityLoaded = true;
        }
        renderBoot();
    }
    else
    {
        renderQuality(airQuality);
    }

    FastLED.show();
}

void renderError()
{
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = i == pivotIndex ? COLOR_ERROR : COLOR_NONE;
    }
}

void renderBoot()
{
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        float distance = pivotIndex - i;
        if (distance < 0 || distance > BOOT_LENGTH)
        {
            leds[i] = COLOR_NONE;
        }
        else
        {
            float value = (BOOT_LENGTH - distance) / BOOT_LENGTH;
            value *= value;
            CRGB bootColor = COLOR_BOOT;
            leds[i].r = (uint8_t)(bootColor.r * value);
            leds[i].g = (uint8_t)(bootColor.g * value);
            leds[i].b = (uint8_t)(bootColor.b * value);
        }
    }
}

void renderQuality(int32_t airQuality)
{
    int32_t fillAmount = NUM_LEDS * airQuality / 200;

    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = i <= fillAmount ? CRGB::Green : COLOR_NONE;
    }
}