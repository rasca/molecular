#ifndef leds_H_INCLUDED
#define leds_H_INCLUDED

///// LEDS /////
#include <sensor.h>

#define NUM_LEDS 50
#define NUM_SEGMENTS 7

#include <FastLED.h>
#include <array>

// std::array<std::array<CRGB, NUM_LEDS>, NUM_SEGMENTS> segments;
CRGB leds[NUM_LEDS * NUM_SEGMENTS];
int hue = 0;
unsigned long startTime = 0;
int duration = 1000;
double dendrite_levels[NUM_SEGMENTS];
double dendrite_increase = 20;
double dendrite_decrease = .9;

int dendrite = 0;

void ledsSetup()
{
  Serial.begin(9600);
  // LEDS
  FastLED.addLeds<WS2812, GPIO_NUM_15>(leds, NUM_LEDS * NUM_SEGMENTS);
  FastLED.setBrightness(255);

  startTime = millis();
  for (int i=0; i<NUM_SEGMENTS; i++)
  {
    dendrite_levels[i] = 0;
  }
}

void restart()
{
  startTime = millis();
}

void tick(int level)
{

  FastLED.clear();
  for (int i=0; i<NUM_SEGMENTS; i++)
  {
    for (int j = NUM_LEDS * i; j < NUM_LEDS * (i + 1); j++)
    {
      leds[j] = CHSV(hue % 255, 255, dendrite_levels[i]);
    }
  }

  double probability  = 0.01 + level * .8 / 100;
  if ((double)rand()/RAND_MAX < probability)
  {
    for (int i = NUM_LEDS * dendrite; i < NUM_LEDS * (dendrite + 1); i++)
    {
      leds[i] = CHSV(hue % 255, 255, 255);
      // leds[i] = CHSV(hue % 255, 255, level * 255 / 100);
    }
    dendrite_levels[dendrite] = min(255.0, dendrite_increase + dendrite_levels[dendrite]);
    dendrite = rand() % NUM_SEGMENTS;
  }

  for (int i=0; i<NUM_SEGMENTS; i++)
  {
    dendrite_levels[i] = max(0.0, dendrite_levels[i] * (dendrite_decrease + (level - 50) / 500.0));
  }

  if (millis() - startTime > duration)
  {
    restart();
  }

  FastLED.show();
}

#endif