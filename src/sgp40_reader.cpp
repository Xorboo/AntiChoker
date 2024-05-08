#include "sgp40_reader.h"

#include <Wire.h>
#include "Adafruit_SGP40.h"

bool sgpInitialized = false;
int32_t measuredAirQuality = 0;

Adafruit_SGP40 sgp;

bool setupSgp()
{
    Serial.println("Initializing SGP40...");
    if (!sgp.begin())
    {
        Serial.println("ERROR: SGP40 Sensor not found");
        measuredAirQuality = -1;
        sgpInitialized = false;
        return sgpInitialized;
    }

    Serial.printf("Found SGP40 serial #%04X%04X%04X\n", sgp.serialnumber[0], sgp.serialnumber[1], sgp.serialnumber[2]);
    sgpInitialized = true;
    measuredAirQuality = 0;
    return sgpInitialized;
}

int32_t getAirQuality()
{
    if (!sgpInitialized)
    {
        Serial.print("ERROR: Can't get air quality before initialization");
        measuredAirQuality = -1;
        return measuredAirQuality;
    }

    int32_t quality = sgp.measureVocIndex();
    Serial.printf("Measurement: %d\n", quality);
    measuredAirQuality = quality;
    return measuredAirQuality;
}