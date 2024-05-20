#ifndef MQTT_SENDER_H
#define MQTT_SENDER_H

#include <Arduino.h>

void setupMqtt();
void sendData(int32_t aitQuality);

#endif
