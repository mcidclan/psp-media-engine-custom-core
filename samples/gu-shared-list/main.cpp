#include "main.h"

PSP_MODULE_INFO("me-gu-shared-list", 0, 1, 1);
PSP_HEAP_SIZE_KB(-1024);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);

meLibSetSharedUncached32(2);
#define meState         (meLibSharedMemory[0])
#define meCounter       (meLibSharedMemory[1])

static void meStart() {
  meState = 1;
}

static void meWaitFinish() {
  meState = 2;
  do {
    sceKernelDelayThread(10);
  } while(meState < 3);
}

static void writeMeList() {
  static u8 r = 0x7f;
  static u8 g = 0x7f;
  static u8 b = 0x7f;
  static u16 _r = 0;
  static u16 _g = 0;
  static u16 _b = 0;
  static u16 _d = 0;
  
  if (!meListRefresh) {
    Vertex* vertices = (Vertex*)meListGetMemory(sizeof(Vertex) * 2, true);
    
    if (_d++ > 100) {
      _r = randInRange(4);
      _g = randInRange(4);
      _b = randInRange(4);
      _d = 0;
    }
    _r >= 2 ? b < 0xfe ? b++ : b-- : b > 1 ? b-- : b++;
    _g >= 2 ? g < 0xfe ? g++ : g-- : g > 1 ? g-- : g++;
    _b >= 2 ? r < 0xfe ? r++ : r-- : r > 1 ? r-- : r++;
    
    vertices[1].color = 0xFF000000 | b << 16 | g << 8 | r;
    vertices[1].x = 480;
    vertices[1].y = 272;
    
    meCmdCursor = 0;
    sendGeCommand(0xd4, (20 << 10) | 0);                                        // CMD_SCISSOR1
    sendGeCommand(0xd5, ((SCR_HEIGHT - 20) << 10) | SCR_WIDTH);                 // CMD_SCISSOR2
    sendGeCommand(0xd3, (0b101 << 8) | 0x01);                                   // CMD_CLEAR, clear before drawing just in case
    sendGeCommand(0x12, GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D);     // CMD_VTYPE
    sendGeCommand(0x01, ((u32)vertices) & 0xffffff);                            // CMD_VADR, vertex data addr
    sendGeCommand(0x04, (GU_SPRITES << 16) | 2);                                // CMD_PRIM, draw
    sendGeCommand(0xd3, 0);                                                     // CMD_CLEAR
    sendGeCommand(0x0b, 0);                                                     // CMD_RET
    meListRefresh = 1;
  }
}

__attribute__((noinline, aligned(4)))
void meLibOnProcess(void) {
  do {
    meLibDelayPipeline();
  } while(!meState);
  
  do {
    writeMeList();
    meCounter++;
  } while(meState == 1);
  
  meState = 3;
  meLibHalt();
}

void drawSquare() {
  static int dir = 1;
  static int move = 0;

  Vertex* const vertices = (Vertex*)sceGuGetMemory(sizeof(Vertex) * 2);
  move += dir;
  if(move > 64) {
    dir = -1;
  } else if(move < -64) {
    dir = 1;
  }
  vertices[0].color = 0;
  vertices[0].x = 176 + move;
  vertices[0].y = 72;
  vertices[0].z = 0;
  vertices[1].color = 0xFF0000FF;
  vertices[1].x = 128 + 176 + move;
  vertices[1].y = 128 + 72;
  vertices[1].z = 0;
  sceGuDrawArray(GU_SPRITES, GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 2, nullptr, vertices);
}

int main() {
  scePowerSetClockFrequency(333, 333, 166);
  
  meLibDefaultInit();
  guInit();
  
  meListSetMemory();
  meStart();
  meListWaitRefresh();
  
  SceCtrlData ctl;
  do {
    sceCtrlPeekBufferPositive(&ctl, 1);

    sceGuStart(GU_DIRECT, list);
    sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
    
    const u32* const list = meListSwitch();
    sceGuCallList(list);
    
    drawSquare();
    
    sceGuFinish();
    sceGuSync(0,0);
    
    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
  } while(!(ctl.Buttons & PSP_CTRL_HOME));
  
  meWaitFinish();
  
  pspDebugScreenInit();
  pspDebugScreenClear();
  pspDebugScreenPrintf("Exiting...");
  sceKernelDelayThread(500000);
  
  meListSetMemory(false);
  sceKernelExitGame();
  
  return 0;
}
