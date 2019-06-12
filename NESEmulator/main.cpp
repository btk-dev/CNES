#define _CRTDBB_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//Above are for memory leak checks
#include <iostream>

#include "Console.h"

int main() {
	Console nes;
	nes.createCPU();
	nes.loadCartridge("M:\\VisualStudio\\Workspaces\\NESEmulator\\nestest.nes");
	while (nes.is_Running()) {
		nes.cpu.Clock_Tick();
	}

	_CrtDumpMemoryLeaks();
	return 0;
}