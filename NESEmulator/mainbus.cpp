#include "mainbus.h"

void mainbus::Init() {
	this->OAMADDR = 0;
	this->OAMDATA = 0;
	this->OAMDMA = 0;
	this->PPUADDR = 0;
	this->PPUCTRL = 0;
	this->PPUDATA = 0;
	this->PPUMASK = 0;
	this->PPUSCROLL = 0;
	this->PPUSTATUS = 0;
}

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
