#include "mainbus.h"

BYTE mainbus::PPUCTRL = 0;
BYTE mainbus::PPUMASK = 0;
BYTE mainbus::PPUSTATUS = 0;
BYTE mainbus::OAMADDR = 0;
BYTE mainbus::OAMDATA = 0;
BYTE mainbus::PPUSCROLL = 0;
BYTE mainbus::PPUADDR = 0;
BYTE mainbus::PPUDATA = 0;
BYTE mainbus::OAMDMA = 0;

void mainbus::Init() {
	mainbus::OAMADDR = 0;
	OAMDATA = 0;
	OAMDMA = 0;
	PPUADDR = 0;
	PPUCTRL = 0;
	PPUDATA = 0;
	PPUMASK = 0;
	PPUSCROLL = 0;
	PPUSTATUS = 0;
}

BYTE mainbus::read(int reg)
{
	BYTE returnValue;
	switch (reg) {
	case 2002:
		//PPU status
		returnValue = PPUSTATUS;
		break;
	case 2004:
		//OAM data
		returnValue = OAMDATA;
		break;
	case 2007:
		//ppu data
		returnValue = PPUDATA;
		break;
	case 4014:
		//oam dma
		returnValue = OAMDMA;
		break;
	default:
		returnValue = 0;
		break;
	}
	return returnValue;
}

void mainbus::write(int reg, BYTE data)
{
	switch (reg) {
	case 2002:
		//PPU status
		PPUSTATUS = data;
		break;
	case 2004:
		//OAM data
		OAMDATA = data;
		break;
	case 2007:
		//ppu data
		PPUDATA = data;
		break;
	case 4014:
		//oam dma
		OAMDMA = data;
		break;
	default:
		break;
	}
}

std::vector<BYTE> mainbus::poll() {
	std::vector<BYTE> reg;
	reg.push_back(PPUCTRL);
	reg.push_back(PPUMASK);
	reg.push_back(PPUSTATUS);
	reg.push_back(OAMADDR);
	reg.push_back(PPUDATA);
	reg.push_back(PPUSCROLL);
	reg.push_back(PPUADDR);
	reg.push_back(OAMDMA);

	return reg;
}
