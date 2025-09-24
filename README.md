## PSP Media Engine Custom Core: Mapper Library.
A library project to map the PSP's Media Engine core functions with the purpose of using them in our code.  
The main idea is simply to make the native kernel/core functions of the media engine available to homebrew developers.  

Additionally, the library comes with some custom functions and processes to ease the use and integration of the Media Engine into homebrew projects.
It provides a Media Engine Core Mapper (meCoreMapper) and a Media Engine Custom Library (meLib).

The media engine core is loaded at address 0x88300000, however depending on the device model, it is loaded from a different image which means that addresses are not the same for models prior to the slim.
As a solution, we have several static correspondence tables and the code will select the table corresponding to our device before launching the Media Engine.

**Note:** This is a work in progress, and the mapping table for newer device generations is evolving faster than for older ones.

## Usage

### Basic Integration

Include the me-custom-core in your build file as follows:

**CMakeLists.txt:**
```cmake
target_link_libraries(project
  ${CMAKE_SOURCE_DIR}/me-custom-core/libme-core.a
)
```

```cmake
target_include_directories(project PRIVATE 
  ${CMAKE_SOURCE_DIR}/me-custom-core
)
```

**Note:** Make sure your project declaration includes C language support:
```cmake
project(your-project-name C CXX ASM)
```

**Alternatively using a Makefile:**
```makefile
LIBS += ./me-custom-core/libme-core.a
```

```makefile
CFLAGS += -I ./me-custom-core
```

**Code example:**
```cpp
#include "me-core.h"

void meLibOnProcess() {
  // ... your ME code
}

int main() {
  const int tableId = meLibDefaultInit();
  // ... main code
  return 0;
}
```

**Using the optional Exception Handlers:**
```cpp
#include "me-core.h"

extern "C" void meLibOnExternalInterrupt(void) {
  // if using meCoreEmitSoftwareInterrupt from sc
}

extern "C" void meLibOnException(void) {
  // optional, handle other custom exceptions here
}

void meLibOnProcess() {
  meLibExceptionHandlerInit(); // init exception handlers
  // ... your ME code
}

int main() {
  const int tableId = meLibDefaultInit();
  // ... main code
  return 0;
}
```

#### General Integration Steps:
1. Configure your build system to include the required directories
2. Include the `me-core.h` library header in your code
3. Implement `meLibOnProcess()` with your Media Engine code
4. Initialize the library in `main()` with `meLibDefaultInit()`
  
See related samples available in the `samples` folder for more information.

### Generate me-core-mapping.h

```bash
awk -f convert-mapping.awk me-core-mapping.def.h > me-core-mapping.c
```

### Generated Library with the embedded Prx
To build the current projet run:
```bash
make clean; make install;
```

This will copy the .a to the project root, which embeds the small PRX in a dedicated data section.  
At runtime, the library will extract the PRX next to the EBOOT and then load it.

## Disclamer
This project and code are provided as-is without warranty. Users assume full responsibility for any implementation or consequences. Use at your own discretion and risk

## Related work
[PSP Media Engine Reload](https://github.com/mcidclan/psp-media-engine-reload)

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
