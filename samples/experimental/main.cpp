#include <pspdebug.h>
#include <psppower.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <me-core-mapper/me-core.h>

// Experimental sample

PSP_MODULE_INFO("me-dsp-exp", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

meLibSetSharedUncachedMem(2);
#define meExit       (meLibSharedMemory[0])
#define meCounter    (meLibSharedMemory[1])

__attribute__((noinline, aligned(4)))
void meLibOnProcess(void) {
  do {
    meCounter++;
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
  pspDebugScreenPrintf("Me DSP/VME Experimental");
  
  SceCtrlData ctl;
  do {
    sceCtrlPeekBufferPositive(&ctl, 1);
    pspDebugScreenSetXY(1, 2);
    pspDebugScreenPrintf("counter: %x", meCounter);
    sceDisplayWaitVblank();
  } while (!(ctl.Buttons & PSP_CTRL_HOME));
  
  meWaitExit();
  
  sceKernelDelayThread(1000000);
  sceKernelExitGame();
  
  return 0;
}
