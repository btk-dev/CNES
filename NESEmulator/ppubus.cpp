#include "ppubus.h"

void ppubus::write(BYTE address, BYTE data)
{
	//write to PPU bus
}

void ppubus::read(BYTE)
{
	//read from bus
}

void ppubus::setMirroring()
{
	//if mirror horizontally then nametable 0 and 1 = 0;
	//and nametable 2 = 3 = 0x400


	//if mirrored vertically then nametable 0 = nametable 2 = 0
	//and nametable 1 = 3 = 0x400

	//if one screen then nametable 0 = 1 = 2 = 3 = 0

	//if one screen higher than nametable 0 = 1 = 2 = 3 = 0x400
}
