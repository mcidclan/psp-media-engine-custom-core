#include <pspkernel.h>
#include <pspdebug.h>
#include <psppower.h>

PSP_MODULE_INFO("me-core-demo", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

int main() {
  pspDebugScreenPrintf("Me Core Demo");
  sceKernelExitGame();
  sceKernelDelayThread(1000000);
  return 0;
}
