#ifndef WATERTEMPERATURE_H
#define WATERTEMPERATURE_H

#include <OneWire.h>
#include <DallasTemperature.h>

namespace Watertemperature
{ // GPIO where the DS18B20 is connected to
    const int oneWireBus = 4;

    // Setup a oneWire instance to communicate with any OneWire devices
    OneWire oneWire(oneWireBus);

    // Pass our oneWire reference to Dallas Temperature sensor
    DallasTemperature sensors(&oneWire);

}

#endif