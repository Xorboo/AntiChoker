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

#define QUALITY_MIN 100.0f
#define QUALITY_MAX 300.0f
DEFINE_GRADIENT_PALETTE(QualityPalette){
    0, 0, 255, 0,
    64, 127, 255, 0,
    128, 255, 255, 0,
    192, 255, 127, 0,
    255, 255, 0, 0};

bool ledsInitialized = false;

bool airQualityLoaded = false;
CRGB leds[NUM_LEDS];
uint16_t pivotIndex = 0;
CRGBPalette16 palette = QualityPalette;
uint32_t wavePosition = 0;
int32_t displayAirQuality = 0;

void renderError();
void renderBoot();
void renderQuality(int32_t airQuality);

float saturate(float value)
{
    return max(0.0f, min(value, 1.0f));
}

bool setupLeds()
{
    Serial.println("Initializing LEDs...");
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS_REAL)
        .setCorrection(TypicalLEDStrip);
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
        if (airQuality >= 80 )
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
    fill_solid(leds, NUM_LEDS, COLOR_NONE);
    leds[pivotIndex] = COLOR_ERROR;
}

void renderBoot()
{
    fadeToBlackBy(leds, NUM_LEDS, 64);
    leds[pivotIndex] = COLOR_BOOT;
}

uint8_t scalePow(uint8_t value, uint8_t n)
{
    float floatValue = value / 255.0f;
    return uint8_t(pow(floatValue, n) * 255);
}

void renderQuality(int32_t airQuality)
{
    displayAirQuality = displayAirQuality * 0.9f + airQuality * 0.1f;
    float factor = saturate((displayAirQuality - QUALITY_MIN) / (QUALITY_MAX - QUALITY_MIN));
    uint8_t index = (uint8_t)(factor * 240); // Some bs colors after 240 in the palette
    CRGB qualityColor = ColorFromPalette(palette, index);
    fill_solid(leds, NUM_LEDS, qualityColor);

    wavePosition += 2 * map(index, 0, 240, 100, 500) / 100.0f;
    for (int i = 0; i < NUM_LEDS; i++)
    {
        int waveIndex = (wavePosition + i * 16) % 255;
        uint8_t scaledBrightness = scalePow(sin8(waveIndex), 5);
        uint8_t mappedBrightness = map(scaledBrightness, 0, 255, 5, 255);

        leds[i] = qualityColor;
        leds[i].fadeToBlackBy(255 - mappedBrightness);
    }
}