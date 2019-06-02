#include "cpu.h"

CPU::CPU() {
	CPU::A = CPU::X = CPU::Y = CPU::SP = CPU::PC /*= CPU::opcode = CPU::P*/ = 0;
	CPU::Init(this);
}

CPU::~CPU() {}

void CPU::Init(CPU* cpu) {
	//set P to $34
	//A, X, Y = 0;
	//S = $FD
	//$4017 = $00 (frame irq enabled)
	//$4015 = $00 (all channels disabled)
	//$4000 - $400F = $00
	//all 15 bits of noise channel LFSR = $0000. The first time the LFSR is clocked from the all-0s state it will shift 1
	//internal memory ($0000 - $07FF) is inconsistant. Most emulators set them to a pattern
	//cpu->P = cpu->memory[0x00034];
	cpu->A = cpu->X = cpu->Y = 0;
	cpu->SP = cpu->memory[0xFD];
	cpu->memory[0x4017] = cpu->memory[0x0000];
	cpu->memory[0x4015] = cpu->memory[0x0000];
	for (int i = 0; i < 16; i++) {
		cpu->memory[0x04000 + i] = cpu->memory[0x0000];
	}
}

bool CPU::Reset(CPU* cpu) {
	//A, X, Y not affected
	//S decremented by 3
	//I flag set to true (ORed with $04)
	//Internal memory unchanged
	//APU mode in $4017 unchanged
	//APU silenced ($4015 = 0)
	cpu->SP -= 3;
	//cpu->P & 0x00000F00 | cpu->memory[0x0004];
	return true;
}

void CPU::loadGame(std::vector<BYTE> game) {
	for (int i = 0; i < game.size(); i++)
		this->memory[0x4020 + i] = game[i];
}

bool CPU::isPageCrossed(BYTE A, BYTE B) {
	if ((A & 0xFF) != (B & 0xFF))
		return true;
	return false;
}

