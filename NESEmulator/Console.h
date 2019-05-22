#pragma once

#include "cpu.h"
#include "cartridge.h"

//.h file for the console, the entire NES with CPU, PPU, etc

class Console {
public:

	//cartridge methods
	std::vector<BYTE> loadCartridge(const char* file_path);
	BYTE getMapperInfo();
	int getPGRInfo();
	int getCHRInfo();

	//cpu methods
	CPU createCPU();
	void resetConsole(CPU*);
private:
	//memory map
	//$0000 - $07FF 2KB internal RAM
	//$0800 - $0FFF - mirror of RAM
	//$1000 - $17FF - mirror of RAM
	//$1800 - $1FFF - mirror of RAM
	//$2000 - $2007 - NES PPU registers
	//$2008 - $3FFF - mirrors of PPU registers(repeat every 8 bytes)
	//$4000 - $4017 - NES APU and I/O registers
	//$4018 - $401F - APU and I/O functionality. Normally disabled
	//$4020 - $FFFF - Cartridge space. PRG ROM, PRG RAM and mapper registers
	unsigned int memory[0x10000];
};