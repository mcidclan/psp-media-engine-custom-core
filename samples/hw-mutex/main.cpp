#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <psppower.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include "me-lib.h"
#include "kcall.h"

PSP_MODULE_INFO("me-core-hw-mutex", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

// align shared vars to 64 for cached/uncached access compatibility
volatile u32* mem    __attribute__((aligned(64))) = nullptr;
volatile u32* shared __attribute__((aligned(64))) = nullptr;

#define meExit           (mem[0])

void meLibExec() {
  // wait until mem is ready
  while (!mem) {
    meCoreDcacheWritebackInvalidateAll();
  }
  
  do {
    const int acquired = meCoreHwMutexTryLock();
    if (acquired) {
      // invalidate cache, forcing next read to fetch from memory
      meCoreDcacheInvalidateRange((u32)shared, sizeof(u32)*4);
      
      shared[0]++;
      if (mem[1] > 100) {
        shared[1] = 0;
      }
      meCoreHwMutexUnlock();
      
      // write modified cache data back to memory
      meCoreDcacheWritebackRange((u32)shared, sizeof(u32)*4);
    }
    
  } while (meExit == 0);
  
  meExit = 2;
  meLibHalt();
}

int sysHwMutexTryLock() {
  return kcall((FCall)(CACHED_KERNEL_MASK | (u32)meCoreHwMutexTryLock));
}

void sysHwMutexUnlock() {
  kcall((FCall)(CACHED_KERNEL_MASK | (u32)meCoreHwMutexUnlock));
}

// function used to hold the mutex in the main loop as a proof
static bool holdMutex() {
  static u32 hold = 100;
  if (hold-- > 0) {
    return false;
  }
  hold = 100;
  return true;
}

static void meWaitExit() {
  // make sure the mutex is unlocked
  sysHwMutexUnlock();
  // wait the me to exit
  meExit = 1;
  do {
    asm volatile("sync");
  } while (meExit < 2);
}

int main() {
  pspDebugScreenInit();
  const int tableId = meLibDefaultInit();
  
  meLibGetUncached32(&mem, 2);
  
  // 64-byte alignment is required while using to use Dcache... Range
  shared = (u32*)memalign(64, (sizeof(u32) * 4 + 63) & ~63);
  memset((void*)shared, 0, sizeof(u32) * 4);
  sceKernelDcacheWritebackAll();
  
  pspDebugScreenSetXY(1, 1);
  pspDebugScreenPrintf("Me Core Mutex Demo");
  
  SceCtrlData ctl;
  u32 counter = 0;
  bool switchMessage = false;
  
  do {
    // invalidate cache, forcing next read to fetch from memory
    sceKernelDcacheInvalidateRange((void*)shared, sizeof(u32) * 4);
    
    // functions that use spinlock
    if (!sysHwMutexTryLock()) {
      switchMessage = false;
      if (shared[1] > 50) {
        switchMessage = true;
      }
      shared[2]++;
      shared[1]++;
      // sceKernelDelayThread(10000);
      
      // proof to visualize the release of the mutex and its effect on the counter (mem[0]) running on the Me
      if (!holdMutex()) {
        sysHwMutexUnlock();
      }
    }
    
    // push cache to memory and invalidate it, refill cache during the next access
    sceKernelDcacheWritebackInvalidateRange((void*)mem, sizeof(u32) * 4);

    sceCtrlPeekBufferPositive(&ctl, 1);
    
    pspDebugScreenSetXY(1, 2);
    pspDebugScreenPrintf("Table Id: %i    ", tableId);
    pspDebugScreenSetXY(1, 3);
    pspDebugScreenPrintf("Sc counter %u   ", counter++);
    pspDebugScreenSetXY(1, 4);
    pspDebugScreenPrintf("Me counter %u   ", mem[0]);
    pspDebugScreenSetXY(1, 5);
    pspDebugScreenPrintf("Shared counters %u, %u  ", mem[1], mem[2]);
    
    pspDebugScreenSetXY(1, 6);
    if (switchMessage) {
      pspDebugScreenPrintf("Hello!");
    } else {
      pspDebugScreenPrintf("xxxxxx");
    }
    
    sceDisplayWaitVblankStart();
  } while (!meExit && !(ctl.Buttons & PSP_CTRL_HOME));
  
  meWaitExit();
  
  free((void*)shared);
  meLibGetUncached32(&mem, 0);
  sceKernelExitGame();
  
  return 0;
}