void CPU::SetZN(BYTE A) {
	if (A == 0x0)
		zeroFlag = true;
	if ((A & 0xF0) == 0x1)
		negativeFlag = true;
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
		result = (operand2 << 8) + operand;
		this->A = this->memory[result];
		//takes 3 cycles
		this->idleCycles = 3;
		//add 3 to program counter
		this->PC += 3;
		SetZN(this->A);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->A = this->memory[result + this->X];
		//takes 4 cycles, plus one if page crossed. I don't think the below check will work.
		this->idleCycles = 4;
		if (isPageCrossed(this->memory[result], this->memory[result + this->X]))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->A = this->memory[result + this->Y];
		//4 cycles, plus one if page crossed
		this->idleCycles = 4;
		if (isPageCrossed(this->memory[result], this->memory[result + this->Y]))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 8:
		//zero page. Wraparound if > FF.
		if (operand > 0xFF)
			operand -= 0xFF;
		this->A = this->memory[operand];
		this->idleCycles = 3;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 9:
		//zero page, X
		result = operand + this->X;
		if (result > 0xFF)
			result = (operand + this->X) - 0xFF;
		this->A = this->memory[result];
		this->idleCycles = 4;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 12:
		//(indirect, X)
		this->A = this->memory[operand + this->X];
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 13:
		//(indirect), Y
		this->A = this->memory[operand] + this->Y;
		//extra cycle if page cross
		this->idleCycles = 5;
		if (isPageCrossed(this->memory[operand], this->memory[operand] + Y))
			this->idleCycles += 1;
		this->PC += 2;
		SetZN(this->A);
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
		result = (operand2 << 8) + operand;
		this->X = this->memory[result];
		//takes 3 cycles
		this->idleCycles = 3;
		//add 3 to program counter
		this->PC += 3;
		SetZN(this->X);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->X = this->memory[result + this->X];
		//takes 4 cycles, plus one if page crossed.
		this->idleCycles = 4;
		if (isPageCrossed(this->memory[result], this->memory[this->X]))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->X);
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->X = this->memory[result + this->Y];
		//4 cycles, plus one if page crossed
		this->idleCycles = 4;
		if (isPageCrossed(this->memory[result], this->memory[result + this->Y]))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->X);
		break;
	case 8:
		//zero page. Wraparound if > FF.
		if (operand > 0xFF)
			operand -= 0xFF;
		this->X = this->memory[operand];
		this->idleCycles = 3;
		this->PC += 2;
		SetZN(this->X);
		break;
	case 9:
		//zero page, X
		result = operand + this->X;
		if (result > 0xFF)
			result = (operand + this->X) - 0xFF;
		this->X = this->memory[result];
		this->idleCycles = 4;
		this->PC += 2;
		SetZN(this->X);
		break;
	case 12:
		//(indirect, X)
		this->A = this->memory[operand + this->X];
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->X);
		break;
	case 13:
		//(indirect), Y
		this->A = this->memory[operand] + this->Y;
		//extra cycle if page cross. Yet to add
		this->idleCycles = 5;
		if (isPageCrossed(this->memory[operand], this->memory[operand] + Y))
			this->idleCycles += 1;
		this->PC += 2;
		SetZN(this->X);
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
		result = (operand2 << 8) + operand;
		this->Y = this->memory[result];
		//takes 3 cycles
		this->idleCycles = 3;
		//add 3 to program counter
		this->PC += 3;
		SetZN(this->Y);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->Y = this->memory[result + this->X];
		//takes 4 cycles, plus one if page crossed.
		this->idleCycles = 4;
		if (isPageCrossed(this->memory[result], this->memory[result + this->X]))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->Y);
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->Y = this->memory[result + this->Y];
		//4 cycles, plus one if page crossed
		this->idleCycles = 4;
		if (isPageCrossed(this->memory[result], this->memory[this->Y]))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->Y);
		break;
	case 8:
		//zero page. Wraparound if > FF.
		if (operand > 0xFF)
			operand -= 0xFF;
		this->Y = this->memory[operand];
		this->idleCycles = 3;
		this->PC += 2;
		SetZN(this->Y);
		break;
	case 9:
		//zero page, X
		result = operand + this->X;
		if (result > 0xFF)
			result = (operand + this->X) - 0xFF;
		this->Y = this->memory[result];
		this->idleCycles = 4;
		this->PC += 2;
		SetZN(this->Y);
		break;
	case 12:
		//(indirect, X)
		this->Y = this->memory[operand + this->X];
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->Y);
		break;
	case 13:
		//(indirect), Y
		this->Y = this->memory[operand] + this->Y;
		//extra cycle if page cross
		this->idleCycles = 5;
		if (isPageCrossed(this->memory[operand], this->memory[operand] + Y))
			this->idleCycles += 1;
		this->PC += 2;
		SetZN(this->Y);
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
		result = (operand2 >> 8) + operand;
		this->memory[result] = this->A;
		//takes 4 cycles
		this->idleCycles = 4;
		//add 3 to program counter
		this->PC += 3;
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->memory[result + this->X] = this->A;
		//takes 4 cycles, plus one if page crossed.
		this->idleCycles = 5;
		this->PC += 3;
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->memory[result + this->Y] = this->A;
		//4 cycles, plus one if page crossed
		this->idleCycles = 5;
		this->PC += 3;
		break;
	case 8:
		//zero page. Wraparound if > FF.
		if (operand > 0xFF)
			operand -= 0xFF;
		this->memory[operand] = this->A;
		this->idleCycles = 3;
		this->PC += 2;
		break;
	case 9:
		//zero page, X
		result = operand + this->X;
		if (result > 0xFF)
			result = (operand + this->X) - 0xFF;
		this->memory[result] = this->A;
		this->idleCycles = 4;
		this->PC += 2;
		break;
	case 12:
		//(indirect, X)
		this->memory[operand + this->X] = this->A;
		this->idleCycles = 6;
		this->PC += 2;
		break;
	case 13:
		//(indirect), Y
		operand2 = this->memory[operand] + this->Y;
		this->memory[operand2] = this->A;
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
		result = (operand2 >> 8) + operand;
		this->memory[result] = this->X;
		//takes 4 cycles
		this->idleCycles = 4;
		//add 3 to program counter
		this->PC += 3;
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->memory[result + this->X] = this->X;
		//takes 4 cycles, plus one if page crossed.
		this->idleCycles = 5;
		this->PC += 3;
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->memory[result + this->Y] = this->X;
		//4 cycles, plus one if page crossed
		this->idleCycles = 5;
		this->PC += 3;
		break;
	case 8:
		//zero page. Wraparound if > FF.
		if (operand > 0xFF)
			operand -= 0xFF;
		this->memory[operand] = this->X;
		this->idleCycles = 3;
		this->PC += 2;
		break;
	case 9:
		//zero page, X
		result = operand + this->X;
		if (result > 0xFF)
			result = (operand + this->X) - 0xFF;
		this->memory[result] = this->X;
		this->idleCycles = 4;
		this->PC += 2;
		break;
	case 12:
		//(indirect, X)
		this->memory[operand + this->X] = this->X;
		this->idleCycles = 6;
		this->PC += 2;
		break;
	case 13:
		//(indirect), Y
		operand2 = this->memory[operand] + this->Y;
		this->memory[operand2] = this->X;
		//extra cycle if page cross
		this->idleCycles = 5;
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
		result = (operand2 >> 8) + operand;
		this->memory[result] = this->Y;
		//takes 4 cycles
		this->idleCycles = 4;
		//add 3 to program counter
		this->PC += 3;
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->memory[result + this->X] = this->Y;
		this->idleCycles = 5;
		this->PC += 3;
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->memory[result + this->Y] = this->Y;
		//4 cycles, plus one if page crossed
		this->idleCycles = 5;
		this->PC += 3;
		break;
	case 8:
		//zero page. Wraparound if > FF.
		if (operand > 0xFF)
			operand -= 0xFF;
		this->memory[operand] = this->Y;
		this->idleCycles = 3;
		this->PC += 2;
		break;
	case 9:
		//zero page, X
		result = operand + this->X;
		if (result > 0xFF)
			result = (operand + this->X) - 0xFF;
		this->memory[result] = this->Y;
		this->idleCycles = 4;
		this->PC += 2;
		break;
	case 12:
		//(indirect, X)
		this->memory[operand + this->X] = this->Y;
		this->idleCycles = 6;
		this->PC += 2;
		break;
	case 13:
		//(indirect), Y
		operand2 = this->memory[operand] + this->Y;
		this->memory[operand2] = this->Y;
		//extra cycle if page cross
		this->idleCycles = 5;
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
	P = (carryFlag >> 0) + (zeroFlag >> 1) + (interruptDisable >> 2) + (decimalModeFlag >> 3) + (bFlag1 >> 4) + (bFlag2 >> 5) + (overflowFlag >> 6) + (negativeFlag >> 7);
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
}
void CPU::PLP(Instructions::Instruction I) {
	if (I.mode != 3)
		return;
	this->SP += 1;
	BYTE P;
	P = this->stack[this->SP];
	carryFlag = (P & 0x00000001);
	zeroFlag = (P & 0x00000010);
	interruptDisable = (P & 0x00000100);
	decimalModeFlag = (P & 0x00001000);
	bFlag1 = (P & 0x00010000);
	bFlag2 = (P & 0x00100000);
	overflowFlag = (P & 0x01000000);
	negativeFlag = (P & 0x10000000);
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
		result = (operand2 << 8) + operand;
		this->A = this->memory[result] & this->A;
		this->idleCycles = 4;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->A = this->memory[result + this->X] & this->A;
		this->idleCycles = 4;
		//for page crossed do I need the below or do I need this->memory[result], this->memory[result + this->X]. This would need to be replaced at every absolute switch
		if (isPageCrossed(result, result + this->X))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand << 8) + operand;
		this->A = (this->memory[result] + this->Y) & this->A;
		this->idleCycles = 4;
		if (isPageCrossed(result, result + this->X))
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
		if ((operand + this->X) > 0xFF)
			operand = (operand + this->X) - 0xFF;
		this->A = this->memory[(operand + this->X)] & this->A;
		this->idleCycles = 4;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 12:
		//(indirect, X)
		this->A = (this->memory[(operand + this->X)]) & this->A;
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 13:
		//(indirect), Y
		this->A = (this->memory[operand] + this->Y) & this->A;
		this->idleCycles = 5;
		if (isPageCrossed((this->memory[operand] + this->Y), (this->memory[operand])))
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
		result = (operand2 << 8) + operand;
		this->A = this->A ^ result;
		this->idleCycles = 4;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->A = this->A ^ (result + this->X);
		this->idleCycles = 4;
		if (isPageCrossed(result, result + this->X))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->A = this->A ^ (result + this->Y);
		this->idleCycles = 4;
		if (isPageCrossed(result, result + this->Y))
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
		if ((operand + this->X)> 0xFF)
			operand = (operand + this->X) - 0xFF;
		this->A = this->memory[operand] ^ this->A;
		this->idleCycles = 4;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 12:
		//(indirect, X)
		this->A = this->memory[operand + this->X] ^ this->A;
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 13:
		//(indirect), Y
		this->A = (this->memory[operand] + this->Y) ^ this->A;
		this->idleCycles = 5;
		if (isPageCrossed((this->memory[operand] + this->Y), (this->memory[operand])))
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
		result = (operand2 << 8) + operand;
		this->A = this->A | result;
		this->idleCycles = 4;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->A = this->A | (result + this->X);
		this->idleCycles = 4;
		if (isPageCrossed(result, result + this->X))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->A);
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->A = this->A | (result + this->Y);
		this->idleCycles = 4;
		if (isPageCrossed(result, result + this->Y))
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
		if ((operand + this->X) > 0xFF)
			operand = (operand + this->X) - 0xFF;
		this->A = this->memory[operand] | this->A;
		this->idleCycles = 4;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 12:
		//(indirect, X)
		this->A = this->memory[operand + this->X] | this->A;
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->A);
		break;
	case 13:
		//(indirect), Y
		this->A = (this->memory[operand] + this->Y) | this->A;
		this->idleCycles = 5;
		if (isPageCrossed((this->memory[operand] + this->Y), (this->memory[operand])))
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
		result = (operand2 << 8) + operand;
		if ((this->A & result) == 0x00)
			zeroFlag = 0;
		else zeroFlag = 1;
		if ((result & 0x01000000) == 0x00)
			overflowFlag = 0;
		else overflowFlag = 1;
		if ((result & 0x10000000) == 0x00)
			negativeFlag = 0;
		else negativeFlag = 1;
		this->idleCycles = 4;
		this->PC += 3;
		break;
	case 8:
		//zero page.
		if ((this->A & operand) == 0x00)
			zeroFlag = 0;
		else zeroFlag = 1;
		if ((operand & 0x01000000) == 0x00)
			overflowFlag = 0;
		else
			overflowFlag = 1;
		if ((operand & 0x10000000) == 0x00)
			negativeFlag = 0;
		else
			negativeFlag = 1;
		this->idleCycles = 3;
		this->PC += 2;
		break;
	default:
		break;
	}
}

