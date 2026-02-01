#ifndef ME_LIB_EXTENDED_H
#define ME_LIB_EXTENDED_H

// Use this file instead of 'me-lib.h' if you wish to use the custom extended
// functions as an alternative to meCore. This is useful for debugging.

#include "me-lib.h"

// define the non-cached kernel mutex
#define mutex hw(0xbc100048)

#define meLibSuspendCpuIntr(var) \
  asm volatile(                  \
    "mfic  %0, $0 \n"            \
    "mtic  $0, $0 \n"            \
    "sync         \n"            \
    : "=r"(var)                  \
    :                            \
    : "$8"                       \
  )

#define meLibResumeCpuIntr(var)  \
  asm volatile(                  \
    "mtic  %0, $0 \n"            \
    "sync         \n"            \
    :                            \
    : "r"(var)                   \
  )

#ifdef __cplusplus
extern "C" {
#endif

  int meLibSendExternalSoftInterrupt();
  u32Me meLibGetCpuId();
  // kernel function to unlock the mutex
  int meLibHwMutexUnlock();
  // kernel function that waits and attempts to lock and acquire the mutex
  int meLibHwMutexLock();
  // kernel function to attempt locking and acquiring the mutex
  int meLibHwMutexTryLock();

  // note:
  // it appears that the main CPU can read the mutex and only set bit[0],
  // while the Me can read the mutex and only set bit[1]
  //
  // mutex    unique
  // 11  xor  01 =>   not 10 = 0
  // 11  xor  10 =>   not 01 = 0
  // 10  xor  01 =>   not 11 = 0
  // 10  xor  10 =>   not 00 = 1
  // 01  xor  01 =>   not 00 = 1
  // 01  xor  10 =>   not 11 = 0

  void meLibDcacheWritebackInvalidateAll();
  void meLibDcacheWritebackInvalidateRange(const u32Me addr, const u32Me size);
  void meLibDcacheInvalidateRange(const u32Me addr, const u32Me size);
  void meLibDcacheWritebackRange(const u32Me addr, const u32Me size);

  void meLibIcacheInvalidateAll();
  void meLibIcacheInvalidateRange(const u32Me addr, const u32Me size);

  int meLibExtendedOverclock();
  int meLibExtendedCancelOverclock();

#ifdef __cplusplus
}
#endif

#endif
