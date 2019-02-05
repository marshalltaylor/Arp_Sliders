

# MIDI clock


## Status
Pretty healthy rendition of the midi clock

* Needs measured timers
* Needs output serial generation
* rx crash still exists.

## General Commands
Use `make` to build.

Use `make info` to see the object and include lists.

To debug, run 2 cmd windows

OpenOCD:

`C:\Users\marshall>openocd -f "E:\github\something\something\usbDebug.cfg" -c "init; reset halt"`

gcb (run from make directory)

`C:/STM32/gcc-arm-none-eabi/bin/arm-none-eabi-gdb -x gdbinit.txt build/output.elf`