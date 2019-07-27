#include "mainbus.h"

BYTE mainbus::read(BYTE reg)
{
	BYTE returnValue;
	switch (reg) {
	case 0x2002:
		//PPU status
		returnValue = this->PPUSTATUS;
		break;
	case 0x2004:
		//OAM data
		returnValue = this->OAMDATA;
		break;
	case 0x2007:
		//ppu data
		returnValue = this->PPUDATA;
		break;
	case 0x4014:
		//oam dma
		returnValue = this->OAMDMA;
		break;
	default:
		returnValue = 0;
		break;
	}
	return returnValue;
}

void mainbus::write(BYTE reg, BYTE data)
{
	switch (reg) {
	case 0x2002:
		//PPU status
		this->PPUSTATUS = data;
		break;
	case 0x2004:
		//OAM data
		this->OAMDATA = data;
		break;
	case 0x2007:
		//ppu data
		this->PPUDATA = data;
		break;
	case 0x4014:
		//oam dma
		this->OAMDMA = data;
		break;
	default:
		break;
	}
}
