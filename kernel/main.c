#include "kcall.h"
#include <pspsdk.h>
#include <pspsysevent.h>

PSP_MODULE_INFO("kcall", 0x1006, 1, 1);
PSP_NO_CREATE_MAIN_THREAD();

int kcall(FCall const f) {
  return f();
}


int kinit(const void* const handler) {
  PspSysEventHandler* seh = sceKernelReferSysEventHandler();
  while (seh != NULL) {
    if (seh->name[3] == 'M' && seh->name[4] == 'e' && seh->name[5] == 'R') {
      seh->handler = (PspSysEventHandlerFunc)handler;
      
      // sceKernelUnregisterSysEventHandler(seh);
      // meLibRpc.handler = (PspSysEventHandlerFunc)handler;
      // sceKernelRegisterSysEventHandler(&meLibRpc);
      
      return 0;
    }
    seh = seh->next;
  }
  return -1;
}

int module_start(SceSize args, void *argp) {
  return 0;
}

int module_stop() {
  return 0;
}
