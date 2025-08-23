## PSP Media Engine's Core: Mapper Library.
A project to map the PSP's Media Engine core functions with the purpose of using them in our code.  
The idea is simply to make the native kernel/core functions of the media engine available to homebrew developers.  

The media engine core is loaded at address 0x88300000, however depending on the device model, it is loaded from a different image which means that addresses are not the same for models prior to the slim.
As a solution, we have several static correspondence tables and teh code will select the table corresponding to our device before launching the Media Engine.

## Usage

### Basic Integration
```cpp
#include "me-lib.h"

void meLibExec() {
  HW_SYS_BUS_CLOCK_ENABLE      = 0x0f;
  HW_SYS_NMI_FLAGS             = 0xffffffff;
  HW_SYS_TACHYON_CONFIG_STATUS = 0x02;
  meLibSync();
  // ... your me code
}

int main() {
  const int tableId = meLibDefaultInit();
  // ... main core
  return 0;
}
```

#### Steps:
1. Include the library header
2. Implement `meLibExec()` with your Media Engine code
3. Initialize the library in `main()` with `meLibDefaultInit()`
4. Add hardware initialization as needed in `meLibExec()`
  
  
See related samples available in the `samples` folder for more information.

## Disclamer
This project and code are provided as-is without warranty. Users assume full responsibility for any implementation or consequences. Use at your own discretion and risk

## Thanks
Thanks to:
- crazyc, hlide and other ps2dev members for being pioneers on that subject
- CelestBlue, artart78, zecoxao, davee, xerpi and John (Hedge) for all the information available on psdevwiki and for reversing things
- Kotcrab for reversing things and for Ghidra scripts
- z2442 for the idea!
- Acid_Snake for testing
- All contributors to the PSP Hardware Registers page + the wiki
- Anyone else who contributed through documentation or reverse engineering

*m-c/d*
