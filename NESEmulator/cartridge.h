#pragma once

#include <vector>
#include <fstream>
#include <errno.h>

using BYTE = unsigned char;

//.h file for the game rom, data from physical cartridge

class Cartridge {
public:
	Cartridge();
	~Cartridge();

	std::vector<BYTE> loadGame(const char* rom_file);

	bool verifyINES(std::vector<BYTE> header);

	BYTE returnMapperInfo();
	BYTE returnPGR();
	BYTE returnCHR();
	//something like that to access console information?
	//extern Console c;

	BYTE PRGROM;
	BYTE CHRROM;
	BYTE mapperInfo;
private:
	unsigned int rom_file[0x4000];
};