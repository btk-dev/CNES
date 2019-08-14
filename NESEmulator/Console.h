#pragma once

#include "cpu.h"
#include "ppu.h"
#include "mainbus.h"
#include "cartridge.h"

//.h file for the console, the entire NES with CPU, PPU, etc

class Console {
public:
	Console();
	~Console();

	//cartridge methods
	void loadCartridge(const char* file_path);
	BYTE getMapperInfo(Cartridge);
	BYTE getPGRInfo(Cartridge);
	BYTE getCHRInfo(Cartridge);

	//cpu methods
	void createCPU();
	void powerUpConsole();
	void resetConsole(CPU*);

	//ppu methods
	void createPPU();

	//annoying mainbus methods I want somewhere else
	void createMainbus();

	bool is_Running();

	CPU cpu;
	PPU ppu;
	Cartridge cart;
	//I really don't want the mainbus here
	mainbus _mainbus;
private:
};