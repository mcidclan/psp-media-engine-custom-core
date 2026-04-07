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
    pspDebugScreenPrintf("clock buses enabled: 0x%08x   \n", clockBuses);
    pspDebugScreenPrintf("table Id: %i   \n", tableId);
    pspDebugScreenPrintf("sp register: 0x%08x   \n", sp);
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
