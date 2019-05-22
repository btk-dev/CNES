#pragma once

#include <vector>
#include <fstream>
#include <errno.h>

typedef unsigned char BYTE;

//.h file for the game rom, data from physical cartridge

class Cartridge {
public:
	Cartridge();
	~Cartridge() {};

	std::vector<BYTE> loadGame(const char* rom_file);

	bool verifyINES(std::vector<BYTE> header);

	BYTE returnMapperInfo();
	int returnPGR();
	int returnCHR();
	//something like that to access console information?
	//extern Console c;

	static int PRGROM;
	static int CHRROM;
	static BYTE mapperInfo;
private:
	unsigned int rom_file[0x4000];
};