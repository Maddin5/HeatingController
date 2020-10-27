/*Implementing of an interface to read data from a sensor*/
#pragma once
#include <Arduino.h>

class Sensor
{
private:
    String sensor_position;
    String mqtt_channel_;
    float temperatur_;

public:
    Sensor();
    Sensor(String mqtt_channel); //Default Constructor

    /*Set the mqtt channel*/
    void SetMqttChannel(String mqtt_channel);

    /*Reads and than saves the incoming values of a sensor*/
    void ReadDataFromSensor(byte *payload, unsigned int length);

    /*Transforms Incoming JSON Massage to float temperatur value*/
    float JSONtoTemperatur(byte *JSOnString);
};