// #define COLUMN
#define SLICE

#include <config.h>
#include <sensor.h>
#include <leds.h>
#include <comms.h>

#if defined(SLICE)
  Sensor sensor;
#endif


void setup()
{
  Serial.begin(9600);

  ledsSetup();

  #if defined(SLICE)
    sensor.setup();
  #endif

  #if defined(COLUMN)
    setupWifiServer();
  #endif

  #if defined(SLICE)
    // setupWifiClient();
  #endif
}

void loop()
{
  #if defined(SLICE)
    sensor.measure();
    // setLevel(sensor.level);
    tick_slice(sensor.level);
  #endif
  #if defined(COLUMN)
    tick_column();
  #endif
}