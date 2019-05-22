#include "Console.h"

CPU Console::createCPU() {
	CPU cpu();
}

void Console::resetConsole(CPU* cpu) {
	cpu->Reset(cpu);
}