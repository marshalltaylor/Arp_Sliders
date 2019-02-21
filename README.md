# Arp Sliders
This device generates arpeggiations on input midi notes.  A second keyboard can be used to configure the arpeggiations for when a looper is at the other channel.  The hardware is a STM32F466 dev board with midi shield and controls.

## Status
Work in progress!

* Core -- healthy.  A cubeMX project can be generated independently of the project and compared.
* Serial -- Debug interface shows basic data
* Midi -- Working but doesn't really do anything
* ADC -- On but not really configured, now 6 channels are active
* Switches -- panel buttons for now but active

## General Commands
Use `make` to build.

Use `make info` to see the object and include lists.

To debug, run 2 cmd windows

OpenOCD:

`C:\Users\marshall>openocd -f "E:\github\something\something\usbDebug.cfg" -c "init; reset halt"`

gcb (run from make directory)

`C:/STM32/gcc-arm-none-eabi/bin/arm-none-eabi-gdb -x gdbinit.txt build/output.elf`

## Architecture

<img src="https://raw.githubusercontent.com/marshalltaylor/Arp_Sliders/master/Documentation/dataflow.png">
