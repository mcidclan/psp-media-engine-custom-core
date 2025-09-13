#include <pspdebug.h>
#include <psppower.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include "me-core.h"

PSP_MODULE_INFO("me-interrupt-sleep", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

extern "C" {
  extern volatile u32 proof[1];
}
#define proof0         (meLibMakeUncachedVar(proof, UNCACHED_USER_MASK)[0])

meLibSetSharedUncachedMem(1);
#define meExit       (meLibSharedMemory[0])
#define meCounter    (meLibSharedMemory[1])

__attribute__((noinline, aligned(4)))
void meLibExec(void) {
  meLibInitExceptions();
  do {
    meCounter++;
    // meLibHalt();
      
    //meLibDelayPipeline();
    //meLibSync();
  } while(meExit == 0);
  
  meExit = 2;
  meLibHalt();
}

static void meWaitExit() {
  meExit = 1;
  do {
    meLibDelayPipeline();
    meLibSync();
  } while (meExit < 2);
}

int main() {
  pspDebugScreenInit();
  meLibDefaultInit();
    
  pspDebugScreenSetXY(1, 1);
  pspDebugScreenPrintf("Me Interrupt Sleep");
  
  bool triangle = false;
  SceCtrlData ctl;
  do {
    sceCtrlPeekBufferPositive(&ctl, 1);
    pspDebugScreenSetXY(1, 2);
    pspDebugScreenPrintf("poc: %x", proof0);
    pspDebugScreenSetXY(1, 3);
    pspDebugScreenPrintf("counter: %x", meCounter);
    
    if (!triangle && (ctl.Buttons & PSP_CTRL_TRIANGLE)) {
      kcall(meCoreEmitSoftwareInterrupt);
      triangle = true;
    } else if(!(ctl.Buttons & PSP_CTRL_TRIANGLE)) {
      triangle = false;
    }
    
    sceDisplayWaitVblank();
  } while (!(ctl.Buttons & PSP_CTRL_HOME));
  
  // meWaitExit();
  
  sceKernelDelayThread(1000000);
  sceKernelExitGame();
  
  return 0;
}