//artithmetic
//adc and sbc not done yet. Need to create algorithm for overflow flag
//none of the arithmetic opcodes completed.
void CPU::ADC(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 2:
		//immediate
		this->A = this->A + operand;
		this->idleCycles = 2;
		this->PC += 2;
		SetZN(this->A);
		if (negativeFlag == 1)
			carryFlag = 1;
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 >> 8) + operand;
		this->A = this->A + result;
		this->idleCycles = 4;
		this->PC += 3;
		SetZN(this->A);
		if (negativeFlag == 1)
			carryFlag = 1;
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 >> 8) + operand;
		this->A = this->A + (this->X + result);
		this->idleCycles = 4;
		if (isPageCrossed(result, result + this->X))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->A);
		if (negativeFlag == 1)
			carryFlag = 1;
		break;
	case 7:
		//absolute, Y
		operand2 = this->memory[this->PC + 2];
		result = (operand2 >> 8) + operand;
		this->A = this->A + (this->Y + result);
		this->idleCycles = 4;
		if (isPageCrossed(result, result + this->Y))
			this->idleCycles += 1;
		this->PC += 3;
		SetZN(this->A);
		if (negativeFlag == 1)
			carryFlag = 1;
		break;
	case 8:
		//zero page. wraparound if > 0xFF
		if (operand > 0xFF)
			operand -= 0xFF;
		this->A = this->A + operand;
		this->idleCycles = 3;
		this->PC += 2;
		SetZN(this->A);
		if (negativeFlag == 1)
			carryFlag = 1;
		break;
	case 9:
		//zero page, X
		if ((operand + this->X) > 0xFF)
			operand = (operand + this->X) - 0xFF;
		this->A = this->A + operand;
		this->idleCycles = 4;
		this->PC += 2;
		SetZN(this->A);
		if (negativeFlag == 1)
			carryFlag = 1;
		break;
	case 12:
		//indirect, X
		this->A = this->memory[operand + this->X] + this->A;
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->A);
		if (negativeFlag == 1)
			carryFlag = 1;
		break;
	case 13:
		//indirect, Y
		this->A = (this->memory[operand] + this->Y) + this->A;
		this->idleCycles = 5;
		if (isPageCrossed((this->memory[operand] + this->Y), (this->memory[operand])))
			this->idleCycles += 1;
		this->PC += 2;
		SetZN(this->A);
		if (negativeFlag == 1)
			carryFlag = 1;
		break;
	default:
		break;
	}
}
void CPU::SBC(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	switch (I.mode) {
	case 2:
		//immediate
		this->A = this->A - operand;
		break;
	case 5:
		//absolute
		break;
	case 6:
		//absolute, X
		break;
	case 7:
		//absolute, Y
		break;
	case 8:
		//zero page
		break;
	case 9:
		//zero page, X
		break;
	case 12:
		//indirect, X
		break;
	case 13:
		//indirect, Y
		break;
	default:
		break;
	}
}
void CPU::CMP(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	BYTE operand2;
	unsigned short result;
	//I believe set ZN(this->A - operand)
	switch (I.mode) {
	case 2:
		//immediate
		break;
	case 5:
		//absolute
		break;
	case 6:
		//absolute, X
		break;
	case 7:
		//absolute, Y
		break;
	case 8:
		//zero page
		break;
	case 9:
		//zero page, X
		break;
	case 12:
		//indirect, X
		break;
	case 13:
		//indirect, Y
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
		break;
	case 5:
		//absolute
		break;
	case 6:
		//absolute, X
		break;
	case 7:
		//absolute, Y
		break;
	case 8:
		//zero page
		break;
	case 9:
		//zero page, X
		break;
	case 12:
		//indirect, X
		break;
	case 13:
		//indirect, Y
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
		break;
	case 5:
		//absolute
		break;
	case 6:
		//absolute, X
		break;
	case 7:
		//absolute, Y
		break;
	case 8:
		//zero page
		break;
	case 9:
		//zero page, X
		break;
	case 12:
		//indirect, X
		break;
	case 13:
		//indirect, Y
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
		result = (operand2 << 8) + operand;
		this->memory[result] += 1;
		this->idleCycles = 6;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
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
		this->memory[operand + this->X] += 1;
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->memory[operand + this->X]);
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
		result = (operand2 << 8) + operand;
		this->memory[result] -= 1;
		this->idleCycles = 6;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
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
		this->memory[operand + this->X] -= 1;
		this->idleCycles = 6;
		this->PC += 2;
		SetZN(this->memory[operand + this->X]);
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
		result = (operand2 << 8) + operand;
		carryFlag = this->memory[result] & 0x80;
		this->memory[result] = this->memory[result] << 1;
		this->idleCycles = 6;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		carryFlag = this->memory[result] & 0x80;
		this->memory[result + this->X] = this->memory[result + this->X] << 1;
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
		carryFlag = this->memory[operand + this->X] & 0x80;
		this->memory[operand + this->X] = this->memory[operand + this->X] << 1;
		this->idleCycles = 6;
		this->PC += 3;
		SetZN(this->memory[operand + this->X]);
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
		result = (operand2 << 8) + operand;
		carryFlag = this->memory[result] & 0x01;
		this->memory[result] = this->memory[result] >> 1;
		this->idleCycles = 6;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		carryFlag = this->memory[result] & 0x01;
		this->memory[result + this->X] = this->memory[result + this->X] >> 1;
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
		carryFlag = this->memory[operand + this->X] & 0x01;
		this->memory[operand + this->X] = this->memory[operand + this->X] >> 1;
		this->idleCycles = 6;
		this->PC += 3;
		SetZN(this->memory[operand + this->X]);
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
		result = (operand2 << 8) + operand;
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
		result = (operand2 << 8) + operand;
		operand = carryFlag;
		carryFlag = this->memory[result] & 0x80;
		this->memory[result + this->X] = this->memory[result + this->X] << 1;
		this->memory[result + this->X] = this->memory[result + this->X] | operand;
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
		result = carryFlag;
		carryFlag = this->memory[operand + this->X] & 0x80;
		this->memory[operand + this->X] = this->memory[operand + this->X] << 1;
		this->memory[operand + this->X] = this->memory[operand + this->X] | result;
		this->idleCycles = 6;
		this->PC += 3;
		SetZN(this->memory[operand + this->X]);
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
		this->A = this->A | result << 8;
		SetZN(this->A);
		this->idleCycles = 2;
		this->PC += 1;
		break;
	case 5:
		//absolute
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		operand = carryFlag;
		carryFlag = this->memory[result] & 0x01;
		this->memory[result] = this->memory[result] << 1;
		this->memory[result] = this->memory[result] | (operand << 8);
		this->idleCycles = 6;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 6:
		//absolute, X
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		operand = carryFlag;
		carryFlag = this->memory[result] & 0x80;
		this->memory[result + this->X] = this->memory[result + this->X] << 1;
		this->memory[result + this->X] = this->memory[result + this->X] | (operand << 8);
		this->idleCycles = 7;
		this->PC += 3;
		SetZN(this->memory[result]);
		break;
	case 8:
		//zero page
		result = carryFlag;
		carryFlag = this->memory[operand] & 0x80;
		this->memory[operand] = this->memory[operand] << 1;
		this->memory[operand] = this->memory[operand] | (operand << 8);
		this->idleCycles = 5;
		this->PC += 2;
		SetZN(this->memory[operand]);
		break;
	case 9:
		//zero page, X
		result = carryFlag;
		carryFlag = this->memory[operand + this->X] & 0x80;
		this->memory[operand + this->X] = this->memory[operand + this->X] << 1;
		this->memory[operand + this->X] = this->memory[operand + this->X] | (operand << 8);
		this->idleCycles = 6;
		this->PC += 3;
		SetZN(this->memory[operand + this->X]);
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
		result = (operand2 << 8) + operand;
		this->stack[this->SP] = this->PC;
		this->SP--;
		this->PC = result;
		this->idleCycles = 3;
		this->PC += 3;
		break;
	case 11:
		//indirect
		//An original 6502 has does not correctly fetch the target address if the indirect vector falls on a page boundary (e.g. $xxFF where xx is any value from $00 to $FF). 
		//In this case fetches the LSB from $xxFF as expected but takes the MSB from $xx00. 
		//This is fixed in some later chips like the 65SC02 so for compatibility always ensure the indirect vector is not at the end of the page.
		//the error is not implemented yet.
		this->stack[this->SP] = this->PC;
		this->SP--;
		operand2 = this->memory[this->PC + 2];
		result = (operand2 << 8) + operand;
		this->PC = this->memory[result];
		this->idleCycles = 5;
		this->PC += 3;
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
	this->stack[this->SP] = this->PC - 1;
	this->SP--;
	operand2 = this->memory[this->PC + 2];
	result = (operand2 << 8) + operand;
	this->PC = this->memory[result];
	this->idleCycles = 6;
	this->PC += 3;
}
void CPU::RTS(Instructions::Instruction I) {
	BYTE operand = this->memory[this->PC + 1];
	if (I.mode != 3)
		return;
	this->SP += 1;
	this->PC = this->stack[this->SP];
	this->idleCycles = 6;
	this->PC += 1;
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
	BYTE P;
	//add flags to byte.
	//this->stack[this->SP] = P;
	this->SP -= 1;
	//set break flag to 1
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
	negativeFlag = (P & 0x80);
	overflowFlag = (P & 0x70);
	this->idleCycles = 6;
	this->PC += 1;
}

