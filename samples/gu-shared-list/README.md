## PSP Media Engine GU's Shared List

A sample demonstrating how to write a GE list from the Media Engine.  

The main idea is to write/update a GE list from the Media Engine and then signal its execution to the Graphics Engine from the SC side.
This is done through a shared uncached double buffer list, ensuring that the GE can read from the range that has been filled, while the ME can continue to write/update to the range that is not currently being read.  

This allows drawing commands to be written concurrently to the Graphics Engine from both CPUs.  

## Usage
Build it with CMake, then copy the EBOOT.PBP into a folder within the GAME directory.  


## Disclamer
This project and code are provided as-is without warranty. Users assume full responsibility for any implementation or consequences. Use at your own discretion and risk


*m-c/d*
