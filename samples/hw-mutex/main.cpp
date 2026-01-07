#include <pspdebug.h>
#include <psppower.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <me-core-mapper/me-core.h>

PSP_MODULE_INFO("me-core-hw-mutex", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

// align shared vars to 64 for cached/uncached access compatibility
volatile u32Me* shared __attribute__((aligned(64))) = nullptr;

meLibSetSharedUncached32(3);
#define meExit           (meLibSharedMemory[0])
#define meCounter        (meLibSharedMemory[1])
#define meStart          (meLibSharedMemory[2])

void meLibOnProcess() {
  do {
    meLibDelayPipeline();
  } while (!meStart);
  
  const u32Me sharedSize = (sizeof(u32Me) * 4 + 63) & ~63;
    
  do {
    meCounter++;
    
    while (meCoreHwMutexTryLock() < 0) {
      meLibDelayPipeline();
    }
    
    // invalidate cache, forcing next read to fetch from memory
    meCoreDcacheInvalidateRange((void*)shared, sharedSize);
    if (shared[1] > 100) {
      shared[1] = 0;
    }
    
    meCoreHwMutexUnlock();
    meLibDelayPipeline();
    
    // write modified cache data back to memory
    meCoreDcacheWritebackRange((void*)shared, sharedSize);
  } while (meExit == 0);
  
  meExit = 2;
  meLibHalt();
}

// function used to hold the mutex in the main loop as a proof
static bool holdMutex() {
  static u32Me hold = 100;
  if (hold-- > 0) {
    return true;
  }
  hold = 100;
  return false;
}

static void meWaitExit() {
  // make sure the mutex is unlocked
  meLibCallHwMutexUnlock();
  // wait the me to exit
  meExit = 1;
  u8 retry = 0;
  do {
    sceKernelDelayThread(100000);
  } while (meExit < 2 && ++retry <= 5);
}

int main() {
  scePowerSetClockFrequency(333, 333, 166);

  pspDebugScreenInit();
  const int tableId = meLibDefaultInit();
  
  // 64-byte alignment is required while using to use Dcache... Range
  shared = (u32Me*)memalign(64, (sizeof(u32Me) * 4 + 63) & ~63);
  memset((void*)shared, 0, sizeof(u32Me) * 4);
  sceKernelDcacheWritebackAll();
  
  pspDebugScreenSetXY(1, 1);
  pspDebugScreenPrintf("Me Core Mutex Demo");
  
  SceCtrlData ctl;
  u32Me counter = 0;
  bool switchMessage = false;
  
  // start the process on the Me just before the main loop
  meStart = true;
  do {
    // invalidate cache, forcing next read to fetch from memory
    sceKernelDcacheInvalidateRange((void*)shared, sizeof(u32Me) * 4);
    
    // try to lock and modify shared variable
    if (meLibCallHwMutexTryLock() >= 0) {
      switchMessage = false;
      if (shared[1] > 50) {
        switchMessage = true;
      }
      shared[2]++;
      shared[1]++;
      // sceKernelDelayThread(1000000);
      
      // proof to visualize the release of the mutex and its effect on the counter (mem[0]) running on the Me
      if (!holdMutex()) {
        meLibCallHwMutexUnlock();
      }
    }
    
    // push cache to memory and invalidate it, refill cache during the next access
    sceKernelDcacheWritebackInvalidateRange((void*)shared, sizeof(u32Me) * 4);

    sceCtrlPeekBufferPositive(&ctl, 1);
    
    pspDebugScreenSetXY(1, 2);
    pspDebugScreenPrintf("Table Id: %i    ", tableId);
    pspDebugScreenSetXY(1, 3);
    pspDebugScreenPrintf("Sc counter %u   ", counter++);
    pspDebugScreenSetXY(1, 4);
    pspDebugScreenPrintf("Me counter %u   ", meCounter);
    pspDebugScreenSetXY(1, 5);
    pspDebugScreenPrintf("Shared counters %u, %u  ", shared[1], shared[2]);
    
    pspDebugScreenSetXY(1, 6);
    if (switchMessage) {
      pspDebugScreenPrintf("Hello!");
    } else {
      pspDebugScreenPrintf("xxxxxx");
    }
    
    sceDisplayWaitVblankStart();
  } while (!(ctl.Buttons & PSP_CTRL_HOME));
  
  meWaitExit();
  
  free((void*)shared);
  sceKernelExitGame();
  
  return 0;
}
