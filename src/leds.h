#ifndef leds_H_INCLUDED
#define leds_H_INCLUDED

#include <config.h>
#include <utils.h>

///// LEDS /////
#include <sensor.h>
#include <palette.h>

#include <FastLED.h>
#include <array>

// std::array<std::array<CRGB, NUM_LEDS>, NUM_SEGMENTS> segments;
CRGB leds[NUM_LEDS * NUM_SEGMENTS];
int hue = 0;
unsigned long startTime = 0;
unsigned long waitTime = 1200;
bool waiting = true;
int duration = 15;
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
  waiting = true;

}

void tick_slice(int level)
{

  FastLED.clear();
  for (int i=0; i<NUM_SEGMENTS; i++)
  {
    for (int j = NUM_LEDS * i; j < NUM_LEDS * (i + 1); j++)
    {
      // leds[j] = scale8(CRGB(palette[i]), dendrite_levels[i]);
      leds[j] = CRGB(palette[i]).nscale8(dendrite_levels[i]);
    }
  }

  double probability  = 0.01 + level * .8 / 100;
  if ((double)rand()/RAND_MAX < probability)
  {
    for (int i = NUM_LEDS * dendrite; i < NUM_LEDS * (dendrite + 1); i++)
    {
      leds[i] = palette[dendrite];
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

#define TAIL_NUM 4
int pos = 0;

bool paint(int pos, CRGB color, double intensity)
{
  // check if we are in a valid range
  if (pos > 0 && pos < NUM_LEDS / 2)
  {
    // the string is folded in two
    // paint half of the string
    leds[pos].r = color.r * intensity;
    leds[pos].g = color.g * intensity;
    leds[pos].b = color.b * intensity;

    int mirror_pos = NUM_LEDS - 1 - pos;
    // paint the other half
    leds[mirror_pos].r = color.r * intensity;
    leds[mirror_pos].g = color.g * intensity;
    leds[mirror_pos].b = color.b * intensity;
  }
}

void tick_column()
{
  // Serial.print("startTime: ");
  // Serial.print(startTime);
  // Serial.print(" millis(): ");
  // Serial.print(millis());
  // Serial.print(" waitTime: ");
  // Serial.print(waitTime);
  // Serial.print(" waiting: ");
  // Serial.print(waiting);
  // Serial.print(" millis-startTime: ");
  // Serial.print(millis() - startTime);
  // Serial.println();
  if (waiting) {
    if (millis() - startTime < waitTime)
    {
      return;
    } else {
      waiting = false;
      startTime = millis();
    }
  }

  FastLED.clear();
  CRGB color = CRGB(255, 75, 75);
  paint(pos, color, 1);

  for (int i = 1 ; i < TAIL_NUM; i++)
  {
    paint(pos - i, color, pow(.3, i));
    paint(pos + i, color, pow(.3, i));
  }
  FastLED.show();

  if (millis() - startTime > duration)
  {
    // cicle between NUM_LEDS+TAIL_NUM..-TAIL_NUM
    pos--;
    startTime = millis();
    if (pos < -TAIL_NUM) 
    {
      pos = NUM_LEDS / 2 + TAIL_NUM;
      restart();
    }
  }

}

#endif