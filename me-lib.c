#include "me-lib.h"

static inline int meLibInit() {
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

#define PRX_FILE "./kcall.prx"
extern unsigned char kcall_module_start;
extern unsigned int kcall_module_size;

int writePrx(void* start, int size) {
  SceUID fd = sceIoOpen(PRX_FILE, PSP_O_WRONLY | PSP_O_CREAT, 0777);
  if (fd < 0) {
      return -1;
  }
  int _size = sceIoWrite(fd, start, size);
  sceIoClose(fd);
  if (_size != size) {
      return -1;
  }
  return 0;
}

int meLibDefaultInit() {
  if(writePrx(&kcall_module_start, (int)&kcall_module_size) < 0) {
    return -3;
  }
  if (pspSdkLoadStartModule(PRX_FILE, PSP_MEMORY_PARTITION_KERNEL) < 0){
    sceKernelExitGame();
    return -3;
  }
  return kcall(meLibInit);
}

void meLibGetUncached32(volatile u32** const mem, const u32 size) {
  static void* _base = NULL;
  if (!_base) {
    const u32 byteCount = size * 4;
    _base = memalign(16, byteCount);
    memset(_base, 0, byteCount);
    sceKernelDcacheWritebackInvalidateAll();
    *mem = (u32*)(UNCACHED_USER_MASK | (u32)_base);
    __asm__ volatile (
      "cache 0x1b, 0(%0)  \n"
      "sync               \n"
      : : "r" (mem) : "memory"
    );
    return;
  } else if (!size) {
    free(_base);
  }
  *mem = NULL;
  return;
}
