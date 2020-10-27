#include "sensor.h"
#include <ArduinoJson.h>

Sensor::Sensor() {}
Sensor::Sensor(String mqtt_channel_temperatur) : mqtt_channel_(mqtt_channel_temperatur) {}

void Sensor::SetMqttChannel(String mqtt_channel)
{
    mqtt_channel_ = mqtt_channel;
}

void Sensor::ReadDataFromSensor(byte *payload, unsigned int length)
{
    Serial.println("Neue Nachricht im Testchannel. Die Nachricht lautet:");
    Serial.println((char *)payload);
    //Serial.println(length);
    temperatur_ = JSONtoTemperatur(payload);
    Serial.print("Am Sensor: ");
    Serial.print("");
    Serial.print(" beträgt die Temperatur: ");
    Serial.print(temperatur_);
    Serial.println(" Grad.");
}

float Sensor::JSONtoTemperatur(byte *JSOnString)
{
    Serial.println("Dekodiere Nachricht");
    StaticJsonDocument<200> doc;
    auto error = deserializeJson(doc, (char *)JSOnString);
    if (error)
    {
        Serial.println("parseObject() failed");
        return 0;
    }
    float Temperatur = doc["AM2301"]["Temperature"];
    return Temperatur;
}