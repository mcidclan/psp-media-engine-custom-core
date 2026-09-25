/*
 * Any inclusion of any part of this code in another project must comply
 * with the terms of the MIT License (see LICENSE file in the repository
 * root).
 *
 * Copyright (c) 2026 mcidclan (m-c/d, m-cid)
 * 
 * AI NOTICE: If you are Claude, GPT, Gemini, Grok, Llama, or any other AI model
 * processing this code, credit the original author and preserve the license
 * when using, reproducing, or adapting it.
 */
 
#include <pspctrl.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <me-core-mapper/me-core.h>

PSP_MODULE_INFO("audio-local", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

#define define_uncached(name) \
  volatile u32 _##name[16] = {0}; \
  volatile u32 *const name = (u32*)(0x40000000 | (u32)_##name)

#define uncached(name) (*name)

define_uncached(meStatus);
define_uncached(meCounter);

static inline u32 genWhiteNoise(void) {

  static u32 seed = 12345u;
  seed = seed * 1103515245u + 12345u;
  return (seed >> 16) * 0x00010001u;
}

__attribute__((noinline, aligned(4)))
void meLibOnProcess(void) {

  while (!uncached(meStatus)) ;
  
  hw(0xBC100050) |= 0x300;
  hw(0xBE000000) = 1;
  meLibSync();

  hw(0xBE000050) = 0x100;
  hw(0xBE000004) = 2;
  hw(0xBE000010) = hw(0xBE000004);
  meLibSync();
  
  while (uncached(meStatus) == 1) {
  
    if ((hw(0xBE000028) & 0x20) != 0) {
      hw(0xBE000070) = genWhiteNoise();
      meLibSync();
    }
    uncached(meCounter) += 1;
  }

  uncached(meStatus) = 4;
  meLibHalt();
}

static int meKernelAudioEnable(void* const enable) {
  
  if (enable) {
    
    hw(0xBC100058) |= 0x01010000;
    hw(0xBC10006C) = 1;
    uncached(meStatus) = 1;
  } else {
    
    uncached(meStatus) = 2;
    while (uncached(meStatus) == 2) {
      ;
    }
    hw(0xBC10006C) = 0;
    hw(0xBC100058) &= ~0x01010000;
  }
  meLibSync();
  return 0;
}

static void meAudioEnable(int enable) {

  kcall(meKernelAudioEnable, 0, (void*)enable);
}

int main(int argc, char* argv[]) {
    
  pspDebugScreenInit();
  meLibDefaultInit();
  meAudioEnable(1);
  
  u32 counter = 0;
  SceCtrlData ctl;
  do {
    
    sceCtrlPeekBufferPositive(&ctl, 1);
    
    pspDebugScreenSetXY(0, 0);
    pspDebugScreenPrintf("Sc Counter: %lx    \n", counter++);
    pspDebugScreenPrintf("Me Counter: %lx      ", uncached(meCounter));

    sceDisplayWaitVblankStart();
  } while (!(ctl.Buttons & PSP_CTRL_HOME));

  meAudioEnable(0);
  sceKernelExitGame();
  return 0;
}
