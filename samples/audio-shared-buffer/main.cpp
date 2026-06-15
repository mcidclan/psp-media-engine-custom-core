// mcidclan 2026
#include "main.h"

PSP_MODULE_INFO("me-audio-shared", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

// Note:
// La,  C  = 440.00 Hz
// Si,  D  = 493.88 Hz
// Do,  E  = 523.25 Hz
// Ré,  F  = 587.33 Hz
// Mi,  G  = 659.26 Hz
// Fa,  A  = 698.46 Hz
// Sol, B  = 783.99 Hz

static int channel = -1;

#define PSP_VOLUME_MAX 0x8000
#define SAMPLES        512
#define PANS           2

meLibSetSharedUncached32(10);
#define sharedBufferIndex (meLibSharedMemory[0])
#define sharedFrontBuffer (meLibSharedMemory[1])
#define sharedGenMode     (meLibSharedMemory[2])

volatile u32* sharedAudioBuffer __attribute__((aligned(64))) = nullptr;

void genLaSiDo(short* const backBuffer) {
  
  static float phase = 0.0f;
  static int noteIndex = 0;
  static int noteBuffersPlayed = 0;
  
  const float notes[] = { 440.0f, 494.0f, 523.0f };
  const int NOTE_DURATION_MS = 250;
  const int NOTE_BUFFER_COUNT = (NOTE_DURATION_MS * 44100 / (SAMPLES * 1000));

  const float step = (2.0f * M_PI * notes[noteIndex]) / 44100.0f;

  for (int i = 0; i < SAMPLES; i++) {
    backBuffer[i * PANS] = (short)(sinf(phase) * 0x7FFF);
    backBuffer[i * PANS + 1] = (short)(sinf(phase) * 0x7FFF);
    
    phase += step;
    if (phase > 2.0f * M_PI) {
      phase -= 2.0f * M_PI;
    }
  }
  
  noteBuffersPlayed++;
  if (noteBuffersPlayed >= NOTE_BUFFER_COUNT) {
    
    noteBuffersPlayed = 0;
    noteIndex = (noteIndex + 1) % 3;
  }
}

void genLa(short* const backBuffer) {
 
  static float phase = 0.0f;
  const float freq = 440.0f;
  const float step = (2.0f * M_PI * freq) / 44100.0f;

  for (int i = 0; i < SAMPLES; i++) {

    backBuffer[i * PANS] = (short)(sinf(phase) * 0x7FFF);
    backBuffer[i * PANS + 1] = (short)(sinf(phase) * 0x7FFF);
    
    phase += step;
    if (phase > 2.0f * M_PI) {
      phase -= 2.0f * M_PI;
    }
  }
}

void genWhiteNoise(short* const backBuffer) {
  
  for (int i = 0; i < SAMPLES; i++) {
    
    const short sample = (short)(randInRange(0xFFFF));
    backBuffer[i * PANS] = sample; // left
    backBuffer[i * PANS + 1] = sample; // right
  }
}


void meLibOnProcess(void) {
  
  meLibDcacheWritebackInvalidateAll();
  
  short* backBuffer = nullptr;
  
  while (1) {
    
    if (sharedFrontBuffer != (u32)backBuffer) {
      
      if (meCoreHwMutexTryLock() >= 0) {
        
        backBuffer = (short*)&sharedAudioBuffer[sharedBufferIndex * SAMPLES];

        switch (sharedGenMode) {
          case 0:
            genLaSiDo(backBuffer);
            break;
          case 1:
            genLa(backBuffer);
            break;
          default:
            genWhiteNoise(backBuffer);
        }
        meCoreHwMutexUnlock();
      }
    }
    meLibDelayPipeline();
  }
}

int audioRead(SceSize args, void *argp) {
  
  int* const meStopped = (int*)*((int*)argp);
  
  sharedBufferIndex = 0;
  sharedFrontBuffer = (u32)&sharedAudioBuffer[0];
    
  while (!*meStopped) {
    
    const u32 frontBufferIndex = !(sharedBufferIndex & 1);
    
    sceAudioOutputPannedBlocking(
      channel,
      PSP_VOLUME_MAX,
      PSP_VOLUME_MAX,
      (short*)sharedFrontBuffer
    );
    
    while (meLibCallHwMutexTryLock() < 0) {
      sceKernelDelayThread(1);
    }
    meLibCallHwMutexUnlock();
    
    sharedFrontBuffer = (u32)&sharedAudioBuffer[frontBufferIndex * SAMPLES];
    sharedBufferIndex = frontBufferIndex;

    sceKernelDelayThread(1);
  }
  return sceKernelExitThread(0);
}

int main() {
  
  // allocate SAMPLES * CHANNELS * 2
  meLibGetUncached32(sharedAudioBuffer, SAMPLES * PANS);
  meLibDefaultInit();

  channel = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL, SAMPLES, 0);

  int meStopped = 0;
  int thid = sceKernelCreateThread("audio-read", audioRead, 0x18, 0x2000, PSP_THREAD_ATTR_VFPU, 0);
  
  if (thid >= 0) {
    
    int* param[1] = {&meStopped};
    sceKernelStartThread(thid, sizeof(meStopped), &param);
  }
  
  pspDebugScreenInit();
  
  int up = 1;
  SceCtrlData ctl;
  do {
    sceCtrlPeekBufferPositive(&ctl, 1);
    
    pspDebugScreenSetXY(0, 0);
    pspDebugScreenPrintf("Press Triangle To Change Sound.");

    if ((ctl.Buttons & PSP_CTRL_TRIANGLE) && up) {
      
      sharedGenMode = (sharedGenMode + 1) % 3;
      up = 0;
    }
    else if (!(ctl.Buttons & PSP_CTRL_TRIANGLE)) {
      up = 1;
    }
    
    sceDisplayWaitVblankStart();
    
  } while (!(ctl.Buttons & PSP_CTRL_HOME));
  
  meStopped = 1;
  SceUInt timeout = 500000;
  sceKernelWaitThreadEnd(thid, &timeout);
  sceKernelTerminateDeleteThread(thid);
  
  sceAudioChRelease(channel);
  meLibFreeUncached32(sharedAudioBuffer);
  sceKernelExitGame();
  
  return 0;
}
