#include "cpu.h"

CPU::CPU() {
	CPU::A = CPU::X = CPU::Y = CPU::SP = CPU::PC /*= CPU::opcode = CPU::P*/ = 0;
	for (int i = 0; i < 0x10000; i++)
		CPU::memory[i] = 0;
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
	cpu->memory[0x4017] = cpu->memory[0x0];
	cpu->memory[0x4015] = cpu->memory[0x0];
	for (int i = 0; i < 16; i++) {
		cpu->memory[0x04000 + i] = cpu->memory[0x0];
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

//void brk() {}
//void ora(CPU* cpu, BYTE mem) {cpu->memory[mem]}

void CPU::Clock_Tick(CPU* cpu) {
	//retrieve opcode
	//execute opcode
	
	//the 6502 is little endian (least sig bytes first)
	//stack pointer counts down from 0xFF to 0x00 with no overflow protection

	//wait for clock ticks to simulate NES speed
	cpu->opcode = cpu->memory[cpu->PC] & 0x00FF;

	switch (opcode) {
	case 0x00: /*brk(); Pushes PC to stack, sets interupt flag, loads PC from vector at FFFE-FFFF pc =+ 2*/break;
	case 0x01: /*ora(cpu, cpu->memory[cpu->PC + 1]);*/ cpu->A = (cpu->memory[cpu->PC + 1] | cpu->X); cpu->PC += 2; cpu->negativeFlag = 0; cpu->zeroFlag = 0; break;
	case 0x05: /*Zero page ORA*/ cpu->A = (cpu->memory[cpu->PC + 1] | cpu->X);  cpu->PC += 2; break;
	}
}