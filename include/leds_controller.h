#ifndef LEDS_CONTROLLER_H
#define LEDS_CONTROLLER_H

#include <Arduino.h>

extern bool ledsInitialized;

bool setupLeds();
void renderLeds(int32_t airQuality);

#endif
