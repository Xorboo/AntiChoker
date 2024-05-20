#ifndef USE_MQTT
#define USE_MQTT 1
#endif

#include <Arduino.h>
#include "sgp40_reader.h"
#include "leds_controller.h"
#if USE_MQTT
#include "mqtt_sender.h"
#endif

TaskHandle_t SgpTaskHandle;
TaskHandle_t MqttTaskHandle;
TaskHandle_t LedTaskHandle;

void runMeasurement(void *pvParameters);
void runLeds(void *pvParameters);
void runMqtt(void *pvParameters);

void setup()
{
    Serial.begin(115200);

    int32_t serialWait = 1000;
    while (!Serial && serialWait > 0)
    {
        serialWait -= 10;
        delay(10);
    }

    xTaskCreatePinnedToCore(runMeasurement, "SgpTask", 10000, NULL, 1, &SgpTaskHandle, 0);
    xTaskCreatePinnedToCore(runLeds, "LedTask", 10000, NULL, 1, &LedTaskHandle, 1);
#if USE_MQTT
    xTaskCreatePinnedToCore(runMqtt, "MqttTask", 10000, NULL, 1, &MqttTaskHandle, 0);
#endif
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

        delay(200);
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

#if USE_MQTT
void runMqtt(void *pvParameters)
{
    delay(5000);
    setupMqtt();

    while (true)
    {
        sendData(measuredAirQuality);
        delay(5 * 1000);
    }
}
#endif
