#include <sensor.h>
#include <leds.h>

Sensor sensor;

void setup()
{
  Serial.begin(9600);

  // sensor.setup();
  ledsSetup();
}

void loop()
{
  sensor.measure();
  tick(sensor.level);
}