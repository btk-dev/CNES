#pragma once
#include <vector>

#include "bus.h"
#include "ppubus.h"

//class to hold graphics processing unit for the NES
//currently have no idea what I am doing with graphics drawing, will need to practice using SDL

using BYTE = unsigned char;

class PPU {
public:

	void Init();

	void load_graphics(std::vector<BYTE>);

	void Clock_Tick();

	bool Reset();

private:

	//$0000 - 1FFF - mapped by the cartridge with chr-rom
	//$2000 - 2FFF - mapped to the 2kB NES internal VRAM, these are drawn to the screen and mirrored
	//$3000 - 3EFF - usually a mirror of the 2kB region above. Not used much
	//$3F00 - 3FFF - not configuratble, always mapped to internal palette control

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
	//$00, 04, 08, 0C -> sprite Y coordinate - 1
	//$01, 05, 09, 0D -> sprite tile #
	//$02, 06, 0A, 0E -> sprite attribute
	//$03, 07, 0B, 0F -> sprite X coordinate
	unsigned int internalmemory[0xFF];

	//PPU contains 32 byte secondary OAM not accessible to the program
	//during each visible scanline this is cleared and then a linear search of primary OAM to find sprites that are within y range for the next scanline.
	//oam data for each sprite found to be within range is copied into the secondary oam which is then used to initialize 8 internal sprites
	BYTE secretOAM[0x20];

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

	//PPU mode type
	BYTE ppuMode;

	//if rendering is enabled
	bool rendering;

	//frame toggles between odd and even
	bool oddFrame;

	//determine if it is the first or second write
	bool firstWrite;

	//x fine scroll. Determined by $2005
	int xScroll;

	//which line of the screen is being rendered
	int scanline;

	//cycle number
	int cycle;

	//read registers
	void readRegisters();

	//write registers
	void writeRegisters();

	//8 registers for sprite info
	BYTE spriteReg1;
	BYTE spriteReg2;
	BYTE spriteReg3;
	BYTE spriteReg4;
	BYTE spriteReg5;
	BYTE spriteReg6;
	BYTE spriteReg7;
	BYTE spriteReg8;

	//8 counters for sprite rendering
	BYTE spriteCounter1;
	BYTE spriteCounter2;
	BYTE spriteCounter3;
	BYTE spriteCounter4;
	BYTE spriteCounter5;
	BYTE spriteCounter6;
	BYTE spriteCounter7;
	BYTE spriteCounter8;

	//8 latches for sprite info
	BYTE spriteLatch1;
	BYTE spriteLatch2;
	BYTE spriteLatch3;
	BYTE spriteLatch4;
	BYTE spriteLatch5;
	BYTE spriteLatch6;
	BYTE spriteLatch7;
	BYTE spriteLatch8;

	//8 latches for sprite attributes
	BYTE spriteAttrib1;
	BYTE spriteAttrib2;
	BYTE spriteAttrib3;
	BYTE spriteAttrib4;
	BYTE spriteAttrib5;
	BYTE spriteAttrib6;
	BYTE spriteAttrib7;
	BYTE spriteAttrib8;

	//patternTable1 is in memory $0000-$0FFF
	//patternTable2 is in memory $1000-$1FFF
	//maybe I don't need these and I can just read directly from memory
	BYTE patternTable1[0xFF];
	BYTE patternTable2[0xFF];

	//palette is in VRAM $3F00-$3F1F
	//3F00 universal background color
	//3F01-3F03 palette 1, 3F05 - $3f07 #2, 3F09 - 3F0B # 3, 3F0D, 3F0F #4, every fourth is mirror of 3F00
	//3F11 - 3F13 sprite palette 1, 3F15 - 3F17 #2, 3F19 - 3F1B #3, 3F1D-3F1F #4
	//ones in the middle of the sprite palettes are mirrors of the ones in between palettes. Super Mario Bros uses data at 3F10 and if not implemented right the sky will be black.
	BYTE palette[0xFF];

	//4 nametables arranged in a 4x4 grid. Usually, two are mirrors of the other 2
	//They are in internal memory starting at locations, $2000, 2400, 2800, 2C00 from top left to bottom right.
	BYTE nametable1[0x1000];
	BYTE nametable2[0x1000];
	BYTE nametable3[0x1000];
	BYTE nametable4[0x1000];

	//64 byte table at the end of each nametable, controls palette assigned to the background.
	//is located inside the nametable with #1 being at 23C0. Maybe don't need
	//each byte controls palette of a 32x32 picel part, divided into 4 2-bit areas. (the size of a block in mario)
	//Give palette number topleft, topright, bottomleft, bottomright each between 0 and 3 the value of the byte it
	//value = (bottomright) << 6 | (bottomleft << 4) | (topright << 2) | (topleft << 0)
	BYTE attributeTable[0x64];

	//I think that the NES color palette will be hard coded to SDL colors because they are in the last part of VRAM and will never change.
};