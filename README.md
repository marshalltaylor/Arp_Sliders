# MIDI clock
The midi clock is a device that can accept and generate midi clock messages.  The hardware is a STM32F466 dev board with midi shield and controls.
## Status
Tested with Roland products and is working.  The 7 segment driver is pretty crazy, but not really a reusable part.  The project really focused on creating a clock object that can be fit into a socket object, with callbacks both ways.  This forced removal of control mechanisms from the clock object making it purely reactive.  The controls are wired to the socket as an interface.
* Core -- healthy.  A cubeMX project can be generated independently of the project and compared.
* Serial -- Debug interface shows basic data
* Midi -- OK but edge cases haven't been found.  Needs bench testing
* ADC -- On but not really configured
* Display -- Crazy, over-baked framebuffer objects

## General Commands
Use `make` to build.

Use `make info` to see the object and include lists.

To debug, run 2 cmd windows

OpenOCD:

`C:\Users\marshall>openocd -f "E:\github\something\something\usbDebug.cfg" -c "init; reset halt"`

gcb (run from make directory)

`C:/STM32/gcc-arm-none-eabi/bin/arm-none-eabi-gdb -x gdbinit.txt build/output.elf`
