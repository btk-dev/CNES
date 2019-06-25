#include "cpu.h"

int _InstructionType[0x100] = {
	//0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	  11,  4,  0,  0,  0,  4,  7,  0,  3,  4,  7,  0,  0,  4,  7,  0, //0
	  9,  4,  0,  0,  0,  4,  7,  0,  10,  4,  0,  0,  0,  4,  7,  0, //1
	  8,  4,  0,  0,  4,  4,  7,  0,  3,  4,  7,  0,  4,  4,  7,  0, //2
	  9,  4,  0,  0,  0,  4,  7,  0,  10,  4,  0,  0,  0,  4,  7,  0, //3
	  11,  4,  0,  0,  0,  4,  7,  0,  3,  4,  7,  0,  8,  4,  7,  0, //4
	  9,  4,  0,  0,  0,  4,  7,  0,  10,  4,  0,  0,  0,  4,  7,  0, //5
	  8,  5,  0,  0,  0,  5,  7,  0,  3,  5,  7,  0,  8,  5,  7,  0, //6
	  9,  5,  0,  0,  0,  5,  7,  0,  10,  5,  0,  0,  0,  5,  7,  0, //7
	  0,  1,  0,  0,  1,  1,  1,  0,  6,  0,  2,  0,  1,  1,  1,  0, //8
	  9,  1,  0,  0,  1,  1,  1,  0,  2,  1,  3,  0,  0,  1,  0,  0, //9
	  1,  1,  1,  0,  1,  1,  1,  0,  2,  1,  2,  0,  1,  1,  1,  0, //A
	  9,  1,  0,  0,  1,  1,  1,  0,  10,  1,  3,  0,  1,  1,  1,  0, //B
	  5,  5,  0,  0,  5,  5,  6,  0,  6,  5,  6,  0,  5,  5,  6,  0, //C
	  9,  5,  0,  0,  0,  5,  6,  0,  10,  5,  0,  0,  0,  5,  6,  0, //D
	  5,  5,  0,  0,  5,  5,  6,  0,  6,  5,  11,  0,  5,  5,  6,  0, //E
	  9,  5,  0,  0,  0,  5,  6,  0,  10,  5,  0,  0,  0,  5,  6,  0  //F
};

int _InstructionMode[0x100] = {
	//0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	  2,  12,  0,  0,  0,  8,  8,  0,  3,  2,  1,  0,  0,  5,  5,  0, //0
	  4,  13,  0,  0,  0,  9,  9,  0,  3,  7,  0,  0,  0,  6,  6,  0, //1
	  5,  12,  0,  0,  8,  8,  8,  0,  3,  2,  1,  0,  5,  5,  5,  0, //2
	  4,  13,  0,  0,  0,  9,  9,  0,  3,  7,  0,  0,  0,  6,  6,  0, //3
	  3,  12,  0,  0,  0,  8,  8,  0,  3,  2,  1,  0,  5,  5,  5,  0, //4
	  4,  13,  0,  0,  0,  9,  9,  0,  3,  7,  0,  0,  0,  6,  6,  0, //5
	  3,  12,  0,  0,  0,  8,  8,  0,  3,  2,  1,  0,  11,  5,  5,  0, //6
	  4,  13,  0,  0,  0,  9,  9,  0,  3,  7,  0,  0,  0,  6,  6,  0, //7
	  0,  12,  0,  0,  8,  8,  8,  0,  3,  0,  3,  0,  5,  5,  5,  0, //8
	  4,  13,  0,  0,  9,  9,  9,  0,  3,  7,  3,  0,  0,  6,  0,  0, //9
	  2,  12,  2,  0,  8,  8,  8,  0,  3,  2,  3,  0,  5,  5,  5,  0, //A
	  4,  13,  0,  0,  9,  9,  9,  0,  3,  7,  3,  0,  6,  6,  7,  0, //B
	  2,  12,  0,  0,  8,  8,  8,  0,  3,  2,  3,  0,  5,  5,  5,  0, //C
	  4,  13,  0,  0,  0,  9,  9,  0,  3,  7,  0,  0,  0,  6,  6,  0, //D
	  2,  12,  0,  0,  8,  8,  8,  0,  3,  2,  3,  0,  5,  5,  5,  0, //E
	  4,  13,  0,  0,  0,  9,  9,  0,  3,  7,  0,  0,  0,  6,  6,  0  //F
};

int _InstructionLength[0x100] = {
	//0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	  1,  2,  0,  0,  0,  2,  2,  0,  1,  2,  1,  0,  0,  3,  3,  0, //0
	  2,  2,  0,  0,  0,  2,  2,  0,  1,  3,  0,  0,  0,  3,  3,  0, //1
	  3,  2,  0,  0,  2,  2,  2,  0,  1,  2,  1,  0,  3,  3,  3,  0, //2
	  2,  2,  0,  0,  0,  2,  2,  0,  1,  3,  0,  0,  0,  3,  3,  0, //3
	  1,  2,  0,  0,  0,  2,  2,  0,  1,  2,  1,  0,  3,  3,  3,  0, //4
	  2,  2,  0,  0,  0,  2,  2,  0,  1,  3,  0,  0,  0,  3,  3,  0, //5
	  1,  2,  0,  0,  0,  2,  2,  0,  1,  2,  1,  0,  3,  3,  3,  0, //6
	  2,  2,  0,  0,  0,  2,  2,  0,  1,  3,  0,  0,  0,  3,  3,  0, //7
	  0,  2,  0,  0,  2,  2,  2,  0,  1,  0,  1,  0,  3,  3,  3,  0, //8
	  2,  2,  0,  0,  3,  2,  2,  0,  1,  3,  1,  0,  0,  3,  0,  0, //9
	  2,  2,  2,  0,  2,  2,  2,  0,  1,  2,  1,  0,  3,  3,  3,  0, //A
	  2,  2,  0,  0,  2,  2,  2,  0,  1,  3,  1,  0,  3,  3,  3,  0, //B
	  2,  2,  0,  0,  2,  2,  2,  0,  1,  2,  1,  0,  3,  3,  3,  0, //C
	  2,  2,  0,  0,  0,  2,  2,  0,  1,  3,  0,  0,  0,  3,  3,  0, //D
	  2,  2,  0,  0,  2,  2,  2,  0,  1,  2,  1,  0,  3,  3,  3,  0, //E
	  2,  2,  0,  0,  0,  2,  2,  0,  1,  3,  0,  0,  0,  3,  3,  0  //F
};

int _InstructionCycles[0x100] = {
	//0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	  7,  6,  0,  0,  0,  3,  5,  0,  3,  2,  2,  0,  0,  4,  6,  0,  //0
	  2,  5,  0,  0,  0,  4,  6,  0,  2,  4,  0,  0,  0,  4,  7,  0,  //1
	  6,  6,  0,  0,  3,  3,  5,  0,  4,  2,  2,  0,  4,  4,  6,  0,  //2
	  2,  5,  0,  0,  0,  4,  6,  0,  2,  4,  0,  0,  0,  4,  7,  0,  //3
	  6,  6,  0,  0,  0,  3,  5,  0,  3,  2,  2,  0,  3,  4,  6,  0,  //4
	  2,  5,  0,  0,  0,  4,  6,  0,  2,  4,  0,  0,  0,  4,  7,  0,  //5
	  6,  6,  0,  0,  0,  3,  5,  0,  4,  2,  2,  0,  5,  4,  6,  0,  //6
	  2,  5,  0,  0,  0,  4,  6,  0,  2,  4,  0,  0,  0,  4,  7,  0,  //7
	  0,  6,  0,  0,  3,  3,  3,  0,  2,  0,  2,  0,  4,  4,  4,  0,  //8
	  2,  6,  0,  0,  4,  4,  4,  0,  2,  5,  2,  0,  0,  5,  0,  0,  //9
	  2,  6,  2,  0,  3,  3,  3,  0,  2,  2,  2,  0,  4,  4,  4,  0,  //A
	  2,  5,  0,  0,  4,  4,  4,  0,  2,  4,  2,  0,  4,  4,  4,  0,  //B
	  2,  6,  0,  0,  3,  3,  5,  0,  2,  2,  2,  0,  4,  4,  6,  0,  //C
	  2,  5,  0,  0,  0,  4,  6,  0,  2,  4,  0,  0,  0,  4,  7,  0,  //D
	  2,  6,  0,  0,  3,  3,  5,  0,  2,  2,  2,  0,  4,  4,  6,  0,  //E
	  2,  5,  0,  0,  0,  4,  6,  0,  2,  4,  0,  0,  0,  4,  7,  0   //F
};

int _InstructionPageCrossCycles[0x100] = {
	//0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  //0
	  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  //1
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  //2
	  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  //3
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  //4
	  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  //5
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  //6
	  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  //7
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  //8
	  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  //9
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  //A
	  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  1,  1,  1,  0,  //B
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  //C
	  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  //D
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  //E
	  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0   //F
};

