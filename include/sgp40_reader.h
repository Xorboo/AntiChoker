#ifndef SGP40_READER_H
#define SGP40_READER_H

#include <Arduino.h>

extern bool sgpInitialized;
extern int32_t measuredAirQuality;

bool setupSgp();
int32_t getAirQuality();

#endif
