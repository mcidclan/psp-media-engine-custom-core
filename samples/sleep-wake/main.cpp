#include <pspdebug.h>
#include <psppower.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <me-core-mapper/me-core.h>

PSP_MODULE_INFO("me-sleep", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

meLibSetSharedUncachedMem(2);
#define meExit       (meLibSharedMemory[0])
#define meCounter    (meLibSharedMemory[1])

ME_LIB_SETUP_DEFAULT_SUSPEND_HANDLER();

__attribute__((noinline, aligned(4)))
void meLibOnProcess(void) {
  meLibExceptionHandlerInit(0);
  do {
    meCounter++;
  } while(meExit == 0);
  meExit = 2;
  meLibHalt();
}

static void meWaitExit() {
  meExit = 1;
  u8 retry = 0;
  do {
    sceKernelDelayThread(100000);
  } while (meExit < 2 && ++retry <= 5);
}

int main() {
  pspDebugScreenInit();
  meLibDefaultInit();
    
  pspDebugScreenSetXY(1, 1);
  pspDebugScreenPrintf("Me Sleep");
  
  SceCtrlData ctl;
  u32Me scCounter = 0;
  do {
    sceCtrlPeekBufferPositive(&ctl, 1);
    pspDebugScreenSetXY(1, 2);
    pspDebugScreenPrintf("Me counter: %x", meCounter);
    pspDebugScreenSetXY(1, 3);
    pspDebugScreenPrintf("Sc counter: %x", scCounter++);
    pspDebugScreenSetXY(1, 4);
    pspDebugScreenPrintf("Sc counter: %x", hw(0x40010000));
    sceDisplayWaitVblank();
  } while (!(ctl.Buttons & PSP_CTRL_HOME));
  
  meWaitExit();  
  sceKernelExitGame();
  
  return 0;
}
