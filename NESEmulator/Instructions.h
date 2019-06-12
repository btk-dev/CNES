#pragma once

#include <string>

using BYTE = unsigned char;

class Instructions {
public:

	struct Instruction {
		BYTE op;
		std::string name;
		int type;
		int length;
		int cycles;
		bool canPageCross;
		int mode;
	};

	Instruction I;

	enum instructionTypes {
		LoadStore = 1,
		Register, //2
		Stack, //3
		Logical, //4
		Arithmetic,  //5
		Increment, //6
		Shift,//7
		Jump, //8
		Branch, //9
		Flag, //10
		System //11
	};

	enum AddressModes {
		Accumulator = 1,
		Immediate, //2
		Implied, //3
		Relative, //4
		Absolute, AbsoluteX, AbsoluteY, //5,6,7
		ZeroPage, ZeroPageX, ZeroPageY, //8,9,10
		Indirect, //11
		IndexedIndirect, //12
		IndirectIndexed, //13
		AbsoluteIndexed, //14
		ZeroPageIndexed //15
	};

	int _InstructionType[0x100] = {
		//0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
		  11,  4,  0,  0,  0,  4,  7,  0,  3,  4,  7,  0,  0,  4,  7,  0, //0
		  9,  4,  0,  0,  0,  4,  7,  0,  10,  4,  0,  0,  0,  4,  7,  0, //1
		  8,  4,  0,  0,  4,  4,  7,  0,  3,  4,  4,  0,  4,  4,  7,  0, //2
		  9,  4,  0,  0,  0,  4,  7,  0,  10,  4,  0,  0,  0,  4,  7,  0, //3
		  11,  4,  0,  0,  0,  4,  7,  0,  3,  4,  7,  0,  8,  4,  7,  0, //4
		  9,  4,  0,  0,  0,  4,  7,  0,  10,  4,  0,  0,  0,  4,  7,  0, //5
		  8,  5,  0,  0,  0,  5,  7,  0,  3,  5,  7,  0,  8,  5,  7,  0, //6
		  8,  5,  0,  0,  0,  5,  7,  0,  10,  5,  0,  0,  0,  5,  7,  0, //7
		  0,  1,  0,  0,  1,  1,  1,  0,  6,  0,  2,  0,  1,  1,  1,  0, //8
		  9,  1,  0,  0,  1,  1,  1,  0,  2,  1,  2,  0,  0,  1,  0,  0, //9
		  1,  1,  1,  0,  1,  1,  1,  0,  2,  1,  2,  0,  1,  1,  1,  0, //A
		  9,  1,  0,  0,  1,  1,  1,  0,  10,  1,  2,  0,  1,  1,  1,  0, //B
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
		  4,  13,  0,  0,  9,  9,  10,  0,  3,  7,  3,  0,  0,  6,  0,  0, //9
		  2,  12,  2,  0,  8,  8,  8,  0,  3,  2,  3,  0,  5,  5,  5,  0, //A
		  4,  13,  0,  0,  9,  9,  10,  0,  3,  7,  3,  0,  6,  6,  7,  0, //B
		  2,  12,  0,  0,  8,  8,  8,  0,  3,  2,  3,  0,  5,  5,  5,  0, //C
		  4,  13,  0,  0,  0,  9,  9,  0,  3,  13,  0,  0,  0,  6,  6,  0, //D
		  2,  12,  0,  0,  8,  8,  8,  0,  3,  2,  3,  0,  5,  5,  5,  0, //E
		  4,  13,  0,  0,  0,  9,  9,  0,  3,  13,  0,  0,  0,  6,  6,  0  //F
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
	//I think I may have to make all these tables static and then create methods to populate them. Or try adding this to cpu class.
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

	Instruction getInstruction(BYTE op) {
		int opcode = op;
		I.op = op;
		I.name = _instructions[opcode];
		I.type = _InstructionType[opcode];
		I.cycles = _InstructionCycles[opcode];
		I.canPageCross = _InstructionPageCrossCycles[opcode];
		I.length = _InstructionLength[opcode];
		I.mode = _InstructionMode[opcode];
		return I;
	};

private:
};