void CPU::Clock_Tick() {
	//check for interrupts

	//retrieve opcode
	//execute opcode
	
	//the 6502 is little endian (least sig bytes first)
	//stack pointer counts down from 0xFF to 0x00 with no overflow protection

	//wait for clock ticks to simulate NES speed

	if (this->idleCycles > 0) {
		this->idleCycles--;
		return;
	}

	this->opcode = this->memory[this->PC] & 0x00FF;

	inst = I.getInstruction(opcode);

	switch (inst.type) {
		//I hate this. Want to change later
	case 1:
		//load and store
		if (inst.name == "LDA")
			LDA(inst);
		if (inst.name == "LDX")
			LDX(inst);
		if (inst.name == "LDY")
			LDY(inst);
		if (inst.name == "STA")
			STA(inst);
		if (inst.name == "STX")
			STX(inst);
		if (inst.name == "STY")
			STY(inst);
		break;
	case 2:
		//register
		if (inst.name == "TAX")
			TAX(inst);
		if (inst.name == "TAY")
			TAY(inst);
		if (inst.name == "TXA")
			TXA(inst);
		if (inst.name == "TYA")
			TYA(inst);
		break;
	case 3:
		//stack
		if (inst.name == "TSX")
			TSX(inst);
		if (inst.name == "TXS")
			TXS(inst);
		if (inst.name == "PHA")
			PHA(inst);
		if (inst.name == "PHP")
			PHP(inst);
		if (inst.name == "PLA")
			PLA(inst);
		if (inst.name == "PLP")
			PLP(inst);
		break;
	case 4:
		//logical
		if (inst.name == "AND")
			AND(inst);
		if (inst.name == "EOR")
			EOR(inst);
		if (inst.name == "ORA")
			ORA(inst);
		if (inst.name == "BIT")
			BIT(inst);
		break;
	case 5:
		//arithmetic
		if (inst.name == "ADC")
			ADC(inst);
		if (inst.name == "SBC")
			SBC(inst);
		if (inst.name == "CMP")
			CMP(inst);
		if (inst.name == "CPX")
			CPX(inst);
		if (inst.name == "CPY")
			CPY(inst);
		break;
	case 6:
		//increment/decrement
		if (inst.name == "INC")
			INC(inst);
		if (inst.name == "INX")
			INX(inst);
		if (inst.name == "INY")
			INY(inst);
		if (inst.name == "DEC")
			DEC(inst);
		if (inst.name == "DEX")
			DEX(inst);
		if (inst.name == "DEY")
			DEY(inst);
		break;
	case 7:
		//shift
		if (inst.name == "ASL")
			ASL(inst);
		if (inst.name == "LSR")
			LSR(inst);
		if (inst.name == "ROL")
			ROL(inst);
		if (inst.name == "ROR")
			ROR(inst);
		break;
	case 8:
		//jump
		if (inst.name == "JMP")
			JMP(inst);
		if (inst.name == "JSR")
			JSR(inst);
		if (inst.name == "RTS")
			RTS(inst);
		break;
	case 9:
		//branch
		if (inst.name == "BCC")
			BCC(inst);
		if (inst.name == "BCS")
			BCS(inst);
		if (inst.name == "BEQ")
			BEQ(inst);
		if (inst.name == "BMI")
			BMI(inst);
		if (inst.name == "BNE")
			BNE(inst);
		if (inst.name == "BPL")
			BPL(inst);
		if (inst.name == "BVC")
			BVC(inst);
		if (inst.name == "BVS")
			BVS(inst);
		break;
	case 10:
		//flags
		if (inst.name == "CLC")
			CLC(inst);
		if (inst.name == "CLD")
			CLD(inst);
		if (inst.name == "CLI")
			CLI(inst);
		if (inst.name == "CLV")
			CLV(inst);
		if (inst.name == "SEC")
			SEC(inst);
		if (inst.name == "SED")
			SED(inst);
		if (inst.name == "SEI")
			SEI(inst);
		break;
	case 11:
		//system
		if (inst.name == "BRK")
			BRK(inst);
		if (inst.name == "NOP")
			NOP(inst);
		if (inst.name == "RTI")
			RTI(inst);
		break;
	default:
		break;
	}

	this->PC += 1;
	std::cout << this->PC;
}