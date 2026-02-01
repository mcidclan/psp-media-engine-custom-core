#include "me-lib-extended.h"
#include "psppower.h"

int meLibSendExternalSoftInterrupt() {
  asm volatile("sync");
  hw(0xBC100044) = 1;
  asm volatile("sync");
  return 0;
}

u32Me meLibGetCpuId() {
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
int meLibHwMutexUnlock() {
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
int meLibHwMutexLock() {
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
int meLibHwMutexTryLock() {
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

void meLibDcacheWritebackInvalidateAll() {
  asm volatile ("sync");
  for (int i = 0; i < 8192; i += 64) {
    asm volatile ("cache 0x14, 0(%0)" :: "r"(i));
    asm volatile ("cache 0x14, 0(%0)" :: "r"(i));
  }
  asm volatile ("sync");
}

void meLibDcacheWritebackInvalidateRange(const u32Me addr, const u32Me size) {
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

void meLibDcacheInvalidateRange(const u32Me addr, const u32Me size) {
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

void meLibDcacheWritebackRange(const u32Me addr, const u32Me size) {
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

void meLibIcacheInvalidateAll() {
  asm volatile ("sync");
  for (int i = 0; i < 8192; i += 64) {
    asm("cache 0x04, 0(%0)" :: "r"(i));
    asm("cache 0x04, 0(%0)" :: "r"(i));
  }
  asm volatile ("sync");
}

void meLibIcacheInvalidateRange(const u32Me addr, const u32Me size) {
  asm volatile("sync");
  for (volatile u32Me i = addr; i < addr + size; i += 64) {
    asm volatile(
      "cache 0x08, 0(%0)\n"
      "cache 0x08, 0(%0)\n"
      :: "r"(i)
    );
  }
  asm volatile("sync");
}

// note: needs to be 333 to be able to reach 444mhz
#define INITIAL_FREQUENCY 333
// modify this value to compare results
#define THEORETICAL_FREQUENCY (444 /*+ 37/4*/)

int meLibExtendedCancelOverclock() {
  
  const u32 index = 2;
  hw(0xbc100068) = 0x80 | index;
  do {
    meLibDelayPipeline();
  } while (hw(0xbc100068) != index);
  
  hw(0xbc200000) = 32 << 16 | 511;
  hw(0xBC200004) = 32 << 16 | 511;
  hw(0xBC200008) = 32 << 16 | 511;
  meLibSync();
  
  hw(0xbc1000fc) = 0x01240901;
  meLibDelayPipeline();
  
  scePowerSetClockFrequency(333, 333, 166);
  return 0;
}

// Note: Only tested on Slim, should be called from SC side
// 444 ok (approaching the stability limit)
// base * (num / den) * ratio, with base = 37 and ratio = 1
int meLibExtendedOverclock() {
  
  scePowerSetClockFrequency(INITIAL_FREQUENCY, INITIAL_FREQUENCY, INITIAL_FREQUENCY/2);

  // set index, to pll ratio 1
  const u32 index = 5; // hw(0xbc100068) & 0xf;
  
  float ratio = 1.0f;
  switch (index) {
    // incomplete
    case 5: ratio = 1.0f; break;
  }
  
  const u32 den = 19;
  const float base = 37;
  const u32 num = (u32)(((float)(THEORETICAL_FREQUENCY * den)) / (base * ratio));
  
  int intr;
  meLibSuspendCpuIntr(intr);
  
  u32 _num = (u32)(((float)(INITIAL_FREQUENCY * den)) / (base * ratio));
  const u32 msb = 0x0124; // unknown

  // throttle all clock domains before overclocking PLL
  hw(0xbc200000) = 32 << 16 | 511;
  hw(0xBC200004) = 32 << 16 | 511;
  hw(0xBC200008) = 32 << 16 | 511;
  meLibSync();
    
  // set bit bit 7 to apply index, wait until hardware clears it
  hw(0xbc100068) = 0x80 | index;
  do {
    meLibDelayPipeline();
  } while (hw(0xbc100068) != index);
  
  // loop until the numerator reaches the target value,
  // and so, progressively increasing clock frequencies
  while (_num <= num) {
    const u32 lsb = _num << 8 | den;
    const u32 multiplier = (msb << 16) | lsb;
    hw(0xbc1000fc) = multiplier;
    meLibDelayPipeline();
    _num++;
  }

  // unthrottle all clock domains after overclocking PLL
  hw(0xbc200000) = 511 << 16 | 511;
  hw(0xBC200004) = 511 << 16 | 511;
  hw(0xBC200008) = 511 << 16 | 511;
  meLibSync();

  // wait for clock stability, signal propagation and pipeline drain
  {
    u32 i = 0xfffff;
    while (--i) {
      meLibDelayPipeline();
    }
  }
  
  meLibResumeCpuIntr(intr);
  return 0;
}
