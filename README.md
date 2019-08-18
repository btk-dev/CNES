# CNES

A C++ NES emulator written in Visual Studio.

## NES

The NES was a video game console created in the late 1980s by Nintendo and was the beginning of the home video game generation. The NES came with 8kB of memory as well as a 1.79MHz processor in North America. Graphics on the system were handled by a custom graphics processing unit called a Picture Processing Unit, PPU, which had it's own 2kB internal memory to hold sprite information.

### Why

After making the Chip-8 interpreter I decided to try and create an NES emulator as they are extremely well documented online and there
are a number of other NES emulators already existing online. 
The NES is a fairly basic system by modern standards, having only 126 official operations and a small amount of memory for programs and internal systems.

### Roadmap
- [x] Parse iNes format roms
- [x] Create virtual cartridge
- []  Create Mapper Interface - Currently only mapper 0 implemented
- [x] Create CPU
    - [x] Set up memory map
    - [x] Hardware structure - registers
    - [x] Cycle loop
          - [x] Read/Writing
          - [x] Addressing Modes
          - [x] Decoding opcodes
    Interrupts
- [x] Using Test Roms ensure CPU is working as intended
- [] PPU
  - [] Basic rendering (using SDL)
  - [x] PPU registers
  - [x] Core Rendering Loop
  - []  Test Rendering Loop
- [] Ensure PPU working with Test Roms - In Progress
- [] APU
     - [] APU registers and memory map
     - [] Sound Output with SDL
- [] Run APU test roms
- [] Input/Output with controllers using SDL
- [] Add other mappers to increase number of games that can be run.

## Secondary Objectives
- [] Load files through SDL window instead of hard coding
- [] Pause/Fast forward
- [] Run/Pause
- [] Saving/Save-states
- [] Config File
      - [] Window Size
      - [] Controls
      - [] Rom Path
- [] Movie Recording
- [] Game Genie
- [] Debugger
- [] Unofficial Opcodes
