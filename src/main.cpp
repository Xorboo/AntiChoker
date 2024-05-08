#include <Arduino.h>
#include "sgp40_reader.h"
#include "leds_controller.h"

TaskHandle_t SgpTaskHandle;
TaskHandle_t LedTaskHandle;

void runMeasurement(void *pvParameters);
void runLeds(void *pvParameters);

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(10);
    }

    xTaskCreatePinnedToCore(runMeasurement, "SgpTask", 10000, NULL, 1, &SgpTaskHandle, 0);
    xTaskCreatePinnedToCore(runLeds, "LedTask", 10000, NULL, 1, &LedTaskHandle, 1);
}

void loop() {}

void runMeasurement(void *pvParameters)
{
    while (true)
    {
        if (!sgpInitialized)
        {
            setupSgp();
        }
        else
        {
            getAirQuality();
        }

        delay(1000);
    }
}

void runLeds(void *pvParameters)
{
    setupLeds();

    while (true)
    {
        renderLeds(measuredAirQuality);
        delay(50);
    }
}