std::string _instructions[0x100] = {
	//0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F
	"BRK", "ORA", "INV", "INV", "INV", "ORA", "ASL", "INV", "PHP", "ORA", "ASL", "INV", "INV", "ORA", "ASL", "INV", //0
	"BPL", "ORA", "INV", "INV", "INV", "ORA", "ASL", "INV", "CLC", "ORA", "INV", "INV", "INV", "ORA", "ASL", "INV", //1
	"JSR", "AND", "INV", "INV", "BIT", "AND", "ROL", "INV", "PLP", "AND", "ROL", "INV", "BIT", "AND", "ROL", "INV", //2
	"BMI", "AND", "INV", "INV", "INV", "AND", "ROL", "INV", "SEC", "AND", "INV", "INV", "INV", "AND", "ROL", "INV", //3
	"RTI", "EOR", "INV", "INV", "INV", "EOR", "LSR", "INV", "PHA", "EOR", "LSR", "INV", "JMP", "EOR", "LSR", "INV", //4
	"BVC", "EOR", "INV", "INV", "INV", "EOR", "LSR", "INV", "CLI", "EOR", "INV", "INV", "INV", "EOR", "LSR", "INV", //5
	"RTS", "ADC", "INV", "INV", "INV", "ADC", "ROR", "INV", "PLA", "ADC", "ROR", "INV", "JMP", "ADC", "ROR", "INV", //6
	"BVS", "ADC", "INV", "INV", "INV", "ADC", "ROR", "INV", "SEI", "ADC", "INV", "INV", "INV", "ADC", "ROR", "INV", //7
	"INV", "STA", "INV", "INV", "STY", "STA", "STX", "INV", "DEY", "INV", "TXA", "INV", "STY", "STA", "STX", "INV", //8
	"BCC", "STA", "INV", "INV", "STY", "STA", "STX", "INV", "TYA", "STA", "TXS", "INV", "INV", "STA", "INV", "INV", //9
	"LDY", "LDA", "LDX", "INV", "LDY", "LDA", "LDX", "INV", "TAY", "LDA", "TAX", "INV", "LDY", "LDA", "LDX", "INV", //A
	"BCS", "LDA", "INV", "INV", "LDY", "LDA", "LDX", "INV", "CLV", "LDA", "TSX", "INV", "LDY", "LDA", "LDX", "INV", //B
	"CPY", "CMP", "INV", "INV", "CPY", "CMP", "DEC", "INV", "INY", "CMP", "DEX", "INV", "CPY", "CMP", "DEC", "INV", //C
	"BNE", "CMP", "INV", "INV", "INV", "CMP", "DEC", "INV", "CLD", "CMP", "INV", "INV", "INV", "CMP", "DEC", "INV", //D
	"CPX", "SBC", "INV", "INV", "CPX", "SBC", "INC", "INV", "INX", "SBC", "NOP", "INV", "CPX", "SBC", "INC", "INV", //E
	"BEQ", "SBC", "INV", "INV", "INV", "SBC", "INC", "INV", "SED", "SBC", "INV", "INV", "INV", "SBC", "INC", "INV"  //F
};

CPU::CPU() {
	this->A = this->X = this->Y = this->SP = this->PC /*= CPU::opcode = CPU::P*/ = 0;
	Init();
}

CPU::~CPU() {}

void CPU::Init() {
	//set P to $34
	//A, X, Y = 0;
	//S = $FD
	//$4017 = $00 (frame irq enabled)
	//$4015 = $00 (all channels disabled)
	//$4000 - $400F = $00
	//all 15 bits of noise channel LFSR = $0000. The first time the LFSR is clocked from the all-0s state it will shift 1
	//internal memory ($0000 - $07FF) is inconsistant. Most emulators set them to a pattern
	//cpu->P = cpu->memory[0x00034];
	//0x000 - 0x07FF is system ram, 0 at start up, 0x0800 - 0x0FFF is a map, 0x1000 - 0x17FF is a map, 0x1800 - 0x1FFF is a map
	for (int i = 0; i <= 8191; i++)
		this->memory[i] = 0x00;
	this->A = this->X = this->Y = 0;
	this->A = this->X = this->Y = this->SP = this->PC /*= CPU::opcode = CPU::P*/ = 0x00;
	this->negativeFlag = 0;
	this->carryFlag = 0;
	this->zeroFlag = 0;
	this->bFlag1 = 0;
	this->bFlag2 = 1;
	this->decimalModeFlag = 0;
	this->interruptDisable = 1;
	this->overflowFlag = 0;
	this->SP = 0xFD;
	this->memory[0x4017] = this->memory[0x0000];
	this->memory[0x4015] = this->memory[0x0000];
	for (int i = 0; i < 16; i++) {
		this->memory[0x04000 + i] = this->memory[0x0000];
	}
	this->memory[383] = 105;
	this->memory[384] = 51;
	for (int i = 0; i < 0xFF; i++)
		this->stack[i] = 0;
}

bool CPU::Reset() {
	//A, X, Y not affected
	//S decremented by 3
	//I flag set to true (ORed with $04)
	this->interruptDisable |= this->memory[0x0004];
	//Internal memory unchanged
	//APU mode in $4017 unchanged
	//APU silenced ($4015 = 0)
	this->SP -= 3;
	//cpu->P & 0x00000F00 | cpu->memory[0x0004];
	return true;
}

void CPU::loadGame(std::vector<BYTE> game, BYTE pgr, BYTE chr) {
	//this needs some work yet. Need to load the file according to prg rom.
	//if pgr only has 1 16kb bank, then mirror the loading into 0xC000, else load second bank into 0xC000 and use map switching
	for (int i = 0; i < game.size(); i++)
		//should actually be this->memory[0x8000] but with nestest this will have to do for now.
		this->memory[0xC000 + i] = game[i];
		//this->memory[0x4020 + i] = game[i];
	//if (pgr == 1)
		//for (int i = 0; i < game.size(); i++)
			//this->memory[0xC000 + i] = game[i];
	//temporary start for program counter for nestest
	this->PC = 0xc000;
	//this->PC = this->memory[0xFFFC] | (this->memory[0xFFFD] << 8);
}

bool CPU::isPageCrossed(BYTE A, BYTE B) {
	if ((A & 0xFF) != (B & 0xFF))
		return true;
	return false;
}

void CPU::writeToMemory(BYTE source, unsigned int destination) {
	//if destination is inside of the program rom then perform a mapper swap
	this->memory[destination] = source;
	SetZN(this->memory[destination]);
}

void CPU::readFromMemory(unsigned int source, BYTE* destination) {
	//this doesn't work as of now
	*destination = this->memory[source];
	SetZN(*destination);
}

void CPU::SetZN(BYTE A) {
	//I think I should clear them if these conditions not met. Probably.
	if (A == 0x0)
		this->zeroFlag = 1;
	else
		this->zeroFlag = 0;
	if ((A & 0x80) == 0x80)
		this->negativeFlag = 1;
	else
		this->negativeFlag = 0;
}

void CPU::handleInterrupt(Instructions::Instruction I) {
	//have to save the values of the A and index registers and restore them afterwards.
	//RESET does not push PC and P to stack
	//NMI, IRQ and BRK all push PC and P to stack.

	//BRK sets B flag(bit 4)
	//NMI, IRQ and RESET do not set the bFlag

	//NMI pulls PC from FFFA/FFFB
	//RESET pulls PC from FFFC/FFFD
	//IRQ/BRK pull PC from FFFE/FFFF
	BRK(I);
	//order of events is thus:
	//BRK:
	//store PC(high)
	//store PC(low)
	//store P
	//fetch PC(low)
	//fetch PC(high)

	//IRQ:
	//same as BRK but clears bFlag before pushing P to stack
	//if there is IRQ pending and current inst has just finished the interupt foces the instruction register to 0 instead of executing next instruction
	//the PLA will decode that as 0x00 which is BRK

	//NMI is done the same as above, but pulls PC from different address

	//RESET does the same as above with these differenes
	//when system is powered on the SP = 0, pull 0 into instruction register
	//first stack access happens at $0100 and then decrement SP to 0xFF
	//repeat so SP is now 0xFE
	//happens again to emulate status register so SP now = 0xFD
	//now read low byte of vector into PC from 0x01FD
	//read high byte of PC again. This is why SP is initially 0xFD

	//after completing interrupt pull PC back to continue operation of program.
}

