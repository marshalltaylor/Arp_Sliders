Done:

TODO:
Configure cubeMX
Compare scope app for
  File structure
  MVC
  panal app -> task
  globals.cpp
  SPI usage with panelComponents
Write basic settings editor
Get GPIO timing and tune tasks

Tasks

The taskSystem does the parsing of the serial now.  When it has a full line,
it starts taskConsole which parses using the command table.  If found, the
command is run from "/commands".

Commands
To add a command,
1. build command

Create a file in /mvc/commands.  Convention is <command string><General description word> as in "adcTest.cpp"

/mvc/commands/commands.h provides the interface interface.
Here's an example with comments:

commandInfo_t adcCmd = {
    "adc",   //This is the command that must be entered to run
    "ADC Test",   //This is a short 1 line description for the general help
    " usage:\n  adc scan",   //As much help as you want!
    &adcProgram
};

adcProgram is the code of the command.  Form int (*fnptr)(int argc, char *argv[]);

2. Add to project makefile
3. Add extern command to /mvc/commands/commands.h
extern commandInfo_t adcCmd;
4. Use adcCmd in the table of (/mvc/commands/commands.c) and extend count.


