#pragma once

//#include <cstring>

#include <pspgu.h>
#include <psppower.h>
#include <pspdisplay.h>
#include <pspsdk.h>
#include <pspkernel.h>
#include <cstring>
#include <malloc.h>
#include <pspdebug.h>
#include <pspctrl.h>

#include <me-core-mapper/me-core.h>
#include "gu.h"

template<typename T>
inline T xorshift() {
  static T state = 1;
  T x = state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  return state = x;
}

unsigned short int randInRange(const unsigned short int range) {
  unsigned short int x = xorshift<unsigned int>();
  unsigned int m = (unsigned int)x * (unsigned int)range;
  return (m >> 16);
}