//Load/Store Operations
void CPU::LDA(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 2:
		//immediate
		//probably have to convert the next line into hex before storing into the accumulator
		this->A = operand;
		//takes 2 cycles
		this->idleCycles = 2;
		//add 2 to program counter to pass all arguments for this instruction
		this->PC += 2;
		SetZN(this->A);
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		//little endian so high memory address is in second byte.
		result = (operand2 << 8) | operand;
		//this->A = this->memory[result];
		readFromMemory(result, &this->A);
		//takes 3 cycles
		this->idleCycles = 3;
		//add 3 to program counter
		this->PC += 3;
		//SetZN(this->A);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		//this->A = this->memory[result];
		readFromMemory(result, &this->A);
		//takes 4 cycles, plus one if page crossed. I don't think the below check will work.
		this->idleCycles = 4;
		if (isPageCrossed(this->memory[result], this->memory[result + this->X]))
			this->idleCycles += 1;
		this->PC += 3;
		//SetZN(this->A);
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->Y;
		//this->A = this->memory[result];
		readFromMemory(result, &this->A);
		//4 cycles, plus one if page crossed
		this->idleCycles = 4;
		if (isPageCrossed(this->memory[result], this->memory[result + this->Y]))
			this->idleCycles += 1;
		this->PC += 3;
		//SetZN(this->A);
		break;
	case 8:
		//zero page. Wraparound if > FF.
		if (operand > 0xFF)
			operand -= 0xFF;
		//this->A = this->memory[operand];
		readFromMemory(operand, &this->A);
		this->idleCycles = 3;
		this->PC += 2;
		//SetZN(this->A);
		break;
	case 9:
		//zero page, X
		result = operand + this->X;
		if (result > 0xFF)
			result -= (0xFF + 1);
		//this->A = this->memory[result];
		readFromMemory(result, &this->A);
		this->idleCycles = 4;
		this->PC += 2;
		//SetZN(this->A);
		break;
	case 12:
		//(indirect, X)
		operand += this->X;
		if (operand> 0xFF)
			operand = operand - (0xFF + 1);
		result = operand + 1;
		if (result > 0xFF)
			result = result - 0xFF - 1;
		result = this->memory[operand] | (this->memory[result] << 8);
		//this->A = this->memory[result];
		readFromMemory(result, &this->A);
		this->idleCycles = 6;
		this->PC += 2;
		//SetZN(this->A);
		break;
	case 13:
		//(indirect), Y
		operand2 = operand + 1;
		result = this->memory[operand] | (this->memory[operand2] << 8);
		result = result + this->Y;
		//this->A = this->memory[result];
		readFromMemory(result, &this->A);
		//extra cycle if page cross
		this->idleCycles = 5;
		if (isPageCrossed(operand, operand2))
			this->idleCycles += 1;
		this->PC += 2;
		//SetZN(this->A);
		break;
	default:
		break;
	}
}
void CPU::LDX(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 2:
		//immediate
		//probably have to convert the next line into hex before storing into the accumulator
		this->X = operand;
		//takes 2 cycles
		this->idleCycles = 2;
		//add 2 to program counter to pass all arguments for this instruction
		this->PC += 2;
		SetZN(this->X);
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		//little endian so high memory address is in second byte.
		result = (operand2 << 8) | operand;
		//this->X = this->memory[result];
		readFromMemory(result, &this->X);
		//takes 3 cycles
		this->idleCycles = 3;
		//add 3 to program counter
		this->PC += 3;
		//SetZN(this->X);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		//this->X = this->memory[result];
		readFromMemory(result, &this->X);
		//takes 4 cycles, plus one if page crossed.
		this->idleCycles = 4;
		if (isPageCrossed(this->memory[result - this->X], this->memory[this->X]))
			this->idleCycles += 1;
		this->PC += 3;
		//SetZN(this->X);
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->Y;
		//this->X = this->memory[result];
		readFromMemory(result, &this->X);
		//4 cycles, plus one if page crossed
		this->idleCycles = 4;
		if (isPageCrossed(this->memory[result], this->memory[result - this->Y]))
			this->idleCycles += 1;
		this->PC += 3;
		//SetZN(this->X);
		break;
	case 8:
		//zero page. Wraparound if > FF.
		if (operand > 0xFF)
			operand -= (0xFF + 1);
		//this->X = this->memory[operand];
		readFromMemory(operand, &this->X);
		this->idleCycles = 3;
		this->PC += 2;
		//SetZN(this->X);
		break;
	case 9:
		//zero page, X
		result = operand + this->Y;
		if (result > 0xFF)
			result -= (0xFF + 1);
		//this->X = this->memory[result];
		readFromMemory(result, &this->X);
		this->idleCycles = 4;
		this->PC += 2;
		//SetZN(this->X);
		break;
	case 12:
		//(indirect, X)
		operand += this->X;
		if (operand > 0xFF)
			operand = operand - (0xFF + 1);
		result = operand + 1;
		if (result > 0xFF)
			result = result - 0xFF - 1;
		result = this->memory[operand] | (this->memory[result] << 8);
		//this->X = this->memory[result];
		readFromMemory(result, &this->A);
		this->idleCycles = 6;
		this->PC += 2;
		//SetZN(this->X);
		break;
	case 13:
		//(indirect), Y
		operand2 = operand + 1;
		result = this->memory[operand] | (this->memory[operand2] << 8);
		//this->X = this->memory[result] + this->Y;
		readFromMemory(result + this->Y, &this->X);
		//extra cycle if page cross. Yet to add
		this->idleCycles = 5;
		if (isPageCrossed(operand, operand2))
			this->idleCycles += 1;
		this->PC += 2;
		//SetZN(this->X);
		break;
	default:
		break;
	}
}
void CPU::LDY(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 2:
		//immediate
		//probably have to convert the next line into hex before storing into the accumulator
		this->Y = operand;
		//takes 2 cycles
		this->idleCycles = 2;
		//add 2 to program counter to pass all arguments for this instruction
		this->PC += 2;
		SetZN(this->Y);
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		//little endian so high memory address is in second byte.
		result = (operand2 << 8) | operand;
		//this->Y = this->memory[result];
		readFromMemory(result, &this->Y);
		//takes 3 cycles
		this->idleCycles = 3;
		//add 3 to program counter
		this->PC += 3;
		//SetZN(this->Y);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		//this->Y = this->memory[result];
		readFromMemory(result, &this->Y);
		//takes 4 cycles, plus one if page crossed.
		this->idleCycles = 4;
		if (isPageCrossed(this->memory[result - this->X], this->memory[result + this->X]))
			this->idleCycles += 1;
		this->PC += 3;
		//SetZN(this->Y);
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->Y;
		//this->Y = this->memory[result];
		readFromMemory(result, &this->Y);
		//4 cycles, plus one if page crossed
		this->idleCycles = 4;
		if (isPageCrossed(this->memory[result - this->Y], this->memory[this->Y]))
			this->idleCycles += 1;
		this->PC += 3;
		//SetZN(this->Y);
		break;
	case 8:
		//zero page. Wraparound if > FF.
		if (operand > 0xFF)
			operand -= 0xFF;
		//this->Y = this->memory[operand];
		readFromMemory(operand, &this->Y);
		this->idleCycles = 3;
		this->PC += 2;
		//SetZN(this->Y);
		break;
	case 9:
		//zero page, X
		result = operand + this->X;
		if (result > 0xFF)
			result -= (0xFF + 1);
		//this->Y = this->memory[result];
		readFromMemory(result, &this->Y);
		this->idleCycles = 4;
		this->PC += 2;
		//SetZN(this->Y);
		break;
	case 12:
		//(indirect, X)
		operand += this->X;
		if (operand > 0xFF)
			operand = operand - (0xFF + 1);
		result = operand + 1;
		if (result > 0xFF)
			result = result - 0xFF - 1;
		result = this->memory[operand] | (this->memory[result] << 8);
		//this->Y = this->memory[result];
		readFromMemory(result, &this->Y);
		this->idleCycles = 6;
		this->PC += 2;
		//SetZN(this->Y);
		break;
	case 13:
		//(indirect), Y
		operand2 = operand + 1;
		result = this->memory[operand] | (this->memory[operand2] << 8);
		//this->Y = this->memory[result] + this->Y;
		readFromMemory(result + this->Y, &this->Y);
		//extra cycle if page cross
		this->idleCycles = 5;
		if (isPageCrossed(operand, operand2))
			this->idleCycles += 1;
		this->PC += 2;
		//SetZN(this->Y);
		break;
	default:
		break;
	}
}
void CPU::STA(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		//little endian so high memory address is in second byte.
		result = (operand2 << 8) | operand;
		this->memory[result] = this->A;
		//takes 4 cycles
		this->idleCycles = 4;
		//add 3 to program counter
		this->PC += 3;
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		//this->memory[result] = this->A;
		writeToMemory(this->A, result);
		//takes 4 cycles, plus one if page crossed.
		this->idleCycles = 5;
		this->PC += 3;
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->Y;
		//this->memory[result] = this->A;
		writeToMemory(this->A, result);
		//4 cycles, plus one if page crossed
		this->idleCycles = 5;
		this->PC += 3;
		break;
	case 8:
		//zero page. Wraparound if > FF.
		if (operand > 0xFF)
			operand -= 0xFF;
		//this->memory[operand] = this->A;
		writeToMemory(this->A, operand);
		this->idleCycles = 3;
		this->PC += 2;
		break;
	case 9:
		//zero page, X
		result = operand + this->X;
		if (result > 0xFF)
			result -= (0xFF + 1);
		//this->memory[result] = this->A;
		writeToMemory(this->A, result);
		this->idleCycles = 4;
		this->PC += 2;
		break;
	case 12:
		//(indirect, X)
		operand += this->X;
		if (operand > 0xFF)
			operand -= (0xFF + 1);
		result = operand + 1;
		if (result > 0xFF)
			result = result - 0xFF - 1;
		result = this->memory[operand] | (this->memory[result] << 8);
		//this->memory[result] = this->A;
		writeToMemory(this->A, result);
		this->idleCycles = 6;
		this->PC += 2;
		break;
	case 13:
		//(indirect), Y
		operand2 = operand + 1;
		result = this->memory[operand] | (this->memory[operand2] << 8);
		result = result + this->Y;
		//this->memory[result] = this->A;
		writeToMemory(this->A, result);
		//extra cycle if page cross
		this->idleCycles = 5;
		this->PC += 2;
		break;
	default:
		break;
	}
}
void CPU::STX(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		//little endian so high memory address is in second byte.
		result = (operand2 << 8) | operand;
		//this->memory[result] = this->X;
		writeToMemory(this->X, result);
		//takes 4 cycles
		this->idleCycles = 4;
		//add 3 to program counter
		this->PC += 3;
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		//this->memory[result] = this->X;
		writeToMemory(this->X, result);
		//takes 4 cycles, plus one if page crossed.
		this->idleCycles = 5;
		this->PC += 3;
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->Y;
		//this->memory[result + this->Y] = this->X;
		writeToMemory(this->X, result + this->Y);
		//4 cycles, plus one if page crossed
		this->idleCycles = 5;
		this->PC += 3;
		break;
	case 8:
		//zero page. Wraparound if > FF.
		if (operand > 0xFF)
			operand -= 0xFF;
		//this->memory[operand] = this->X;
		writeToMemory(this->X, operand);
		this->idleCycles = 3;
		this->PC += 2;
		break;
	case 9:
		//zero page, X
		result = operand + this->X;
		if (result > 0xFF)
			result -= (0xFF + 1);
		//this->memory[result] = this->X;
		writeToMemory(this->X, result);
		this->idleCycles = 4;
		this->PC += 2;
		break;
	case 12:
		//(indirect, X)
		operand += this->X;
		if (operand > 0xFF)
			operand = operand - (0xFF + 1);
		result = operand + 1;
		if (result > 0xFF)
			result = result - 0xFF - 1;
		result = this->memory[operand] | (this->memory[result] << 8);
		//this->memory[result + this->X] = this->X;
		writeToMemory(this->X, result);
		this->idleCycles = 6;
		this->PC += 2;
		break;
	case 13:
		//(indirect), Y
		operand2 = operand + 1;
		result = this->memory[operand] | (this->memory[operand2] << 8);
		result = this->memory[result] + this->Y;
		//this->memory[result] = this->X;
		writeToMemory(this->X, result);
		//extra cycle if page cross
		this->idleCycles = 5;
		if (isPageCrossed(result, result - this->Y))
			this->idleCycles++;
		this->PC += 2;
		break;
	default:
		break;
	}
}
void CPU::STY(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		//little endian so high memory address is in second byte.
		result = (operand2 << 8) | operand;
		this->memory[result] = this->Y;
		//takes 4 cycles
		this->idleCycles = 4;
		//add 3 to program counter
		this->PC += 3;
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		//this->memory[result] = this->Y;
		writeToMemory(this->Y, result);
		this->idleCycles = 5;
		this->PC += 3;
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->Y;
		//this->memory[result + this->Y] = this->Y;
		writeToMemory(this->Y, result);
		//4 cycles, plus one if page crossed
		this->idleCycles = 5;
		this->PC += 3;
		break;
	case 8:
		//zero page. Wraparound if > FF.
		if (operand > 0xFF)
			operand -= 0xFF;
		//this->memory[operand] = this->Y;
		writeToMemory(this->Y, operand);
		this->idleCycles = 3;
		this->PC += 2;
		break;
	case 9:
		//zero page, X
		result = operand + this->X;
		if (result > 0xFF)
			result -= (0xFF + 1);
		//this->memory[result] = this->Y;
		writeToMemory(this->Y, result);
		this->idleCycles = 4;
		this->PC += 2;
		break;
	case 12:
		//(indirect, X)
		operand += this->X;
		if (operand > 0xFF)
			operand = operand - (0xFF + 1);
		result = operand + 1;
		if (result > 0xFF)
			result = result - 0xFF - 1;
		result = this->memory[operand] | (this->memory[result] << 8);
		//this->memory[result + this->X] = this->Y;
		writeToMemory(this->Y, result);
		this->idleCycles = 6;
		this->PC += 2;
		break;
	case 13:
		//(indirect), Y
		operand2 = operand + 1;
		result = this->memory[operand] | (this->memory[operand2] << 8);
		result = this->memory[result] + this->Y;
		//this->memory[result] = this->Y;
		writeToMemory(this->Y, result);
		//extra cycle if page cross
		this->idleCycles = 5;
		if (isPageCrossed(operand, operand2))
			this->idleCycles++;
		this->PC += 2;
		break;
	default:
		break;
	}
}

