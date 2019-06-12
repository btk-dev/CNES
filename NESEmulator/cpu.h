#pragma once
#include <vector>
#include <iostream>

#include "Instructions.h"

typedef unsigned char BYTE;

class CPU {
public:
	CPU();
	~CPU();

	void Init();

	void Clock_Tick();

	bool Reset();

	void loadGame(std::vector<BYTE> game);

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
	bool bFlag1;
	bool bFlag2;
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
	unsigned int memory[0xFFFF];
	//Stack starts at 0xFF and goes down
	unsigned int stack[0xFF];
	//Common
	//$6000 - $7FFF - Battery backed save or work ram
	//$8000 - $FFFF - Usual ROM
	//CPU expects
	//$FFFA-$FFFB - NMI vector
	//$FFFC-$FFFD - Reset vector
	//$FFFE-$FFFF - IRQ/BRK vector
	BYTE opcode;

	//number of cycles to idle for
	int idleCycles;

	bool isPageCrossed(BYTE, BYTE);

	Instructions I;
	Instructions::Instruction inst;

	void SetZN(BYTE);

	void LDA(Instructions::Instruction);
	void LDX(Instructions::Instruction);
	void LDY(Instructions::Instruction);
	void STA(Instructions::Instruction);
	void STX(Instructions::Instruction);
	void STY(Instructions::Instruction);
	void TAX(Instructions::Instruction);
	void TAY(Instructions::Instruction);
	void TXA(Instructions::Instruction);
	void TYA(Instructions::Instruction);
	void TSX(Instructions::Instruction);
	void TXS(Instructions::Instruction);
	void PHA(Instructions::Instruction);
	void PHP(Instructions::Instruction);
	void PLA(Instructions::Instruction);
	void PLP(Instructions::Instruction);
	void AND(Instructions::Instruction);
	void EOR(Instructions::Instruction);
	void ORA(Instructions::Instruction);
	void BIT(Instructions::Instruction);
	void ADC(Instructions::Instruction);
	void SBC(Instructions::Instruction);
	void CMP(Instructions::Instruction);
	void CPX(Instructions::Instruction);
	void CPY(Instructions::Instruction);
	void INC(Instructions::Instruction);
	void INX(Instructions::Instruction);
	void INY(Instructions::Instruction);
	void DEC(Instructions::Instruction);
	void DEX(Instructions::Instruction);
	void DEY(Instructions::Instruction);
	void ASL(Instructions::Instruction);
	void LSR(Instructions::Instruction);
	void ROL(Instructions::Instruction);
	void ROR(Instructions::Instruction);
	void JMP(Instructions::Instruction);
	void JSR(Instructions::Instruction);
	void RTS(Instructions::Instruction);
	void BCC(Instructions::Instruction);
	void BCS(Instructions::Instruction);
	void BEQ(Instructions::Instruction);
	void BMI(Instructions::Instruction);
	void BNE(Instructions::Instruction);
	void BPL(Instructions::Instruction);
	void BVC(Instructions::Instruction);
	void BVS(Instructions::Instruction);
	void CLC(Instructions::Instruction);
	void CLD(Instructions::Instruction);
	void CLI(Instructions::Instruction);
	void CLV(Instructions::Instruction);
	void SEC(Instructions::Instruction);
	void SED(Instructions::Instruction);
	void SEI(Instructions::Instruction);
	void BRK(Instructions::Instruction);
	void NOP(Instructions::Instruction);
	void RTI(Instructions::Instruction);
};
