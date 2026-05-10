#pragma once

#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspkernel.h>
#include <psppower.h>
#include <me-core-mapper/me-core.h>

#include <cmath>

#define Q_FORMAT 23
#define F2Q(v) ((int)((v) * (1u << Q_FORMAT)))
#define Q2F(v) ((float)(int)(v) / (1u << Q_FORMAT))

void getB (float C, u32* const b0, u32* const b1) {
  
  const float f1 = (-1.0f + sqrtf(1.0f + 4.0f * C)) * 0.5f;
  const float f2 = C / f1 - 1.0f;
  *b0 = F2Q(f1);
  *b1 = F2Q(f2);
}
