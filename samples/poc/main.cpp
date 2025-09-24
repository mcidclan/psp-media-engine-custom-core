#include <pspdebug.h>
#include <psppower.h>
#include <pspctrl.h>
#include <me-core-mapper/me-core.h>

PSP_MODULE_INFO("me-core-demo", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

meLibSetSharedUncachedMem(3);
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

int main() {
  pspDebugScreenInit();
  const int tableId = meLibDefaultInit();
    
  pspDebugScreenSetXY(1, 1);
  pspDebugScreenPrintf("Me Core Poc");
  
  SceCtrlData ctl;
  do {
    pspDebugScreenSetXY(1, 2);
    pspDebugScreenPrintf("clock buses enabled: 0x%08x", clockBuses);
    pspDebugScreenSetXY(1, 3);
    pspDebugScreenPrintf("table Id: %i", tableId);
    pspDebugScreenSetXY(1, 4);
    pspDebugScreenPrintf("sp register: 0x%08x", sp);
    sceCtrlPeekBufferPositive(&ctl, 1);
  } while(!meExit && !(ctl.Buttons & PSP_CTRL_HOME));
  
  sceKernelDelayThread(1000000);
  sceKernelExitGame();
  
  return 0;
}
