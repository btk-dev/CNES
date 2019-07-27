#pragma once

#include <vector>

using BYTE = unsigned char;

//used to communicate between the PPU and the CPU

class mainbus {
public:

	 BYTE read(BYTE address);
	 void write(BYTE address, BYTE data);

private:

	//All the PPU registers that can be read/written to by the CPU
	BYTE PPUCTRL;
	BYTE PPUMASK;
	BYTE PPUSTATUS;
	BYTE OAMADDR;
	BYTE OAMDATA;
	BYTE PPUSCROLL;
	BYTE PPUADDR;
	BYTE PPUDATA;
	BYTE OAMDMA;

};