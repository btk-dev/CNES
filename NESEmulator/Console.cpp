#include "Console.h"

Console::Console() {}

Console::~Console() {}

void Console::powerUpConsole() {}

void Console::createCPU() {
	this->cpu = CPU();
	this->cpu.Init();
}

void Console::resetConsole(CPU* cpu) {
	cpu->Reset();
}

BYTE Console::getPGRInfo(Cartridge c) {
	return c.returnPGR();
}

BYTE Console::getCHRInfo(Cartridge c) {
	return c.returnCHR();
}

BYTE Console::getMapperInfo(Cartridge c) {
	return c.returnMapperInfo();
}

void Console::loadCartridge(const char* file_path) {
	Cartridge c = this->cart;
	std::vector<BYTE> game = c.loadGame(file_path);
	//have to actually call the verify game function in cartridge
	std::vector<BYTE> header(game.begin(), game.begin() + 16);

	c.verifyINES(header);

	//number of pages for program rom
	//comes in 16Kb pieces
	BYTE pgr = Console::getPGRInfo(c);
	
	//number of pages for chr rom
	//chr is for the ppu sprite information
	//comes in 8kb pieces
	BYTE chr = Console::getCHRInfo(c);

	BYTE mapper = Console::getMapperInfo(c);

	game.erase(game.begin(), game.begin() + 16);

	//for (int i = 0; i < game.size(); i++) {
		//Console::memory[0x4020 + i] = game[i];
	//}
	this->cpu.loadGame(game, pgr, chr);
}

bool Console::is_Running() {
	//check if console is still running
	return true;
}