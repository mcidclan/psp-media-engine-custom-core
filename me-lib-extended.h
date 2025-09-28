#ifndef ME_LIB_EXTENDED_H
#define ME_LIB_EXTENDED_H

// Use this file instead of 'me-lib.h' if you wish to use the custom extended
// functions as an alternative to meCore. This is useful for debugging.

#include "me-lib.h"

// define the non-cached kernel mutex
#define mutex hw(0xbc100048)

static inline int _meLibSendSoftInterrupt() {
  asm("sync");
  hw(0xBC100044) = 1;
  asm("sync");
  return 0;
}

static inline u32Me _meLibGetCpuId() {
  u32Me unique;
  asm volatile(
    "sync\n"
    "mfc0 %0, $22\n"
    "sync"
    : "=r" (unique)
  );
  return (unique + 1) & 3;
  // reads processor id from cp0 register $22
  // 0 = main cpu
  // 1 = me
}

// kernel function to unlock the mutex
static inline int _meLibHwMutexUnlock() {
  // if acquired, briefly holds the lock with a pipeline delay,
  // allowing cache operations to complete, could be useful
  meLibDelayPipeline();
  mutex = 0;
  asm volatile("sync");
  // provides opportunities for others with a pipeline delay
  meLibDelayPipeline();
  return 0;
}

// kernel function that waits and attempts to lock and acquire the mutex
static inline int _meLibHwMutexLock() {
  const u32Me unique = meLibGetCpuId();
  do {
    mutex = unique; // the main CPU can affect only bit[0] (0b01), while the Me can only affect bit[1] (0b10)
    asm volatile("sync");
    if (!(((mutex & 3) ^ unique))) { // see note
      return 0; // lock acquired
    }
    // gives a breath with a pipeline delay (7 stages)
    meLibDelayPipeline();
  } while (1);
  return 1;
}

// kernel function to attempt locking and acquiring the mutex
static inline int _meLibHwMutexTryLock() {
  const u32Me unique = meLibGetCpuId();
  mutex = unique;
  asm volatile("sync");
  if (!(((mutex & 3) ^ unique))) { // see note
    return 0; // lock acquired
  }
  asm volatile("sync"); // make sure to be sync before leaving kernel mode
  return 1;
}

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

static inline void meLibDcacheWritebackInvalidateAll() {
  asm volatile ("sync");
  for (int i = 0; i < 8192; i += 64) {
    asm("cache 0x14, 0(%0)" :: "r"(i));
    asm("cache 0x14, 0(%0)" :: "r"(i));
  }
  asm volatile ("sync");
}

static inline void meLibDcacheWritebackInvalidateRange(const u32Me addr, const u32Me size) {
  asm volatile("sync");
  for (volatile u32Me i = addr; i < addr + size; i += 64) {
    asm volatile(
      "cache 0x1b, 0(%0)\n"
      "cache 0x1b, 0(%0)\n"
      :: "r"(i)
    );
  }
  asm volatile("sync");
}

static inline void meLibDcacheInvalidateRange(const u32Me addr, const u32Me size) {
  asm volatile("sync");
  for (volatile u32Me i = addr; i < addr + size; i += 64) {
    asm volatile(
      "cache 0x19, 0(%0)\n"
      "cache 0x19, 0(%0)\n"
      :: "r"(i)
    );
  }
  asm volatile("sync");
}

static inline void meLibDcacheWritebackRange(const u32Me addr, const u32Me size) {
  asm volatile("sync");
  for (volatile u32Me i = addr; i < addr + size; i += 64) {
    asm volatile(
      "cache 0x1a, 0(%0)\n"
      "cache 0x1a, 0(%0)\n"
      :: "r"(i)
    );
  }
  asm volatile("sync");
}

#endif
