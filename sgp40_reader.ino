#include <Wire.h>
#include "Adafruit_SGP40.h"

Adafruit_SGP40 sgp;

bool setupSgp() {
  Serial.println("Initializing SGP40...");
  if (!sgp.begin()){
    Serial.println("ERROR: SGP40 Sensor not found");
    return false;
  }

  Serial.printf("Found SGP40 serial #%04X%04X%04X", sgp.serialnumber[0], sgp.serialnumber[1], sgp.serialnumber[2]);
  return true;
}

int32_t getAirQuality() {
  if (!sgpInitialized) {
    Serial.print("ERROR: Can't get air quality before initialization");
    return 0;
  }

  int32_t quality = sgp.measureVocIndex();
  Serial.printf("Measurement: %d\n", quality);
  return quality;
}