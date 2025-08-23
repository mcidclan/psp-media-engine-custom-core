## PSP Media Engine's Core Mapper
A project to map the PSP's Media Engine core functions with the purpose of using them in our code.  
The idea is simply to make the native kernel/core functions of the media engine available to homebrew developers.  

The media engine core is loaded at address 0x88300000, however depending on the device model, it is loaded from a different image which means that addresses are not the same for models prior to the slim.
As a solution, we have several static correspondence tables and teh code will select the table corresponding to our device before launching the Media Engine.

## Usage
See related samples availables in the `samples` folder

## Disclamer
This project and code are provided as-is without warranty. Users assume full responsibility for any implementation or consequences. Use at your own discretion and risk

## Thanks
Thanks to:
- All contributors to the PSP Hardware Register's page + the wiki
- Anyone else who contributed through documentation or reverse engineering
- z2442 for the idea!

*m-c/d*
