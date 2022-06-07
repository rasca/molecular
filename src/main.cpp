#include <sensor.h>
#include <leds.h>

void setup()
{
  Serial.begin(9600);

  ledsSetup();
}

void loop()
{
  measure();
  tick();
}