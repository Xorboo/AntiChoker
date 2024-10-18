#include "mqtt_sender.h"

#include <WiFi.h>
#include <PubSubClient.h>

#ifndef WIFI_SSID
#define WIFI_SSID "no_wifi_ssid"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "no_wifi_password"
#endif

#ifndef MQTT_SERVER
#define MQTT_SERVER "localhost"
#endif

#ifndef MQTT_PORT
#define MQTT_PORT 1883
#endif

#ifndef MQTT_ANONYMOUS
#define MQTT_ANONYMOUS 1
#endif

#ifndef MQTT_USER
#define MQTT_USER "no_mqtt_user"
#endif

#ifndef MQTT_PASSWORD
#define MQTT_PASSWORD "no_mqtt_password"
#endif

// MQTT topic
const char *topic = "sensor/air_quality";

WiFiClient espClient;
PubSubClient client(espClient);

void setupWifi();
void checkWifi();
void waitForWifi();
void reconnectMqtt();

void setupMqtt()
{
    setupWifi();

    Serial.printf("Connecting to MQTT: %s:%d\n", MQTT_SERVER, MQTT_PORT);
    client.setServer(MQTT_SERVER, MQTT_PORT);
}

void sendData(int32_t aitQuality)
{
    checkWifi();

    if (!client.connected())
    {
        reconnectMqtt();
    }
    client.loop();

    char msg[50];
    snprintf(msg, 50, "airQuality=%d", aitQuality);
    client.publish(topic, msg);
}

void setupWifi()
{
    Serial.printf("Connecting to WiFi: %s\n", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    waitForWifi();
}

void checkWifi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi connection lost. Reconnecting...");
        WiFi.disconnect();
        WiFi.reconnect();
        waitForWifi();
    }
}

void waitForWifi()
{
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.printf("\nWiFi connected, IP address: %s\n", WiFi.localIP().toString().c_str());
}

void reconnectMqtt()
{
    while (!client.connected())
    {
        Serial.println("Attempting MQTT connection...");
#if MQTT_ANONYMOUS
        boolean connected = client.connect("ESP32_Air_Quality");
#else
        boolean connected = client.connect("ESP32_Air_Quality", MQTT_USER, MQTT_PASSWORD);
#endif

        if (connected)
        {
            Serial.println("Connected to MQTT");
        }
        else
        {
            Serial.printf("MQTT connection failed, rc=%d, retry in 5 seconds\n", client.state());
            delay(5000);
        }
    }
}