//Register Transfers
void CPU::TAX(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	this->X = this->A;
	SetZN(this->X);
	this->idleCycles = 2;
	this->PC += 1;
}
void CPU::TAY(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	this->Y = this->A;
	SetZN(this->Y);
	this->idleCycles = 2;
	this->PC += 1;
}
void CPU::TXA(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	this->A = this->X;
	SetZN(this->A);
	this->idleCycles = 2;
	this->PC += 1;
}
void CPU::TYA(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	this->A = this->Y;
	SetZN(this->A);
	this->idleCycles = 2;
	this->PC += 1;
}

//Stack Operations
void CPU::TSX(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	this->X = this->SP;
	SetZN(this->X);
	this->idleCycles = 2;
	this->PC += 1;
}
void CPU::TXS(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	this->SP = this->X;
	this->idleCycles = 2;
	this->PC += 1;
}
void CPU::PHA(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	this->stack[this->SP] = this->A;
	this->idleCycles = 3;
	this->PC += 1;
	this->SP -= 1;
}
void CPU::PHP(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	BYTE P;
	//bit 5 is always set and bit 4 is always set to 1 by php
	//will differ from nestest as the nestest never sets bit 4 (bflag1)
	P = (carryFlag << 0) + (zeroFlag << 1) + (interruptDisable << 2) + (decimalModeFlag << 3) + (1 << 4) + (1 << 5) + (overflowFlag << 6) + (negativeFlag << 7);
	this->stack[this->SP] = P;
	this->idleCycles = 3;
	this->PC += 1;
	this->SP -= 1;
}
void CPU::PLA(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	this->SP += 1;
	this->A = this->stack[this->SP];
	this->idleCycles = 4;
	this->PC += 1;
	SetZN(this->A);
}
void CPU::PLP(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	this->SP += 1;
	BYTE P;
	P = this->stack[this->SP];
	carryFlag = (P & 0x01 >> 0);
	zeroFlag = (P & 0x02) >> 1;
	interruptDisable = (P & 0x04) >> 2;
	decimalModeFlag = (P & 0x08) >> 3;
	//plp ignores bits 4 and 5
	//bFlag1 = (P & 0x10) >> 4;
	//bFlag2 = (P & 0x20) >> 5;
	overflowFlag = (P & 0x40) >> 6;
	negativeFlag = (P & 0x80) >> 7;
	this->idleCycles = 4;
	this->PC += 1;
}

//Logical
void CPU::AND(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 2:
		//immediate
		this->A = this->A & operand;
		this->idleCycles = 2;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		this->A = this->memory[result] & this->A;
		//readFromMemory(this->memory[result] & this->A, &this->A);
		this->idleCycles = 4;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		this->A = this->memory[result] & this->A;
		this->idleCycles = 4;
		//for page crossed do I need the below or do I need this->memory[result], this->memory[result + this->X]. This would need to be replaced at every absolute switch
		if (isPageCrossed(result, result - this->X))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->Y;
		this->A = (this->memory[result]) & this->A;
		this->idleCycles = 4;
		if (isPageCrossed(result, result - this->Y))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 8:
		//zero page. Wraparound if > FF.
		if (operand > 0xFF)
			operand -= 0xFF;
		this->A = this->memory[operand] & this->A;
		this->idleCycles = 3;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 9:
		//zero page, X
		operand += this->X;
		if ((operand) > 0xFF)
			operand -= (0xFF + 1);
		this->A = this->memory[operand] & this->A;
		this->idleCycles = 4;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 12:
		//(indirect, X)
		operand += this->X;
		if (operand > 0xFF)
			operand = operand - (0xFF + 1);
		result = operand + 1;
		if (result > 0xFF)
			result = result - 0xFF - 1;
		result = this->memory[operand] | (this->memory[result] << 8);
		this->A = this->memory[result] & this->A;
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 13:
		//(indirect), Y
		operand2 = operand + 1;
		result = this->memory[operand] | (this->memory[operand2] << 8);
		result += this->Y;
		this->A = (this->memory[result]) & this->A;
		this->idleCycles = 5;
		if (isPageCrossed(this->memory[result], this->memory[result] + this->Y))
			this->idleCycles += 1;
		this->PC += 2;
		SetZN(this->A);
		break;
	default:
		break;
	}
}
void CPU::EOR(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 2:
		//immediate
		this->A = this->A ^ operand;
		this->idleCycles = 2;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		this->A = this->A ^ this->memory[result];
		this->idleCycles = 4;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		this->A = this->A ^ (this->memory[result]);
		this->idleCycles = 4;
		if (isPageCrossed(result, result - this->X))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->Y;
		this->A = this->A ^ (this->memory[result]);
		this->idleCycles = 4;
		if (isPageCrossed(result, result - this->Y))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 8:
		//zero page. Wraparound if > FF.
		if (operand > 0xFF)
			operand -= 0xFF;
		this->A = this->memory[operand] ^ this->A;
		this->idleCycles = 3;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 9:
		//zero page, X
		operand += this->X;
		if ((operand)> 0xFF)
			operand -= (0xFF + 1);
		this->A = this->memory[operand] ^ this->A;
		this->idleCycles = 4;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 12:
		//(indirect, X)
		operand += this->X;
		if (operand > 0xFF)
			operand = operand - (0xFF + 1);
		result = operand + 1;
		if (result > 0xFF)
			result = result - 0xFF - 1;
		result = this->memory[operand] | (this->memory[result] << 8);
		this->A = this->memory[result] ^ this->A;
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 13:
		//(indirect), Y
		operand2 = operand + 1;
		result = this->memory[operand] | (this->memory[operand2] << 8);
		result += this->Y;
		this->A = (this->memory[result]) ^ this->A;
		this->idleCycles = 5;
		if (isPageCrossed((result- this->Y), (result)))
			this->idleCycles += 1;
		this->PC += 2;
		SetZN(this->A);
		break;
	default:
		break;
	}
}
void CPU::ORA(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 2:
		//immediate
		this->A = this->A | operand;
		this->idleCycles = 2;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		this->A = this->A | this->memory[result];
		this->idleCycles = 4;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		this->A = this->A | (this->memory[result]);
		this->idleCycles = 4;
		if (isPageCrossed(result, result - this->X))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->Y;
		this->A = this->A | (this->memory[result]);
		this->idleCycles = 4;
		if (isPageCrossed(result, result - this->Y))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 8:
		//zero page. Wraparound if > FF.
		if (operand > 0xFF)
			operand -= 0xFF;
		this->A = this->memory[operand] | this->A;
		this->idleCycles = 3;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 9:
		//zero page, X
		operand = operand + this->X;
		if ((operand) > 0xFF)
			operand = operand - (0xFF + 1);
		this->A = this->memory[operand] | this->A;
		this->idleCycles = 4;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 12:
		//(indirect, X)
		operand += this->X;
		if (operand > 0xFF)
			operand = operand - (0xFF + 1);
		result = operand + 1;
		if (result > 0xFF)
			result = result - 0xFF - 1;
		result = this->memory[operand] | (this->memory[result] << 8);
		this->A = this->memory[result] | this->A;
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 13:
		//(indirect), Y
		operand2 = operand + 1;
		result = this->memory[operand] | (this->memory[operand2] << 8);
		result += this->Y;
		this->A = (this->memory[result]) | this->A;
		this->idleCycles = 5;
		if (isPageCrossed((result - this->Y), (result)))
			this->idleCycles += 1;
		this->PC += 2;
		SetZN(this->A);
		break;
	default:
		break;
	}
}
void CPU::BIT(Instructions::Instruction I) {
	//and mask but dont store result
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		if ((this->A & this->memory[result]) == 0x00)
			zeroFlag = 1;
		else zeroFlag = 0;
		if ((this->memory[result] & 0x40) == 0x00)
			overflowFlag = 0;
		else overflowFlag = 1;
		if ((this->memory[result] & 0x80) == 0x00)
			negativeFlag = 0;
		else negativeFlag = 1;
		this->idleCycles = 4;
		this->PC += 3;
		break;
	case 8:
		//zero page.
		operand2 = this->memory[operand];
		if ((this->A & operand2) == 0x00)
			zeroFlag = 1;
		else zeroFlag = 0;
		/*
		if ((operand & 0x70) == 0x00)
			overflowFlag = 0;
		else
			overflowFlag = 1;
		if ((operand & 0x80) == 0x00)
			negativeFlag = 0;
		else
			negativeFlag = 1;
			*/
		this->overflowFlag = (operand2 & 0x40);
		this->negativeFlag = (operand2 & 0x80);
		this->idleCycles = 3;
		this->PC += 2;
		break;
	default:
		break;
	}
}

