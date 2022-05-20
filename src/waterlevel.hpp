#ifndef WATERLEVEL_H
#define WATERLEVEL_H

#include <WiFi.h>

namespace Waterlevel
{

    const int trigPin = 5;
    const int echoPin = 18;
// define sound speed in cm/uS
#define SOUND_SPEED 0.034
    // #define CM_TO_INCH 0.393701

    long duration;
    float distanceCm;

    float getWaterLevel()
    {
        // Clears the trigPin
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        // Sets the trigPin on HIGH state for 10 micro seconds
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        // Reads the echoPin, returns the sound wave travel time in microseconds
        duration = pulseIn(echoPin, HIGH);

        // Calculate the distance
        distanceCm = duration * SOUND_SPEED / 2;

        // Convert to inches
        // distanceInch = distanceCm * CM_TO_INCH;

        // Prints the distance in the Serial Monitor

        Serial.print("Distance : ");
        Serial.print(distanceCm);
        Serial.println(" cm");

        // delay(1000);
        return distanceCm;
    }
}

#endif