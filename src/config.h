#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define NUM_LEDS 50
#if defined(SLICE)
  #define NUM_SEGMENTS 7
#endif
#if defined(COLUMN)
  #define NUM_SEGMENTS 1
#endif

/*
int programs_current = 0;
int programs_total = 5;
*/

#endif