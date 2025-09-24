#include <pspdebug.h>
#include <psppower.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <me-core-mapper/me-core.h>

PSP_MODULE_INFO("me-exceptions", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

meLibSetSharedUncachedMem(2);
#define meExit          (meLibSharedMemory[0])
#define meCounter       (meLibSharedMemory[1])

meLibMakeUncachedMem(_meExtIntrProof, 1);
meLibMakeUncachedMem(_meExpProof, 1);
#define meExtIntrProof  (meLibMakeUncachedVar(_meExtIntrProof, UNCACHED_USER_MASK)[0])
#define meExpProof      (meLibMakeUncachedVar(_meExpProof, UNCACHED_USER_MASK)[0])

extern "C" __attribute__((noinline, aligned(4)))
void meLibOnExternalInterrupt(void) {
  asm volatile(
    ".set noreorder                  \n"
    // increment external interrupt proof
    "la       $k0, %0                \n"
    "li       $k1, 0xa0000000        \n"
    "or       $k0, $k0, $k1          \n"
    "cache    0x8, 0($k0)            \n"
    "sync                            \n"
    
    "lw       $k1, 0($k0)            \n"
    "addiu    $k1, $k1, 1            \n"
    "sw       $k1, 0($k0)            \n"
    "sync                            \n"
    ".set reorder                    \n"
    :
    : "i" (_meExtIntrProof)
    : "k0", "k1", "memory"
  );
}

extern "C" __attribute__((noinline, aligned(4)))
void meLibOnException(void) {
  asm volatile(
    ".set noreorder                  \n"
    // increment exception proof
    "la       $k0, %0                \n"
    "li       $k1, 0xa0000000        \n"
    "or       $k0, $k0, $k1          \n"
    "cache    0x8, 0($k0)            \n"
    "sync                            \n"
    
    "lw       $k1, 0($k0)            \n"
    "addiu    $k1, $k1, 2            \n"
    "sw       $k1, 0($k0)            \n"
    "sync                            \n"
    ".set reorder                    \n"
    :
    : "i" (_meExpProof)
    : "k0", "k1", "memory"
  );
}

__attribute__((noinline, aligned(4)))
void meLibOnProcess(void) {  
  meLibExceptionHandlerInit();
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
  pspDebugScreenPrintf("Me Exceptions");
  
  bool triangle = false;
  SceCtrlData ctl;
  do {
    sceCtrlPeekBufferPositive(&ctl, 1);
    pspDebugScreenSetXY(1, 2);
    pspDebugScreenPrintf("Me Counter: %x", meCounter);
    pspDebugScreenSetXY(1, 3);
    pspDebugScreenPrintf("Me Ext Intr Proof: %x", meExtIntrProof);
    pspDebugScreenSetXY(1, 4);
    pspDebugScreenPrintf("Me Exception Proof: %x", meExpProof);
    
    if (!triangle && (ctl.Buttons & PSP_CTRL_TRIANGLE)) {
      kcall(meCoreEmitSoftwareInterrupt);
      triangle = true;
    } else if(!(ctl.Buttons & PSP_CTRL_TRIANGLE)) {
      triangle = false;
    }
    
    sceDisplayWaitVblank();
  } while (!(ctl.Buttons & PSP_CTRL_HOME));
  
  meWaitExit();
  
  sceKernelDelayThread(1000000);
  sceKernelExitGame();
  
  return 0;
}
