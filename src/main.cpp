#define COLUMN
// #define SLICE

#include <config.h>
#include <sensor.h>
#include <leds.h>

#if defined(SLICE)
  Sensor sensor;
#endif


void setup()
{
  Serial.begin(9600);

  // sensor.setup();
  ledsSetup();
}

void loop()
{
  #if defined(SLICE)
    sensor.measure();
    tick_slice(sensor.level);
  #endif
  #if defined(COLUMN)
    tick_column();
  #endif
}