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

extern "C" __attribute__((noinline, aligned(4)))
void meLibOnExternalInterrupt(void) {
  asm volatile(
    ".set push                       \n"
    ".set noreorder                  \n"
    ".set noat                       \n"
    
    // save regs context
    "addi     $sp, $sp, -16          \n"
    "sw       $k0, 0($sp)            \n"
    "sw       $k1, 4($sp)            \n"
    "sw       $ra, 8($sp)            \n"
    "sw       $at, 12($sp)           \n"
    
    // check if 0xbfc00300 equal 1
    "li       $k0, 0xbfc00300        \n"
    "lw       $k1, 0($k0)            \n"
    "li       $k0, 1                 \n"
    "bne      $k1, $k0, 1f           \n"
    "nop                             \n"

    // call meLibSaveContext
    "la       $k0, %0                \n"
    "li       $k1, 0x80000000        \n"
    "or       $k0, $k0, $k1          \n"
    "cache    0x8, 0($k0)            \n"
    "sync                            \n"
    "jal      $k0                    \n"
    "nop                             \n"
    "1:                              \n"
    
    // check if 0xbfc00300 equal 2
    "li       $k0, 0xbfc00300        \n"
    "lw       $k1, 0($k0)            \n"
    "li       $k0, 2                 \n"
    "bne      $k1, $k0, 2f           \n"
    "nop                             \n"

    // call meLibRestoreContext
    "la       $k0, %1                \n"
    "li       $k1, 0x80000000        \n"
    "or       $k0, $k0, $k1          \n"
    "cache    0x8, 0($k0)            \n"
    "sync                            \n"
    "jal      $k0                    \n"
    "nop                             \n"
    "2:                              \n"

    "li       $k0, 0xbfc00300        \n"
    "sw       $zero, 0($k0)          \n"
    
    // restore regs context
    "lw       $k0, 0($sp)           \n"
    "lw       $k1, 4($sp)           \n"
    "lw       $ra, 8($sp)           \n"
    "lw       $at, 12($sp)           \n"
    "addi     $sp, $sp, 16          \n"

    ".set pop                        \n"
    :
    :  "i" (meLibSaveContext), "i" (meLibRestoreContext)
    : "k0", "k1", "memory"
  );
}

extern "C" void meLibOnSleep() {
  hw(0xbfc00300) = 1;
  meCoreEmitSoftwareInterrupt();
}

extern "C" void meLibOnWake() {
  hw(0xbfc00300) = 3;
  HW_SYS_RESET_ENABLE = 0x04;
  HW_SYS_RESET_ENABLE = 0x00;
  meLibSync();
  while (hw(0xbfc00300)) {
    meLibSync();
  }
  hw(0xbfc00300) = 2;
  meCoreEmitSoftwareInterrupt();
}

__attribute__((noinline, aligned(4)))
void meLibOnProcess(void) {
  meLibExceptionHandlerInit();
  hw(0xbfc00300) = 0;
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
    sceDisplayWaitVblank();
  } while (!(ctl.Buttons & PSP_CTRL_HOME));
  
  meWaitExit();  
  sceKernelExitGame();
  
  return 0;
}
