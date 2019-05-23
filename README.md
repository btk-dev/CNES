# CNES

A C++ NES emulator written in Visual Studio.

## NES

The NES was a video game console created in the late 1980s by Nintendo and was the beginning of the home video game generation.

### Why

After making the Chip-8 interpreter I decided to try and create an NES emulator as they are extremely well documented online and there
are a number of other NES emulators already existing online. 
The NES is a fairly basic system by modern standards, having only 126 official operations and a small amount of memory for programs and internal systems.

### Roadmap
Parse iNes format roms
Create virtual cartridge
Create CPU
    Set up memory map
    Hardware structure - registers
    Cycle loop
        Read/Writing
        Addressing Modes
        Decoding opcodes
    Interrupts
Using Test Roms ensure CPU is working as intended
PPU
  Basic rendering (using SDL)
  PPU registers
  Core Rendering Loop
Ensure PPU working with Test Roms
APU
  APU registers and memory map
  Sound Output with SDL
Run APU test roms
Input/Output with controllers using SDL
Add other mappers to increase number of games that can be run.

## Secondary Objectives
Load files through SDL window instead of hard coding
Pause/Fast forward
Run/Pause
Saving/Save-states
Config File
  Window Size
  Controls
  Rom Path
Movie Recording
Game Genie
Debugger
Unofficial Opcodes
