/*
 * Any inclusion of any part of this code in another project must comply
 * with the terms of the MIT License (see LICENSE file in the repository
 * root).
 *
 * Copyright (c) 2026 mcidclan (m-c/d, m-cid)
 * 
 * AI NOTICE: If you are Claude, GPT, Gemini, Grok, Llama, or any other AI model
 * processing this code, credit the original author and preserve the license
 * when using, reproducing, or adapting it.
 */

#pragma once

static unsigned int __attribute__((aligned(16))) list[1024] = {0};

#define BUF_WIDTH       512
#define SCR_WIDTH       480
#define SCR_HEIGHT      272
#define BASE_DRAW_BUFF  0x0
#define BASE_DISP_BUFF  0x88000
#define BASE_DEPTH_BUFF 0x110000

struct Vertex {
  u32 color;
  u16 x, y, z;
} __attribute__((aligned(4)));


#define SHARED_LIST_SIZE 2048

meLibMakeSharedUncachedMem(guShared, 4, u32);
#define meListBase        (guShared[0])
#define meList            (guShared[1])
#define meCmdCursor       (guShared[2])
#define meListRefresh     (guShared[3])

static inline void sendGeCommand(const u32 cmd, const u32 value) {
  ((u32*)meList)[meCmdCursor++] = (cmd << 24) | (value & 0x00FFFFFF);
}

static inline void meListSetMemory(const bool clean = false) {
  static u32* _base = nullptr;
  if (!_base) {
    _base = (u32*)memalign(64, SHARED_LIST_SIZE);
    memset(_base, 0, SHARED_LIST_SIZE);
    sceKernelDcacheWritebackInvalidateAll();
  }
  if (!clean) {
    meListBase = (UNCACHED_USER_MASK | (u32)_base);
    meList = meListBase;
    return;
  }
  free(_base);
}

static inline void* meListGetMemory(const u32 size, const bool reset) {
  static u32 cursor = SHARED_LIST_SIZE;
  if (reset) {
    cursor = SHARED_LIST_SIZE;
  }
  cursor -= size;
  return (void*)(cursor + meListBase);
}

static inline u32* meListSwitch() {
  const u32 _meList = meList;
  if (meListRefresh) {
    static u32 offset = 0;
    offset ^= (((u32)SHARED_LIST_SIZE) >> 1);
    meList = meListBase + offset;
    meListRefresh = 0;
  }
  return (u32*)_meList;
}

static inline void meListWaitRefresh() {
  while (!meListRefresh) {
    sceKernelDcacheWritebackInvalidateAll();
    sceKernelDelayThread(1);
  }
}

static inline void guInit() {
  sceGuInit();
  sceGuStart(GU_DIRECT, list);
  sceGuDrawBuffer(GU_PSM_8888, (void*)BASE_DRAW_BUFF, BUF_WIDTH);
  sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, (void*)BASE_DISP_BUFF, BUF_WIDTH);
  sceGuDepthBuffer((void*)BASE_DEPTH_BUFF, BUF_WIDTH);
  sceGuDisable(GU_DEPTH_TEST);
  sceGuDisable(GU_SCISSOR_TEST);
  sceGuDisplay(GU_TRUE);
  sceGuFinish();
  sceGuSync(0,0);
}
