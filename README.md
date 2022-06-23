
# stm32f466_devel
This repository is part of an ongoing effort to build audio synthesizers and controllers using standardized parts.

## Organization
By layer,
* apps -- production and test apps exist.
* FreeRTOS -- extracted from cubemx and compiled as archive.
* modules -- logging, midi, seven segment video, panel stuff (uCModules).  Module folder pattern should be followed.
* bsp -- wraps STM project and provides sole point of entry for the outer world.
  * bsp_clock, bsp_dot2, bsp_common -- implementations of bsp interface.
  * CubeProject -- Unmodified generated code from CubeMX, includes project files.
  * UserProject -- Minimally modified generated code, compare new generations of CubeProject to this and carefully import important deltas.

## Development Workflow
The stm32f466_devel repository is used to build and test individual parts, though it does have a few applications.  It really shouldn't contain *any* applications, but while in development they can stay here.  When specialized hardware is ready, or a family of similar products are to be made, this repo should be forked, and apps built up for that family.

In the future, an app should be built from an independent repo that uses this one (or a release of this one) from another location.  Right now, pathing in the makefiles will probably be incorrect if this is attempted.

Example: add a CRT controller to the project and write apps for a few recycled tubes.
1. Create a board that identifies the IO required for the applications
2. Extend the bsp.h interface to include the new functions required
3. Write the bsp implementations to support this
4. Write a 'module' that connects to the bsp and has helpful non-hw specific things, like position setting, memory page management, primitives.  This module shall have a test folder with an app that uses the module.  It should link to the logging module if appropriate, rather than calling the bsp printf directly.
5. Fork the repository (Note 1)
6. Build an app that uses for new module for each tube, display type, etc
7. The new repository is now its own contained thing.  You are free to make changes for the new family of apps, but if changes are deemed standard, they should be placed on a branch of this repository as well and brought back into the development branch.

*Note 1: if the board is fundamentally different (would need breaking changes to the generated cubemx project), the forking should happen earlier and the module would not be common to development.  This example is esoteric, smaller changes would be developed straight in this branch and may not even need apps apart from test.*

## Toolchain
TODO: this section needs to be run again and written up.
* Install gcc-arm-none-eabi.  Point the makefiles here...  Standard location: `C:\STM32\gcc-arm-none-eabi\bin\arm-none-eabi-<tool>.exe`
* Install openocd.  Add path to environment variables.  Example: `C:\Users\marshall\AppData\Roaming\GNU MCU Eclipse\OpenOCD\0.10.0-8-20180512-1921\bin`.  make will invoke openocd.exe without path.

## Commands
Run these commands from the app or test folder through cygwin.

`make` -- build the application

`make info` -- print object and include lists for debugging

`make openocd` -- Start the openocd server to maintain a link with the hw.  This usually doesn't need to be restarted

`make gdb` -- Gives command to load program and start debugging.  Make environment can't launch gdb properly here.

## Boards
Most basically, all boards in this repository shall have the following serial IO characteristics.

Reroute HID serial to uart 6. 115200 baud
* Open jumpers SB13 and SB14
* Close jumpers SB62 and SB63
* Link CN3, RX to CN10, pin 4
* Link CN3, TX to CN10, pin 19

Uart 2 is midi. 31500 baud
*Arduino pins D0/1

Each board shall have a readme for pin maps in their repsective board folders.
* TODO: board specs for bsp_dot2
* TODO: board specs for bsp_clock
