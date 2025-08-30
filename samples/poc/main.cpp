#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <psppower.h>
#include <pspctrl.h>
#include "me-lib.h"

PSP_MODULE_INFO("me-core-demo", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

static volatile u32* mem = nullptr;
#define meExit           (mem[0])
#define clockBuses       (mem[1])
#define sp               (mem[2])

__attribute__((noinline, aligned(4)))
void meLibExec(void) {
  // meCoreBusClockPreserve poc
  HW_SYS_BUS_CLOCK_ENABLE = -1;
  meLibSync();
  meCoreBusClockPreserve(0x0f);
  
  // wait until mem is ready
  do {
    meCoreDcacheWritebackInvalidateAll();
  } while(!mem);
  
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

int main() {
  pspDebugScreenInit();
  const int tableId = meLibDefaultInit();
  
  meLibGetUncached32(&mem, 3);
  
  pspDebugScreenSetXY(1, 1);
  pspDebugScreenPrintf("Me Core Demo");
  
  SceCtrlData ctl;
  do {
    pspDebugScreenSetXY(1, 1);
    pspDebugScreenPrintf("clock buses enabled: 0x%08x", clockBuses);
    pspDebugScreenSetXY(1, 2);
    pspDebugScreenPrintf("table Id: %i", tableId);
    pspDebugScreenSetXY(1, 3);
    pspDebugScreenPrintf("sp register: 0x%08x", sp);
    sceCtrlPeekBufferPositive(&ctl, 1);
  } while(!meExit && !(ctl.Buttons & PSP_CTRL_HOME));
  
  sceKernelDelayThread(1000000);
  sceKernelExitGame();
  
  return 0;
}
