#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <psppower.h>
#include <pspctrl.h>
#include "me-lib.h"
#include "kcall.h"

PSP_MODULE_INFO("me-core-demo", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

static volatile u32* mem = nullptr;
#define meExit           (mem[0])
#define clockBuses       (mem[1])

extern char __start__me_section;
extern char __stop__me_section;
__attribute__((section("_me_section")))
void meHandler() {
  asm volatile(
    "li          $t0, 0x30000000\n"
    "mtc0        $t0, $12\n"
    "sync\n"
    : : : "t0"
  );
  
  HW_SYS_BUS_CLOCK_ENABLE      = -1;
  HW_SYS_NMI_FLAGS             = 0xffffffff;
  HW_SYS_TACHYON_CONFIG_STATUS = 0x02;
  meLibSync();
  
  meCoreBusClockFilterEnabled(0x0f);

  // Wait until mem is ready
  while (!mem) {
    meCoreDcacheWritebackInvalidateAll();
  }
  
  clockBuses = HW_SYS_BUS_CLOCK_ENABLE;
  
  meExit = 1;
  meLibHalt();
}

u32 version = 0;

int initMe() {
  const int tableId = meCoreGetTableIdFromWitnessWord();
  if (tableId < 2) {
    return -1;
  }
  meCoreSelectSystemTable(tableId);
  #define me_section_size (&__stop__me_section - &__start__me_section)
  memcpy((void *)ME_HANDLER_BASE, (void*)&__start__me_section, me_section_size);
  sceKernelDcacheWritebackInvalidateAll();
  HW_SYS_RESET_ENABLE = 0x04;
  HW_SYS_RESET_ENABLE = 0x00;
  meLibSync();
  return tableId;
}

int main() {
  if (pspSdkLoadStartModule("./kcall.prx", PSP_MEMORY_PARTITION_KERNEL) < 0){
    sceKernelExitGame();
    return 0;
  }
  
  pspDebugScreenInit();
  
  const int tableId = kcall(initMe);
  meLibGetUncached32(&mem, 2);
  
  pspDebugScreenSetXY(1, 1);
  pspDebugScreenPrintf("Me Core Demo");
  
  SceCtrlData ctl;
  do {
    pspDebugScreenSetXY(1, 1);
    pspDebugScreenPrintf("clock buses enabled: 0x%08x", clockBuses);
    pspDebugScreenSetXY(1, 2);
    pspDebugScreenPrintf("table Id: %i", tableId);
    sceCtrlPeekBufferPositive(&ctl, 1);
  } while(!meExit && !(ctl.Buttons & PSP_CTRL_HOME));
  
  sceKernelDelayThread(1000000);
  sceKernelExitGame();
  
  return 0;
}
