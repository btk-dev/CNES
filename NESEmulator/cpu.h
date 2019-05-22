#pragma once

typedef unsigned short BYTE;

class CPU {
public:
	CPU();
	~CPU();

	void Init(CPU*);

	void Clock_Tick(CPU*);

	bool Reset(CPU*);
private:
	//clock cycle of 1.79 MHz (1.66 on PAL)
	//clock divisor d = 12 (16 on PAL)
	//struct registers {
		//unsigned short A; //accumulator
		//unsigned short X; //index
		//unsigned short Y; //index
		//unsigned short PC; //program counter
		//unsigned short SP; //stack counter
		//unsigned short P; //flag register. Composed of 6 1-bit registers
		//order of flags to bits - negative, overflow, B flag (no cpu effect) takes 2 bits, decimal, interrupt disable, zero, carry
	//};
	BYTE A;
	BYTE X;
	BYTE Y;
	unsigned short PC;
	unsigned short SP;
	//unsigned short P;
	bool carryFlag;
	bool zeroFlag;
	bool interruptDisable;
	bool decimalModeFlag;
	bool breakCommand;
	bool overflowFlag;
	bool negativeFlag;

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
	//Common
	//$6000 - $7FFF - Battery backed save or work ram
	//$8000 - $FFFF - Usual ROM
	//CPU expects
	//$FFFA-$FFFB - NMI vector
	//$FFFC-$FFFD - Reset vector
	//$FFFE-$FFFF - IRQ/BRK vector
	BYTE opcode;
};
