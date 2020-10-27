/* This class implemnts a physical room, which can contain sensors, regulators and heating elements. Each room than reports back it'S set values and the current values as well */

#pragma once
#include <Arduino.h>

class Raum
{
private:
    double ist_temperatur = 99;
    double soll_temperatur = 99;
    double stellventilposition = 500;

public:
    Raum(); //Default constructor

    void ReadDataFromSensor();
};