#pragma once

//class to hold graphics processing unit for the NES

using BYTE = unsigned char;

class PPU {
public:

	void Init();

	void Clock_Tick();

	bool Reset();

private:

	//$0000 - 1FFF - mapped by the cartridge with chr-rom
	//$2000 - 2FFF - mapped to the 2kB NES internal VRAM, these are drawn to the screen and mirrored
	//$3000 - 3EFF - usually a mirror of the 2kB region above. Not used much
	//$3F00 - 3FFF - not configuratble, always mapped to internall palette control

	//memory map for PPU memory
	//$0000 - $0FFF -> Pattern table 0
	//$1000 - $1FFF -> Pattern table 1
	//$2000 - $23FF -> Nametable 0
	//$2400 - $27FF -> Nametable 1
	//$2800 - $2BFF -> Nametable 2
	//$2C00 - $2FFF -> Nametable 3
	//$3000 - $3EFF -> Mirrors of $2000 - $2EFF
	//$3F00 - $3F1F -> Palette RAM indexes
	//$3F20 - $3FFF -> Mirrors of $3F00 - $3F1F
	unsigned int memory[0x3FFF];

	//PPU contains internal memory to determine sprite rendering
	//$00, 04, 08, 0C -> sprite Y coordinate
	//$01, 05, 09, 0D -> sprite tile #
	//$02, 06, 0A, 0E -> sprite attribute
	//$03, 07, 0B, 0F -> sprite X coordinate
	unsigned int internalmemory[0xFF];

	//PPU exposes 8 memory mapped registers to the CPU (usually sit at $2000 through $2007 in CPU memory and mirrored every 8 bytes from 2008 - 3FFF.)
	//will probably use setters and getters in bus class.

	//Address $2000. Bits - VPHB SINN. NMI enable (V), PPU master/slave (P), sprite height (H), background tile select (B), sprite tile select (S), increment mode (I), nametable select (NN)
	BYTE PPUCTRL;
	//Address $2001. Bits BGRs bMmG. color emphasis (BGR), sprite enable (s), background enable (b), sprite left column enable (M), background left column enable (m), greyscale (G)
	BYTE PPUMASK;
	//Address $2002. Bits VSO- ----. vblank (V), sprite 0 hit (S), sprite overflow (O), read resets write pair for $2005/$2006
	BYTE PPUSTATUS;
	//Address $2003. Bits aaaa aaaa. OAM read/write address
	BYTE OAMADDR;
	//Address $2004. Bits dddd dddd. OAM data read/write
	BYTE OAMDATA;
	//Address $2005. Bits xxxx xxxx. fine scroll position (2 writes: X scroll, Y scroll)
	BYTE PPUSCROLL;
	//Address $2006. Bits aaaa aaaa. PPU read/write address (2 writes: most significant byte, least sig byte)
	BYTE PPUADDR;
	//Address $2007. Bits dddd dddd. PPU data read/write
	BYTE PPURDATA;
	//Address $4014. Bits aaaa aaaa. OAM DMA high address
	BYTE OAMDMA;

	//frame toggles between odd and even
	bool oddFrame;
};