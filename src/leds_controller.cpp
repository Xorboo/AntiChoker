#include "leds_controller.h"

#include <FastLED.h>

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define DATA_PIN 1
#define NUM_LEDS_REAL 17
#define NUM_LEDS_EXTRA 7
#define NUM_LEDS (NUM_LEDS_REAL + NUM_LEDS_EXTRA)

#define QUALITY_SMOOTH_256 32
#define QUALITY_MIN 100.0f
#define QUALITY_MAX 400.0f
#define QUALITY_MIN_SPEED 2
#define QUALITY_MAX_SPEED 12

// Number of waves visible on the strip at the same time
#define WAVES_COUNT 1
#define WAVE_GAMMA 4
#define BRIGHTNESS_MIN 3

#define COLOR_NONE CRGB::Black
#define COLOR_ERROR CRGB::Red
#define COLOR_BOOT CRGB::Yellow
DEFINE_GRADIENT_PALETTE(QualityColorPalette){
    0, 0, 255, 0,
    64, 127, 255, 0,
    128, 255, 255, 0,
    192, 255, 127, 0,
    255, 255, 0, 0};
// Final palette color is at index 240 and not 255, so we have to adjust for that
#define PALETTE_MAX_INDEX 240

bool ledsInitialized = false;

bool airQualityLoaded = false;
CRGB leds[NUM_LEDS];
uint8_t pivotIndex = 0;
CRGBPalette16 qualityColorPalette = QualityColorPalette;
uint32_t wavePosition = 0;
uint16_t displayAirQuality = 0;

void renderError();
void renderBoot();
void renderQuality(int32_t airQuality);

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

    pivotIndex = addmod8(pivotIndex, 1, NUM_LEDS);
    if (airQuality < 0)
    {
        renderError();
    }
    else if (!airQualityLoaded)
    {
        if (airQuality >= 80)
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
    fadeLightBy(leds, NUM_LEDS, 64);
    leds[pivotIndex] = COLOR_BOOT;
}

void renderQuality(int32_t airQuality)
{
    displayAirQuality = lerp16by8(displayAirQuality, airQuality, QUALITY_SMOOTH_256);
    float factor = constrain((float)(displayAirQuality - QUALITY_MIN) / (QUALITY_MAX - QUALITY_MIN), 0.0f, 1.0f);
    uint8_t index = factor * PALETTE_MAX_INDEX;
    CRGB qualityColor = ColorFromPalette(qualityColorPalette, index);
    fill_solid(leds, NUM_LEDS, qualityColor);

    wavePosition += lerp8by8(QUALITY_MIN_SPEED, QUALITY_MAX_SPEED, 255 * factor);
    uint8_t ledShiftAmount = WAVES_COUNT * (255 / NUM_LEDS);
    for (int i = 0; i < NUM_LEDS; i++)
    {
        uint8_t waveIndex = (wavePosition + i * ledShiftAmount) & 0xFF;
        uint8_t brightness = applyGamma_video(sin8(waveIndex), WAVE_GAMMA);
        leds[i].nscale8_video(max((uint8_t)BRIGHTNESS_MIN, brightness));
    }
}