#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <psppower.h>
#include <pspctrl.h>
#include "me-lib.h"
#include "kcall.h"

PSP_MODULE_INFO("me-core-hw-mutex", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

static volatile u32* mem = nullptr;
#define meExit           (mem[0])

void libMeExec() {
  // wait until mem is ready
  while (!mem) {
    meCoreDcacheWritebackInvalidateAll();
  }
  
  // todo
  
  meExit = 1;
  meLibHalt();
}

int main() {
  pspDebugScreenInit();
  const int tableId = meLibDefaultInit();
  
  meLibGetUncached32(&mem, 2);
  
  pspDebugScreenSetXY(1, 1);
  pspDebugScreenPrintf("Me Core Mutex Demo");
  
  SceCtrlData ctl;
  do {
    pspDebugScreenSetXY(1, 1);
    pspDebugScreenPrintf("table Id: %i", tableId);
    sceCtrlPeekBufferPositive(&ctl, 1);
  } while(!meExit && !(ctl.Buttons & PSP_CTRL_HOME));
  
  sceKernelDelayThread(1000000);
  sceKernelExitGame();
  
  return 0;
}
