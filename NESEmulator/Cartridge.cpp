#include "cartridge.h"

Cartridge::Cartridge()
{

}

Cartridge::~Cartridge() {

}

std::vector<BYTE> Cartridge::loadGame(const char* rom_file) {
	std::streampos fileSize;
	std::ifstream file(rom_file, std::ios::binary);

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<BYTE> fileData(fileSize);
	file.read((char*)& fileData[0], fileSize);

	return fileData;
}


bool Cartridge::verifyINES(std::vector<BYTE> header) {
	bool isValid = false;

	//header map in bytes
	//0 - 3 Constant $4E $45 $53 $1A "NES followed by MS-DOS end-of-file
	//4 - size of program rom in 16KB units (16384 * x = value in header)
	//5 - size of CHR rom in 8kb units (8192 * x = value in header. 0 means board uses CHR RAM)
	//6 - mapper, mirror, batter, trainer flags
	//7 - mapper, vs/playchoice, NES 2.0 flags
	//8 - PRG-RAM size (rarely used)
	//9 - TV system (rarely used)
	//10 - TV system, PRG-RAM prescence (unofficial, rarely used)
	//11-15 unused padding (either filled with 0s or rippers name)
	std::string constant;
	for (int i = 0; i <= 2; i++)
		constant += header[i];
	if (constant == "NES" && header[3] == 0x1A)
		isValid = true;

	Cartridge::PRGROM = header[4];
	Cartridge::CHRROM = header[5];

	Cartridge::mapperInfo = (header[7] & 0xF0) >> 4;
	Cartridge::mapperInfo = (Cartridge::mapperInfo & (header[7] & 0xF0));

	return isValid;
}

BYTE Cartridge::returnMapperInfo()
{
	return Cartridge::mapperInfo;
}

BYTE Cartridge::returnPGR()
{
	return Cartridge::PRGROM;
}

BYTE Cartridge::returnCHR()
{
	return Cartridge::CHRROM;
}
