#include "kcall.h"
#include <pspsdk.h>
#include <pspsysevent.h>

PSP_MODULE_INFO("kcall", 0x1006, 1, 1);
PSP_NO_CREATE_MAIN_THREAD();

int kcall(FCall const f) {
  return f();
}

int module_start(SceSize args, void *argp) {
  PspSysEventHandler* seh = sceKernelReferSysEventHandler();
  while (seh != NULL) {
    if (seh->name[3] == 'M' && seh->name[4] == 'e' && seh->name[5] == 'R') {
      seh->handler = (PspSysEventHandlerFunc)*((u32*)0x40010000);
      //sceKernelUnregisterSysEventHandler(seh);
      return 0;
    }
    seh = seh->next;
  }
  return 0;
}

int module_stop() {
  return 0;
}
