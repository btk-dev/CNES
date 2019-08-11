#pragma once

#include <vector>

using BYTE = unsigned char;

//used to communicate between the PPU and the CPU

class mainbus {
public:

	 BYTE read(BYTE address);
	 void write(BYTE address, BYTE data);

	 //add method to poll and pull all registers for beginning of PPU and CPU cycles for any changes.
	 //I don't like this system but I feel like I have kind of backed myself into this corner
	 //though that may just be the alcohol.
	 //may change for function that has bool to determine if there is a change, and if there is just return the changed register
	 std::vector<BYTE> poll();
	 void Init();

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