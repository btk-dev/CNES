#pragma once

#include <vector>
#include <fstream>
#include <errno.h>

//typedef unsigned char BYTE;

//.h file for the game rom, data from physical cartridge

class Cartridge {
public:
	Cartridge();
	~Cartridge();

	std::vector<unsigned short> loadGame(const char* rom_file);

	bool verifyINES(std::vector<unsigned short> header);

	unsigned short returnMapperInfo();
	unsigned short returnPGR();
	unsigned short returnCHR();
	//something like that to access console information?
	//extern Console c;

	unsigned short PRGROM;
	unsigned short CHRROM;
	unsigned short mapperInfo;
private:
	unsigned int rom_file[0x4000];
};