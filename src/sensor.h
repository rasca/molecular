#ifndef SENSOR_H_INCLUDED
#define SENSOR_H_INCLUDED

#include <math.h>

///// Sensor /////
#include <NewPing.h>

#define TRIGGER_PIN 12
#define ECHO_PIN 14
#define MAX_DISTANCE 200
#define MIN_DISTANCE 16

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

class Sensor
{
public:
    double measurament;
    double filtered = 0;
    double level = 0;


    void setup()
    {
        startTime = millis();
        pinMode(LED_BUILTIN, OUTPUT);
    }

    void restart()
    {
        startTime = millis();
    }

    void tick()
    {
        if (millis() - startTime > duration)
        {
        measure();
        restart();
        }
    }

    int measure()
    {
        measurament = sonar.ping_cm();
        if (measurament > 0)
        {
            digitalWrite(LED_BUILTIN, HIGH);
        } else {
            digitalWrite(LED_BUILTIN, LOW);
        }

        // disregard first (zero_threshold)
        if (measurament == 0)
        {
            zeroes_count++;
        }
        else
        {
            zeroes_count = 0;
        }

        // clamp to MIN_DISTANCE
        if (measurament < MIN_DISTANCE && measurament > 0)
        {
            measurament = MIN_DISTANCE;
        }

        // if we have a measurement update the filter
        if (zeroes_count == 0)
        {
            // use the measurement
            filtered = filtered * (1 - alpha) + measurament * alpha;
        }
        else if (zeroes_count > zeroes_threshold)
        // if we have enough zeros update the filter
        {
            // multiple zeros. reading should be MAX_DISTANCE
            filtered = filtered * (1 - alpha) + MAX_DISTANCE * alpha;
        }

        // perform ln(1/x) transformation
        // filtered is at least MIN_DISTANCE (no division by zero)
        level = multiplier * log(MAX_DISTANCE / filtered);

        if (level > 100)
        {
            level = 100;
        }

        printDebug();

        return level;
    }

private:
    ///// First order filter /////
    ///// and ignore first zeroes /////
    const double alpha = 0.05;
    const int zeroes_threshold = 15;
    int zeroes_count = 0;

    unsigned long startTime = 0;
    int duration = 60;

    ///// Higher output on closer distance /////
    ///// multiplier * ln(MAX_DISTANCE/x) ////
    ///// 40 * ln(200/cm) ////
    const double multiplier = 40; // good results for 2m = 100 at 16cm

    void printDebug()
    {
        Serial.print("measurament: ");
        Serial.print(measurament);
        Serial.print(" smooth: ");
        Serial.print(filtered);
        Serial.print(" level: ");
        Serial.println(level);
    }

};

#endif