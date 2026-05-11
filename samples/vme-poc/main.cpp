
/*
 * VME PoC
 * 
 * Sample code using the PSP DSP known as VME2. It is a POC demonstrating
 * the VME flow and how to perform an operation on a set of 32 values through a
 * 3-stage fixed-point Q.23 pipeline that multiplies each value of a buffer
 * by a variable factor.
 * 
 * Copyright mcidclan, m-c/d 2026
 */
#include "main.h"

PSP_MODULE_INFO("vme-poc", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

ME_LIB_SETUP_SIMPLE_SUSPEND_HANDLER();

meLibSetSharedUncached32(2);
#define meExit       (meLibSharedMemory[0])
#define meCounter    (meLibSharedMemory[1])

#define DATA_COUNT 64
#define BYTE_COUNT (DATA_COUNT * 4)
#define DEFAULT_FACTOR 0.5f

float factor = DEFAULT_FACTOR;

volatile u32* meDataGen __attribute__((aligned(64))) = nullptr;
volatile u32* meDataOut __attribute__((aligned(64))) = nullptr;
volatile u32* meVars    __attribute__((aligned(64))) = nullptr;

void meLibOnProcess(void) {
  
  meLibExceptionHandlerInit(0);
  
  vmeLibEnable();
  vmeLibWipe();
  
  const float values[] __attribute__((aligned(64))) = {
    
    0.12f,  0.35f,  0.67f,  0.89f,
    0.45f,  0.23f,  0.78f,  0.56f,
    0.91f,  0.34f,  0.11f,  0.72f,
    0.48f,  0.63f,  0.82f,  0.19f,
   -0.25f, -0.47f, -0.63f, -0.81f,
   -0.12f, -0.55f, -0.38f, -0.74f,
   -0.92f, -0.17f, -0.44f, -0.69f,
   -0.31f, -0.58f, -0.83f, -0.96f,
  };
  
  u32* const top = (u32*)VME_TOP_BUFFERS;
  const int size = sizeof(values) / sizeof(values[0]);
  
  for (int i = 0; i < size; i++) {
    
    top[i] = F2Q(values[i]);
  }

  // Start of the VME related code
  vmeLibStart();
  
  vme_set(GLOBAL, MAPPER_B, VME_DEF_MAPPER);

  const u8  k = Q_FORMAT;
  const u32 b = F2Q(DEFAULT_FACTOR);
  const int count = (size + 0x10) - 1;
  
  {
    // r1 = (x * b) >> k
    const u32 op = 0x00204000;
    vme_set(PE_0, TOP_DESCRIPTOR, op, k);
    vme_set(PE_0, TOP_REGISTER_B, b);
    
    vme_set(PE_0, TOP_SRC, VME_PFX_ROUTE);
    vme_set(PE_0, TOP_COUNT, VME_PFX_PARAM, count);
    
    vme_set(PE_0, BASE_SRC, VME_PFX_ROUTE);
    vme_set(PE_0, BASE_COUNT, VME_PFX_PARAM, count);
    
    vme_set(PE_0, DST, VME_PFX_ROUTE, (6 << 16));
    vme_set(PE_0, DST_COUNT, VME_PFX_PARAM, count);

    // force update over local buffer with a 0x10 padding
    // necessary to get the correct result from the first cycle
    vme_set(PE_0, DST_PARAM_2, 0x00020010);
    vme_set(PE_0, DST_PARAM_3, 0x00200000);
    
  }
  
  {
    // r2 = (r1 * b) >> k
    const u32 op = 0x00204000;
    vme_set(PE_1, TOP_DESCRIPTOR, VME_BASE_0, (0x04 << 24), op, k);
    vme_set(PE_1, TOP_REGISTER_B, b);
    
    vme_set(PE_1, BASE_SRC, VME_PFX_ROUTE);
    vme_set(PE_1, BASE_COUNT, VME_PFX_PARAM, count);
    
    vme_set(PE_1, DST, VME_PFX_ROUTE, (9 << 16));
    vme_set(PE_1, DST_COUNT, VME_PFX_PARAM, count);
    
    // force update over local buffer with a 0x10 padding
    // necessary to get the correct result from the first cycle
    vme_set(PE_1, DST_PARAM_2, 0x00020010);
    vme_set(PE_1, DST_PARAM_3, 0x00200000);
  }
  
  {
    // r3 = r1 + r2
    const u32 op = 0x02010000;
    vme_set(PE_2, TOP_DESCRIPTOR, VME_BASE_1, op);
    
    const int offset = 0xfff0; // cancel padding (-0x10)
    vme_set(PE_2, DST, VME_PFX_ROUTE, 0x60000, offset);
    vme_set(PE_2, DST_COUNT, VME_PFX_PARAM, count);
  }
  
  // End of the VME related code
  vmeLibFinish();
  vmeLibDisable();
  
  while (1) {
    
    meCounter += 1;
    
    vmeLibEnable();
    
    // Start of the VME datapath update
    vmeLibStart();
    vme_set(PE_0, TOP_REGISTER_B, meVars[0]);
    vme_set(PE_1, TOP_REGISTER_B, meVars[1]);

    // End of the VME datapath update
    vmeLibFinish();
    
    if (!meVars[2]) {
      
      meCoreMemcpy((void*)meDataGen, (void*)(VME_TOP_BUFFERS), BYTE_COUNT);
      meVars[2] = 1;
    }
    
    meCoreMemcpy((void*)meDataOut, (void*)(VME_BASE_BUFFERS + 8192*2), BYTE_COUNT);

    vmeLibDisable();
  }
}

void updateFactor(SceCtrlData* const ctl) {
  
  static bool up = false;
  static float lastFactor = 0.0f;
  
  if (!(ctl->Buttons & PSP_CTRL_TRIANGLE) && !(ctl->Buttons & PSP_CTRL_CROSS)) {
    
    up = true;
  } else if (up) {

    if (ctl->Buttons & PSP_CTRL_TRIANGLE) {
        factor = (factor + 0.1f > 1.0f) ? 1.0f : factor + 0.1f;
        up = false;
    }
    else if (ctl->Buttons & PSP_CTRL_CROSS) {
        factor = (factor - 0.1f < 0.1f) ? 0.1f : factor - 0.1f;
        up = false;
    }
  }
  
  if (lastFactor != factor) {
    
    getB(factor, (u32*)&meVars[0], (u32*)&meVars[1]);
    lastFactor = factor;
  }
}

int main() {
  
  scePowerSetClockFrequency(333, 333, 166);
  
  meLibGetUncached32(meVars, 16);
  meLibGetUncached32(meDataGen, DATA_COUNT);
  meLibGetUncached32(meDataOut, DATA_COUNT);

  meLibDefaultInit();
  
  pspDebugScreenInit();
  SceCtrlData ctl;
  do {
    
    sceCtrlPeekBufferPositive(&ctl, 1);
    
    const u32* const gen = (u32*)meDataGen;
    const u32* const out = (u32*)meDataOut;
    
    pspDebugScreenSetXY(0, 0);
    pspDebugScreenPrintf("ME Generated Data:");
    
    for(int i = 0; i < 8; i++) {
      
      const int off = i * 4;
      pspDebugScreenSetXY(0, i+2);
      pspDebugScreenPrintf("%f, %f, %f, %f,",
        Q2F(gen[off+0]), Q2F(gen[off+1]), Q2F(gen[off+2]), Q2F(gen[off+3]));
    }
    
    pspDebugScreenSetXY(0, 12);
    pspDebugScreenPrintf("VME Output Data:");
    for(int i = 0; i < 8; i++) {
      
      const int off = i * 4;
      pspDebugScreenSetXY(0, i+14);
      pspDebugScreenPrintf("%f, %f, %f, %f,",
        Q2F(out[off+0]), Q2F(out[off+1]), Q2F(out[off+2]), Q2F(out[off+3]));
    }

    pspDebugScreenSetXY(0, 24);
    pspDebugScreenPrintf("Input factor: %f", factor);
    pspDebugScreenSetXY(0, 25);
    pspDebugScreenPrintf("Use Triangle/Cross to change its values");

    pspDebugScreenSetXY(0, 28);
    pspDebugScreenPrintf("meCounter: 0x%lx", meCounter);
        
    updateFactor(&ctl);
    sceDisplayWaitVblank();
    
  } while (!(ctl.Buttons & PSP_CTRL_HOME));
  
  meLibFreeUncached32(meVars);
  meLibFreeUncached32(meDataGen);
  meLibFreeUncached32(meDataOut);
  
  sceKernelExitGame();
  return 0;
}
