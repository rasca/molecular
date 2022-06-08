#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <config.h>

#include <algorithm>
// #include <iterator>
// #include <functional>

int clamp(int x, int upper, int lower)
{
    return min(upper, max(x, lower));
}


int wrap(int pre) {
  if (pre < 0) {
    pre = pre + NUM_LEDS;
  } else if (pre > NUM_LEDS) {
    pre = pre - NUM_LEDS;
  }
  return pre;
}

#endif