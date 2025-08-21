## PSP Media Engine's Core Mapper

A project to map the PSP's Media Engine core functions with the purpose of using them in our code.  
The idea is simply to make the native kernel/core functions of the media engine available to homebrew developers.  

The media engine core is loaded at address 0x88300000, however depending on the device model, it is loaded from a different image which means that addresses are not the same for models prior to the slim.
As a solution, we have several static correspondence tables and teh code will select the table corresponding to our device before launching the Media Engine.

## Usage
Build it with CMake, then copy the EBOOT.PBP and kcall.prx (available in the /build/kernel folder) into the same folder within the GAME directory.  

Note: The provided `main.cpp` is experimental and serves as an entry point for manual testing and exploration, allowing functions to be called and their behavior observed to determine how they can be integrated into homebrew projects.

## Disclamer
This project and code are provided as-is without warranty. Users assume full responsibility for any implementation or consequences. Use at your own discretion and risk


*m-c/d*
