#pragma once

#include <vector>

using BYTE = unsigned char;

//used to communicate between the PPU and the CPU

class BUS {
public:

	 void read(BYTE, unsigned int);
	 void write(BYTE, unsigned int);

private:
};