#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <psppower.h>
#include "me-lib.h"
#include "kcall.h"

PSP_MODULE_INFO("me-core-demo", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

static volatile u32* mem = nullptr;
#define meExit           (mem[0])

extern char __start__me_section;
extern char __stop__me_section;
__attribute__((section("_me_section")))
void meHandler() {
  HW_SYS_BUS_CLOCK_ENABLE      = 0x7f;
  HW_SYS_NMI_FLAGS             = 0xffffffff;
  HW_SYS_TACHYON_CONFIG_STATUS = 0x02;
  meLibSync();
  
  // Wait until mem is ready
  while (!mem) {
    meCoreDcacheWritebackInvalidateAll();
  }
  
  meExit = 1;
  meLibHalt();
}

int initMe() {
  meCoreSelectSystemTable(ME_CORE_T2_IMG_TABLE);
  #define me_section_size (&__stop__me_section - &__start__me_section)
  memcpy((void *)ME_HANDLER_BASE, (void*)&__start__me_section, me_section_size);
  sceKernelDcacheWritebackInvalidateAll();
  HW_SYS_RESET_ENABLE = 0x04;
  HW_SYS_RESET_ENABLE = 0x00;
  meLibSync();
  return 0;
}

int main() {
  if (pspSdkLoadStartModule("./kcall.prx", PSP_MEMORY_PARTITION_KERNEL) < 0){
    sceKernelExitGame();
    return 0;
  }
  
  pspDebugScreenInit();
  
  kcall(initMe);
  meLibGetUncached32(&mem, 2);
  
  pspDebugScreenPrintf("Me Core Demo");
  do {
    meLibSync();
  } while(!meExit);
  sceKernelExitGame();
  
  return 0;
}
