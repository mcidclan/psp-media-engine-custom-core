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

#include <pspdebug.h>
#include <psppower.h>
#include <pspctrl.h>
#include <me-core-mapper/me-core.h>

PSP_MODULE_INFO("me-core-demo", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

meLibSetSharedUncached32(3);
#define meExit       (meLibSharedMemory[0])
#define clockBuses   (meLibSharedMemory[1])
#define sp           (meLibSharedMemory[2])

__attribute__((noinline, aligned(4)))
void meLibOnProcess(void) {
  // meCoreBusClockPreserve poc
  HW_SYS_BUS_CLOCK_ENABLE = -1;
  meLibSync();
  meCoreBusClockPreserve(0x0f);
  
  // retrieve sp register's value, set to 2MB (old gen) / 4MB (new gen)
  asm volatile(
     "move      %0, $sp\n"
    : "=r" (sp) : :
  );
  
  // clockBuses value for as a meCoreBusClockPreserve proof
  clockBuses = HW_SYS_BUS_CLOCK_ENABLE;
  
  meExit = 1;
  meLibHalt();
}

void exiting() {
  pspDebugScreenPrintf("exiting...\n");
  sceKernelDelayThread(3000000);
  sceKernelExitGame();
}

int main() {
  pspDebugScreenInit();
  pspDebugScreenSetXY(0, 0);
  pspDebugScreenPrintf("Me Core Poc\n");

  const int tableId = meLibDefaultInit();
  
  if (
    (tableId != ME_CORE_T2_IMG_TABLE) &&
    (tableId != ME_CORE_IMG_TABLE)
  ) {
    pspDebugScreenPrintf("table not available: %d   \n", tableId);
    exiting();
  }
  
  if (tableId < 0) {
    pspDebugScreenPrintf("error: %d   \n", tableId);
    exiting();
  }
  
  SceCtrlData ctl;
  u32 count = 0;
  do {
    pspDebugScreenSetXY(0, 1);
    pspDebugScreenPrintf("clock buses enabled: 0x%08lx   \n", clockBuses);
    pspDebugScreenPrintf("table Id: %i   \n", tableId);
    pspDebugScreenPrintf("sp register: 0x%08lx   \n", sp);
    sceCtrlPeekBufferPositive(&ctl, 1);
    
    if (meExit) {
      count++;
      if (count >= 30) {
        break;
      }
      sceKernelDelayThread(100000);
    }
    
  } while (!(ctl.Buttons & PSP_CTRL_HOME));
  
  exiting();
  
  return 0;
}