//artithmetic
//I think ADC and SBC work correctly but I may be mistaken. Will check with nestest
void CPU::ADC(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	bool tmp;
	switch (I.mode) {
	case 2:
		//immediate
		//overflow set if value is greater than signed 8 bit value, carry set if > unsigned
		//processor does a xor on bits 6 and 7 of the result
		/*
		if ((this->A + operand) > 127)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
			*/
		//if ((!(a^b)) & (a ^ c) & 0x80) where a + b = c. if this = 0x00 overflow flag is off, otherwise set.
		tmp = this->carryFlag;
		//if (!((this->A ^ operand) & 0x80) && (this->A ^ (this->A + operand + tmp) & 0x80))
		if ((this->A ^ (this->A + operand + tmp)) & (operand ^ (this->A + operand + tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		this->carryFlag = 0;
		if ((this->A + operand + tmp) > 0xFF)
			this->carryFlag = 1;
		this->A = this->A + operand + tmp;
		this->idleCycles = 2;
		this->PC += 2;
		SetZN(this->A);
		//this->carryFlag = 0;
		//if (this->overflowFlag == 1)
			//this->carryFlag = 1;
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		tmp = this->carryFlag;
		if ((this->A ^ (this->A + this->memory[result] + tmp)) & (this->memory[result] ^ (this->A + this->memory[result] + tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		this->carryFlag = 0;
		if ((this->A + this->memory[result] + tmp) > 0xFF)
			this->carryFlag = 1;
		this->A = this->A + this->memory[result] + tmp;
		this->idleCycles = 4;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		tmp = this->carryFlag;
		if ((this->A ^ (this->A + this->memory[result] + tmp)) & (this->memory[result] ^ (this->A + this->memory[result] + tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		this->carryFlag = 0;
		if ((this->A + this->memory[result] + tmp) > 0xFF)
			this->carryFlag = 1;
		this->A = this->A + this->memory[result] + tmp;
		this->idleCycles = 4;
		if (isPageCrossed(result, result - this->X))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->Y;
		tmp = this->carryFlag;
		if ((this->A ^ (this->A + this->memory[result] + tmp)) & (this->memory[result] ^ (this->A + this->memory[result] + tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		this->carryFlag = 0;
		if ((this->A + this->memory[result] + tmp) > 0xFF)
			this->carryFlag = 1;
		this->A = this->A + this->memory[result] + tmp;
		this->idleCycles = 4;
		if (isPageCrossed(result, result - this->Y))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 8:
		//zero page. wraparound if > 0xFF
		if (operand > 0xFF)
			operand -= (0xFF + 1);
		tmp = this->carryFlag;
		if ((this->A ^ (this->A + this->memory[operand] + tmp)) & (this->memory[operand] ^ (this->A + this->memory[operand] + tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		this->carryFlag = 0;
		if ((this->A + this->memory[operand] + tmp) > 0xFF)
			this->carryFlag = 1;
		this->A = this->A + this->memory[operand] + tmp;
		this->idleCycles = 3;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 9:
		//zero page, X
		operand += this->X;
		if (operand > 0xFF)
			operand = operand - (0xFF + 1);
		tmp = this->carryFlag;
		if ((this->A ^ (this->A + this->memory[operand] + tmp)) & (this->memory[operand] ^ (this->A + this->memory[operand] + tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		this->carryFlag = 0;
		if ((this->A + this->memory[operand] + tmp) > 0xFF)
			this->carryFlag = 1;
		this->A = this->A + this->memory[operand] + tmp;
		this->idleCycles = 4;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 12:
		//indirect, X
		tmp = this->carryFlag;
		operand2 = operand + this->X;
		if (operand2 > 0xFF)
			operand2 = operand2 - (0xFF + 1);
		result = operand2 + 1;
		if (result > 0xFF)
			result = result - 0xFF - 1;
		result = this->memory[operand] | (this->memory[result] << 8);
		if ((this->A ^ (this->A + this->memory[result] + tmp)) & (this->memory[result] ^ (this->A + this->memory[result] + tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		this->carryFlag = 0;
		if ((this->A + this->memory[result] + tmp) > 0xFF)
			this->carryFlag = 1;
		this->A = this->memory[result] + this->A + tmp;
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 13:
		//indirect, Y
		tmp = this->carryFlag;
		operand2 = operand + 1;
		result = this->memory[operand] | (this->memory[operand2] << 8);
		result = result + this->Y;
		if ((this->A ^ (this->A + this->memory[result] + tmp)) & (this->memory[result] ^ (this->A + this->memory[result]+ tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		this->carryFlag = 0;
		if ((this->A + this->memory[result]+ tmp) > 0xFF)
			this->carryFlag = 1;
		this->A = (this->memory[result]) + this->A + tmp;
		this->idleCycles = 5;
		if (isPageCrossed((this->memory[result]), (this->memory[result] - this->Y)))
			this->idleCycles += 1;
		this->PC += 2;
		SetZN(this->A);
		break;
	default:
		break;
	}
}
void CPU::SBC(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	bool tmp;
	switch (I.mode) {
	case 2:
		//immediate
		tmp = this->carryFlag;
		if ((this->A ^ (this->A + (255 - operand) + tmp)) & ((255 - operand) ^ (this->A + (255 - operand) + tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		//if ((this->A + (255 - operand) + tmp) > 0xFF)
			//this->carryFlag = 1;
		//if the number that is being subtracted from is bigger than the subtracted number then there is no borrow required.
		if (this->A >= operand)
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->A = this->A + (255 - operand) + tmp;
		SetZN(this->A);
		this->idleCycles = 2;
		this->PC += 2;
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		tmp = this->carryFlag;
		if ((this->A ^ (this->A + (255 - this->memory[result]) + tmp)) & ((255 - this->memory[result]) ^ (this->A + (255 - this->memory[result]) + tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		if (this->A >= this->memory[result])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->A = this->A + (255 - this->memory[result]) + tmp;
		SetZN(this->A);
		if (this->overflowFlag == 1)
			this->carryFlag = 1;
		this->idleCycles = 4;
		this->PC += 3;
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		tmp = this->carryFlag;
		if ((this->A ^ (this->A + (255 - this->memory[result]) + tmp)) & ((255 - this->memory[result]) ^ (this->A + (255 - this->memory[result]) + tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		if (this->A >= this->memory[result])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->A = this->A + (255- this->memory[result]) + tmp;
		SetZN(this->A);
		this->idleCycles = 4;
		if (isPageCrossed(result, result - this->X))
			this->idleCycles += 1;
		this->PC += 3;
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->Y;
		tmp = this->carryFlag;
		if ((this->A ^ (this->A + (255 - this->memory[result]) + tmp)) & ((255 - this->memory[result]) ^ (this->A + (255 - this->memory[result]) + tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		if (this->A >= this->memory[result])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->A = this->A + (255 - this->memory[result]) + tmp;
		SetZN(this->A);
		this->idleCycles = 4;
		if (isPageCrossed(result, result - this->Y))
			this->idleCycles += 1;
		this->PC += 3;
		break;
	case 8:
		//zero page
		if (operand > 0xFF)
			operand = operand - 0xFF;
		tmp = this->carryFlag;
		if ((this->A ^ (this->A + (255 - this->memory[operand]) + tmp)) & ((255 - this->memory[operand]) ^ (this->A + (255 - this->memory[operand]) + tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		if (this->A >= this->memory[operand])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->A = this->A + (255 - this->memory[operand]) + tmp;
		SetZN(this->A);
		this->idleCycles = 3;
		this->PC += 2;
		break;
	case 9:
		//zero page, X
		operand += this->X;
		if ((operand) > 0xFF)
			operand = operand - (0xFF + 1);
		tmp = this->carryFlag;
		if ((this->A ^ (this->A + (255 - this->memory[operand]) + tmp)) & ((255 - this->memory[operand]) ^ (this->A + (255 - this->memory[operand]) + tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		if (this->A >= this->memory[operand])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->A = this->A + (255 - this->memory[operand]) + tmp;
		SetZN(this->A);
		this->idleCycles = 3;
		this->PC += 2;
		break;
	case 12:
		//indirect, X
		tmp = this->carryFlag;
		operand = operand + this->X;
		if (operand > 0xFF)
			operand -= (0xFF + 1);
		result = operand + 1;
		if (result > 0xFF)
			result -= 0xFF - 1;
		result = this->memory[operand] | (this->memory[result] << 8);
		if ((this->A ^ (this->A + (255 - this->memory[result]) + tmp)) & ((255 - this->memory[result]) ^ (this->A + (255 - this->memory[result]) + tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		if (this->A >= this->memory[result])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->A = this->A + (255 - this->memory[result]) + tmp;
		SetZN(this->A);
		this->idleCycles = 3;
		this->PC += 2;
		break;
	case 13:
		//indirect, Y
		tmp = this->carryFlag;
		operand2 = operand + 1;
		result = this->memory[operand] | (this->memory[operand2] << 8);
		result = result + this->Y;
		if ((this->A ^ (this->A + (255 - this->memory[result]) + tmp)) & ((255 - this->memory[result]) ^ (this->A + (255 - this->memory[result]) + tmp)) & 0x80)
			this->overflowFlag = 1;
		else
			this->overflowFlag = 0;
		if (this->A >= (this->memory[result]))
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->A = this->A  + (255 - (this->memory[result])) + tmp;
		SetZN(this->A);
		this->idleCycles = 3;
		if (isPageCrossed(result, result - this->Y))
			this->idleCycles++;
		this->PC += 2;
		break;
	default:
		break;
	}
}
void CPU::CMP(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 2:
		//immediate
		SetZN(this->A - operand);
		if (this->A >= operand)
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->idleCycles = 2;
		this->PC += 2;
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		SetZN(this->A - this->memory[result]);
		if (this->A >= this->memory[result])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->idleCycles = 4;
		this->PC += 3;
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		SetZN(this->A - this->memory[result]);
		if (this->A >= this->memory[result])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->idleCycles = 4;
		if (isPageCrossed(result, result + this->X))
			this->idleCycles += 1;
		this->PC += 3;
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->Y;
		SetZN(this->A - this->memory[result]);
		if (this->A >= this->memory[result])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->idleCycles = 4;
		if (isPageCrossed(result, result - this->Y))
			this->idleCycles += 1;
		this->PC += 3;
		break;
	case 8:
		//zero page
		SetZN(this->A - this->memory[operand]);
		if (this->A >= this->memory[operand])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->idleCycles = 3;
		this->PC += 2;
		break;
	case 9:
		//zero page, X
		operand += this->X;
		if (operand > 0xFF)
			operand -= (0xFF + 1);
		SetZN(this->A - this->memory[operand + this->X]);
		if (this->A >= this->memory[operand + this->X])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->idleCycles = 4;
		this->PC += 2;
		break;
	case 12:
		//indirect, X
		operand2 = operand + this->X;
		if (operand2 > 0xFF)
			operand2 -= (0xFF + 1);
		result = operand2 + 1;
		if (result > 0xFF)
			result -= 0xFF - 1;
		result = this->memory[operand2] | (this->memory[result] << 8);
		SetZN(this->A - this->memory[result]);
		if (this->A >= this->memory[result])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->idleCycles = 6;
		this->PC += 2;
		break;
	case 13:
		//indirect, Y
		operand2 = operand + 1;
		result = this->memory[operand] | (this->memory[operand2] << 8);
		result = result + this->Y;
		SetZN(this->A - this->memory[result]);
		if (this->A >= this->memory[result])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->idleCycles = 5;
		if (isPageCrossed(result, result - this->Y))
			this->idleCycles += 1;
		this->PC += 2;
		break;
	default:
		break;
	}
}
void CPU::CPX(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 2:
		//immediate
		SetZN(this->X - operand);
		if (this->X >= operand)
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->idleCycles = 2;
		this->PC += 2;
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		SetZN(this->X - this->memory[result]);
		if (this->X >= this->memory[result])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->idleCycles = 3;
		this->PC += 3;
		break;
	case 8:
		//zero page
		SetZN(this->X - this->memory[operand]);
		if (this->X >= this->memory[operand])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->idleCycles = 4;
		this->PC += 2;
		break;
	default:
		break;
	}
}
void CPU::CPY(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 2:
		//immediate
		SetZN(this->Y - operand);
		if (this->Y >= operand)
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->idleCycles = 2;
		this->PC += 2;
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		SetZN(this->Y - this->memory[result]);
		if (this->Y >= this->memory[result])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->idleCycles = 3;
		this->PC += 3;
		break;
	case 8:
		//zero page
		SetZN(this->Y - this->memory[operand]);
		if (this->Y >= this->memory[operand])
			this->carryFlag = 1;
		else
			this->carryFlag = 0;
		this->idleCycles = 4;
		this->PC += 2;
		break;
	default:
		break;
	}
}

//Increment/Decrement
void CPU::INC(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		this->memory[result] += 1;
		this->idleCycles = 6;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		this->memory[result + this->X] += 1;
		this->idleCycles = 7;
		this->PC += 3;
		SetZN(this->memory[result + this->X]);
		break;
	case 8:
		//zero page
		this->memory[operand] += 1;
		this->idleCycles = 5;
		this->PC += 2;
		SetZN(this->memory[operand]);
		break;
	case 9:
		//zero page, X
		operand += this->X;
		if (operand > 0xFF)
			operand -= (0xFF + 1);
		this->memory[operand] += 1;
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->memory[operand]);
		break;
	default:
		break;
	}
}
void CPU::INX(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	this->X += 1;
	this->idleCycles = 2;
	this->PC += 1;
	SetZN(this->X);
}
void CPU::INY(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	this->Y += 1;
	this->idleCycles = 2;
	this->PC += 1;
	SetZN(this->Y);
}
void CPU::DEC(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		this->memory[result] -= 1;
		this->idleCycles = 6;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		this->memory[result + this->X] -= 1;
		this->idleCycles = 7;
		this->PC += 3;
		SetZN(this->memory[result + this->X]);
		break;
	case 8:
		//zero page
		this->memory[operand] -= 1;
		this->idleCycles = 5;
		this->PC += 2;
		SetZN(this->memory[operand]);
		break;
	case 9:
		//zero page, X
		operand += this->X;
		if (operand > 0xFF)
			operand -= (0xFF + 1);
		this->memory[operand] -= 1;
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->memory[operand]);
		break;
	default:
		break;
	}
}
void CPU::DEX(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	this->X -= 1;
	this->idleCycles = 2;
	this->PC += 1;
	SetZN(this->X);
}
void CPU::DEY(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	this->Y -= 1;
	this->idleCycles = 2;
	this->PC += 1;
	SetZN(this->Y);
}

//Shifts
void CPU::ASL(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 1:
		//accumulator
		carryFlag = this->A & 0x80;
		this->A = (this->A << 1);
		SetZN(this->A);
		this->idleCycles = 2;
		this->PC += 1;
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		carryFlag = this->memory[result] & 0x80;
		this->memory[result] = this->memory[result] << 1;
		this->idleCycles = 6;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		carryFlag = this->memory[result] & 0x80;
		this->memory[result] = this->memory[result] << 1;
		this->idleCycles = 7;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 8:
		//zero page
		carryFlag = this->memory[operand] & 0x80;
		this->memory[operand] = this->memory[operand] << 1;
		this->idleCycles = 5;
		this->PC += 2;
		SetZN(this->memory[operand]);
		break;
	case 9:
		//zero page, X
		operand += this->X;
		if (operand > 0xFF)
			operand -= (0xFF + 1);
		carryFlag = this->memory[operand] & 0x80;
		this->memory[operand] = this->memory[operand] << 1;
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->memory[operand]);
		break;
	default:
		break;
	}
}
void CPU::LSR(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 1:
		//accumulator
		carryFlag = this->A & 0x01;
		this->A = (this->A >> 1);
		SetZN(this->A);
		this->idleCycles = 2;
		this->PC += 1;
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		carryFlag = this->memory[result] & 0x01;
		this->memory[result] = this->memory[result] >> 1;
		this->idleCycles = 6;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		carryFlag = this->memory[result] & 0x01;
		this->memory[result] = this->memory[result] >> 1;
		this->idleCycles = 7;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 8:
		//zero page
		carryFlag = this->memory[operand] & 0x01;
		this->memory[operand] = this->memory[operand] >> 1;
		this->idleCycles = 5;
		this->PC += 2;
		SetZN(this->memory[operand]);
		break;
	case 9:
		//zero page, X
		operand += this->X;
		if (operand > 0xFF)
			operand -= (0xFF + 1);
		carryFlag = this->memory[operand] & 0x01;
		this->memory[operand] = this->memory[operand] >> 1;
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->memory[operand]);
		break;
	default:
		break;
	}
}
void CPU::ROL(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	//I think this should work
	switch (I.mode) {
	case 1:
		//accumulator
		result = carryFlag;
		carryFlag = this->A & 0x80;
		this->A = this->A << 1;
		this->A = this->A | result;
		SetZN(this->A);
		this->idleCycles = 2;
		this->PC += 1;
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		operand = carryFlag;
		carryFlag = this->memory[result] & 0x80;
		this->memory[result] = this->memory[result] << 1;
		this->memory[result] = this->memory[result] | operand;
		this->idleCycles = 6;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		operand = carryFlag;
		carryFlag = this->memory[result] & 0x80;
		this->memory[result] = this->memory[result] << 1;
		this->memory[result] = this->memory[result] | operand;
		this->idleCycles = 7;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 8:
		//zero page
		result = carryFlag;
		carryFlag = this->memory[operand] & 0x80;
		this->memory[operand] = this->memory[operand] << 1;
		this->memory[operand] = this->memory[operand] | result;
		this->idleCycles = 5;
		this->PC += 2;
		SetZN(this->memory[operand]);
		break;
	case 9:
		//zero page, X
		operand += this->X;
		if (operand > 0xFF)
			operand -= (0xFF + 1);
		result = carryFlag;
		carryFlag = this->memory[operand] & 0x80;
		this->memory[operand] = this->memory[operand] << 1;
		this->memory[operand] = this->memory[operand] | result;
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->memory[operand]);
		break;
	default:
		break;
	}
}
void CPU::ROR(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	//I think this should work? Have to set highest bit of address to old carry flag
	switch (I.mode) {
	case 1:
		//accumulator
		result = carryFlag;
		carryFlag = this->A & 0x01;
		this->A = (this->A >> 1);
		this->A = this->A | (result << 7);
		SetZN(this->A);
		this->idleCycles = 2;
		this->PC += 1;
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		operand = carryFlag;
		carryFlag = this->memory[result] & 0x01;
		this->memory[result] = this->memory[result] >> 1;
		this->memory[result] = this->memory[result] | (operand << 7);
		this->idleCycles = 6;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		result += this->X;
		operand = carryFlag;
		carryFlag = this->memory[result] & 0x01;
		this->memory[result] = this->memory[result] >> 1;
		this->memory[result] = this->memory[result] | (operand << 7);
		this->idleCycles = 7;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 8:
		//zero page
		result = carryFlag;
		carryFlag = this->memory[operand] & 0x01;
		this->memory[operand] = this->memory[operand] >> 1;
		this->memory[operand] = this->memory[operand] | (result << 7);
		this->idleCycles = 5;
		this->PC += 2;
		SetZN(this->memory[operand]);
		break;
	case 9:
		//zero page, X
		operand += this->X;
		if (operand > 0xFF)
			operand -= (0xFF + 1);
		result = carryFlag;
		carryFlag = this->memory[operand] & 0x01;
		this->memory[operand] = this->memory[operand] >> 1;
		this->memory[operand] = this->memory[operand] | (result << 7);
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->memory[operand]);
		break;
	default:
		break;
	}
}

//Jumps
void CPU::JMP(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		//this->stack[this->SP] = this->PC;
		//this->SP--;
		this->PC = result;
		this->idleCycles = 3;
		//this->PC += 3;
		break;
	case 11:
		//indirect
		//An original 6502 has does not correctly fetch the target address if the indirect vector falls on a page boundary (e.g. $xxFF where xx is any value from $00 to $FF). 
		//In this case fetches the LSB from $xxFF as expected but takes the MSB from $xx00. 
		//This is fixed in some later chips like the 65SC02 so for compatibility always ensure the indirect vector is not at the end of the page.
		//the error is not implemented yet.
		/* dont think I need to save jmp to stack
		this->stack[this->SP] = (this->PC & 0x00FF);
		this->SP--;
		this->stack[this->SP] = ((this->PC & 0xFF00) >> 8);
		this->SP--;
		*/
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) | operand;
		if (operand == 0xFF) {
			operand2 = this->memory[result & 0xFF00];
			operand = this->memory[result];
		}
		else {
			operand = this->memory[result];
			operand2 = this->memory[result + 1];
		}
		result = (operand2 << 8) | operand;
		this->PC = result;
		this->idleCycles = 5;
		break;
	default:
		break;
	}
}
void CPU::JSR(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	if (I.mode != 5)
		return;
	//this->PC or this->PC - 1? I think the minus one is due to incrementing the program counter at decoding, which I don't do, so I shouldn't do the - 1 here.
	operand2 = this->memory[this->PC + 2];
	//this->PC--;
	this->PC += 2;
	this->stack[this->SP] = ((this->PC & 0xFF00) >> 8);
	this->SP--;
	this->stack[this->SP] = (this->PC & 0x00FF);
	this->SP--;
	result = (operand2 << 8) | operand;
	//this->PC = this->memory[result];
	this->PC = result;
	this->idleCycles = 6;
	//this->PC += 3;
}
void CPU::RTS(Instructions::Instruction I) {
	BYTE operand;
	BYTE operand2;
	unsigned short result;
	if (I.mode != 3)
		return;
	this->SP++;
	operand = this->stack[this->SP];
	this->SP++;
	operand2 = this->stack[this->SP];
	result = (operand2 << 8) | operand;
	this->PC = result;
	this->idleCycles = 6;
	this->PC++;
	//this->PC += 1;
	//this->PC += 2;
}

//Branches
void CPU::BCC(Instructions::Instruction I) {
	if (I.mode != 4)
		return;
	BYTE operand = this->memory[this->PC + 1];
	this->idleCycles = 2;
	if (this->carryFlag != 0) {
		this->PC += 2;
		return;
	}
	this->idleCycles += 1;
	this->PC = this->PC + operand;
	if (isPageCrossed(this->PC, this->PC - operand))
		this->idleCycles += 1;
	this->PC += 2;
}
void CPU::BCS(Instructions::Instruction I) {
	if (I.mode != 4)
		return;
	BYTE operand = this->memory[this->PC + 1];
	this->idleCycles = 2;
	if (this->carryFlag != 1) {
		this->PC += 2;
		return;
	}
	this->idleCycles += 1;
	this->PC = this->PC + operand;
	if (isPageCrossed(this->PC, this->PC - operand))
		this->idleCycles += 1;
	this->PC += 2;
}
void CPU::BEQ(Instructions::Instruction I) {
	if (I.mode != 4)
		return;
	BYTE operand = this->memory[this->PC + 1];
	this->idleCycles = 2;
	if (this->zeroFlag != 1) {
		this->PC += 2;
		return;
	}
	this->idleCycles += 1;
	this->PC = this->PC + operand;
	if (isPageCrossed(this->PC, this->PC - operand))
		this->idleCycles += 1;
	this->PC += 2;
}
void CPU::BMI(Instructions::Instruction I) {
	if (I.mode != 4)
		return;
	BYTE operand = this->memory[this->PC + 1];
	this->idleCycles = 2;
	if (this->negativeFlag != 1) {
		this->PC += 2;
		return;
	}
	this->idleCycles += 1;
	this->PC = this->PC + operand;
	if (isPageCrossed(this->PC, this->PC - operand))
		this->idleCycles += 1;
	this->PC += 2;
}
void CPU::BNE(Instructions::Instruction I) {
	if (I.mode != 4)
		return;
	BYTE operand = this->memory[this->PC + 1];
	this->idleCycles = 2;
	if (this->zeroFlag != 0) {
		this->PC += 2;
		return;
	}
	this->idleCycles += 1;
	this->PC = this->PC + operand;
	if (isPageCrossed(this->PC, this->PC - operand))
		this->idleCycles += 1;
	this->PC += 2;
}
void CPU::BPL(Instructions::Instruction I) {
	if (I.mode != 4)
		return;
	BYTE operand = this->memory[this->PC + 1];
	this->idleCycles = 2;
	if (this->negativeFlag != 0) {
		this->PC += 2;
		return;
	}
	this->idleCycles += 1;
	this->PC = this->PC + operand;
	if (isPageCrossed(this->PC, this->PC - operand))
		this->idleCycles += 1;
	this->PC += 2;
}
void CPU::BVC(Instructions::Instruction I) {
	if (I.mode != 4)
		return;
	BYTE operand = this->memory[this->PC + 1];
	this->idleCycles = 2;
	if (this->overflowFlag != 0) {
		this->PC += 2;
		return;
	}
	this->idleCycles += 1;
	this->PC = this->PC + operand;
	if (isPageCrossed(this->PC, this->PC - operand))
		this->idleCycles += 1;
	this->PC += 2;
}
void CPU::BVS(Instructions::Instruction I) {
	if (I.mode != 4)
		return;
	BYTE operand = this->memory[this->PC + 1];
	this->idleCycles = 2;
	if (this->overflowFlag != 1) {
		this->PC += 2;
		return;
	}
	this->idleCycles += 1;
	this->PC = this->PC + operand;
	if (isPageCrossed(this->PC, this->PC - operand))
		this->idleCycles += 1;
	this->PC += 2;
}

//Status Flag Changes
void CPU::CLC(Instructions::Instruction I) {
	this->carryFlag = 0;
	this->idleCycles = 2;
	this->PC += 1;
}
void CPU::CLD(Instructions::Instruction I) {
	this->decimalModeFlag = 0;
	this->idleCycles = 2;
	this->PC += 1;
}
void CPU::CLI(Instructions::Instruction I) {
	this->interruptDisable = 0;
	this->idleCycles = 2;
	this->PC += 1;
}
void CPU::CLV(Instructions::Instruction I) {
	this->overflowFlag = 0;
	this->idleCycles = 2;
	this->PC += 1;
}
void CPU::SEC(Instructions::Instruction I) {
	this->carryFlag = 1;
	this->idleCycles = 2;
	this->PC += 1;
}
void CPU::SED(Instructions::Instruction I) {
	this->decimalModeFlag = 1;
	this->idleCycles = 2;
	this->PC += 1;
}
void CPU::SEI(Instructions::Instruction I) {
	this->interruptDisable = 1;
	this->idleCycles = 2;
	this->PC += 1;
}

//System
void CPU::BRK(Instructions::Instruction I) {
	//generate interupt. Push flags to stack
	if (I.mode != 3)
		return;
	//push program bank register?
	this->stack[this->SP] = (this->PC >> 8);
	this->SP--;
	this->stack[this->SP] = (this->PC & 0x00FF);
	this->SP--;
	BYTE P;
	P = (carryFlag << 0) + (zeroFlag << 1) + (interruptDisable << 2) + (decimalModeFlag << 3) + (bFlag1 << 4) + (bFlag2 << 5) + (overflowFlag << 6) + (negativeFlag << 7);
	this->interruptDisable = 1;
	this->stack[this->SP] = P;
	this->SP -= 1;
	this->bFlag1 = 1;
	this->bFlag2 = 1;
	//pull program bank register?
	//with RESET pull program counter from FFFC/FFFD
	//with NMI pull program counter from FFFA/FFFB
	//with IRQ pull program counter from FFFE/FFFF
	this->PC = this->memory[0xFFFE] | (this->memory[0xFFF] << 8);
	this->idleCycles = 7;
	this->PC += 1;
}
void CPU::NOP(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	this->idleCycles = 2;
	this->PC += 1;
}
void CPU::RTI(Instructions::Instruction I) {
	//get satus flags from stack
	this->SP += 1;
	BYTE P;
	P = this->stack[this->SP];
	this->negativeFlag = (P & 0x80);
	this->overflowFlag = (P & 0x40);
	this->bFlag2 = 1; //Interrupts will always push a 1 in this flag. I don't have interrupts yet so this fakes that for now. //(P & 0x20);
	this->bFlag1 = (P & 0x10);
	this->decimalModeFlag = (P & 0x08);
	this->interruptDisable = (P & 0x04);
	this->zeroFlag = (P & 0x02);
	this->carryFlag = (P & 0x01);
	this->idleCycles = 6;
	this->PC += 1;
	this->SP++;
	//would get PC from stack after handling interupt, which stores PC to stack, which I don't have yet.
	//this->PC = this->stack[this->SP] | (this->stack[this->SP + 1] << 8)
	this->SP++;
}

void CPU::Clock_Tick() {
	//check for interrupts

	//poll for interrupt somehow
	//IRQ is a CPU interrupt, NMI is a PPU interrupt. That is how I will differentiate them.

	//retrieve opcode
	//execute opcode
	
	//the 6502 is little endian (least sig bytes first)
	//stack pointer counts down from 0xFD to 0x00 with no overflow protection

	//wait for clock ticks to simulate NES speed

	if (this->idleCycles > 0) {
		this->idleCycles--;
		return;
	}

	this->opcode = this->memory[this->PC];

	//inst = I.getInstruction(opcode);
	//I hate everything about this right now. Storing information in inst from the Instruction class, or setting inst.name = temp causes a complete meltdown for some reason.
	std::string temp = _instructions[opcode];
	inst.length = _InstructionLength[opcode];
	inst.cycles = _InstructionCycles[opcode];
	inst.mode = _InstructionMode[opcode];
	inst.type = _InstructionType[opcode];

	BYTE P;
	P = (carryFlag << 0) + (zeroFlag << 1) + (interruptDisable << 2) + (decimalModeFlag << 3) + (bFlag1 << 4) + (bFlag2 << 5) + (overflowFlag << 6) + (negativeFlag << 7);


	std::cout << std::hex << this->PC << " " << std::hex << unsigned(this->opcode) << " " << std::hex << this->memory[this->PC] << " " << std::hex << this->memory[this->PC + 1] << " A:" << std::hex << unsigned(this->A) << " X:" << std::hex << unsigned(this->X) << " Y:" << std::hex << unsigned(this->Y) << "  P:" << std::hex << unsigned(P) << " SP:" << std::hex  <<this->SP << " C:" << inst.cycles;
	//if (inst.length > 2)
		//std::cout << std::hex << this->memory[this->PC + 2];

	std::cout << "\n";

	switch (inst.type) {
		//I hate this. Want to change later
	case 1:
		//load and store
		if (temp == "LDA")
			LDA(inst);
		if (temp == "LDX")
			LDX(inst);
		if (temp == "LDY")
			LDY(inst);
		if (temp == "STA")
			STA(inst);
		if (temp == "STX")
			STX(inst);
		if (temp == "STY")
			STY(inst);
		break;
	case 2:
		//register
		if (temp == "TAX")
			TAX(inst);
		if (temp == "TAY")
			TAY(inst);
		if (temp == "TXA")
			TXA(inst);
		if (temp == "TYA")
			TYA(inst);
		break;
	case 3:
		//stack
		if (temp == "TSX")
			TSX(inst);
		if (temp == "TXS")
			TXS(inst);
		if (temp == "PHA")
			PHA(inst);
		if (temp == "PHP")
			PHP(inst);
		if (temp == "PLA")
			PLA(inst);
		if (temp == "PLP")
			PLP(inst);
		break;
	case 4:
		//logical
		if (temp == "AND")
			AND(inst);
		if (temp == "EOR")
			EOR(inst);
		if (temp == "ORA")
			ORA(inst);
		if (temp == "BIT")
			BIT(inst);
		break;
	case 5:
		//arithmetic
		if (temp == "ADC")
			ADC(inst);
		if (temp == "SBC")
			SBC(inst);
		if (temp == "CMP")
			CMP(inst);
		if (temp == "CPX")
			CPX(inst);
		if (temp == "CPY")
			CPY(inst);
		break;
	case 6:
		//increment/decrement
		if (temp == "INC")
			INC(inst);
		if (temp == "INX")
			INX(inst);
		if (temp == "INY")
			INY(inst);
		if (temp == "DEC")
			DEC(inst);
		if (temp == "DEX")
			DEX(inst);
		if (temp == "DEY")
			DEY(inst);
		break;
	case 7:
		//shift
		if (temp == "ASL")
			ASL(inst);
		if (temp == "LSR")
			LSR(inst);
		if (temp == "ROL")
			ROL(inst);
		if (temp == "ROR")
			ROR(inst);
		break;
	case 8:
		//jump
		if (temp == "JMP")
			JMP(inst);
		if (temp == "JSR")
			JSR(inst);
		if (temp == "RTS")
			RTS(inst);
		break;
	case 9:
		//branch
		if (temp == "BCC")
			BCC(inst);
		if (temp == "BCS")
			BCS(inst);
		if (temp == "BEQ")
			BEQ(inst);
		if (temp == "BMI")
			BMI(inst);
		if (temp == "BNE")
			BNE(inst);
		if (temp == "BPL")
			BPL(inst);
		if (temp == "BVC")
			BVC(inst);
		if (temp == "BVS")
			BVS(inst);
		break;
	case 10:
		//flags
		if (temp == "CLC")
			CLC(inst);
		if (temp == "CLD")
			CLD(inst);
		if (temp == "CLI")
			CLI(inst);
		if (temp == "CLV")
			CLV(inst);
		if (temp == "SEC")
			SEC(inst);
		if (temp == "SED")
			SED(inst);
		if (temp == "SEI")
			SEI(inst);
		break;
	case 11:
		//system
		if (temp == "BRK")
			BRK(inst);
		if (temp == "NOP")
			NOP(inst);
		if (temp == "RTI")
			RTI(inst);
		break;
	default:
		break;
	}
	 //handle interrupt
	if (this->interruptPending && !this->interruptDisable) {
		handleInterrupt(inst);
		this->interruptPending = false;
	}
}