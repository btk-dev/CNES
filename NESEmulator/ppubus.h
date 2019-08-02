#pragma once

//class to store the ppu address bus contents

using BYTE = unsigned char;

class ppubus {
public:
	void write(BYTE, BYTE);
	void read(BYTE);

	void setMirroring();
private:
};
