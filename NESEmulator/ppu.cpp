#include "ppu.h"

void PPU::Init()
{
	//at power up 
	//ppucntl = 0000 0000
	//ppumask = 0000 0000
	//ppustatus = +x+x xxxx
	//oamaddr = $00
	//$2005/$2006 latch is cleared
	//ppu scroll = $0000
	//ppuaddr = $0000
	//ppudata = $00
	//odd frame = 0
	//OAM, palette, nt ram, chr ram = unspecifed
	//where ? is unknown, x is irrelevant, + is often set, U = unchanged
	//writes to PPUCTRL, PPUMASK, PPUSCROLL, PPUADDR are ignored for the first 29 658 cycles
	this->PPUCTRL = this->PPUMASK = 0;
}

void PPU::Clock_Tick()
{
	if (this->oddFrame)
		this->oddFrame = false;
	else
		this->oddFrame = true;
}

bool PPU::Reset()
{
	//at reset
	//ppurctrl = 0000 0000
	//ppumask = 0000 0000
	//ppu status = U??x xxxx
	//oamaddr is unchanged
	//$2005/$2006 latch is cleared
	//ppuscroll = $0000
	//ppuaddr is uncahnged
	//ppudata = $00
	//odd frame = no
	//oam is unspecified 
	//palette, nt ram, chr ram = unchanged

	return true;
}
