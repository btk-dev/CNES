#define _CRTDBB_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//Above are for memory leak checks
#include <iostream>

#include "Console.h"

int main() {

	Console nes;
	nes.createCPU();
	nes.createPPU();
	//nes.loadCartridge("M:\\VisualStudio\\Workspaces\\NESEmulator\\nestest.nes");
	//donkey kong first opcode crashes
	nes.loadCartridge("M:\\VisualStudio\\Workspaces\\NESEmulator\\Donkey_kong.nes");
	//crashing loading the below, too big.
	//nes.loadCartridge("M:\\VisualStudio\\Workspaces\\NESEmulator\\official.nes");
	//nes.loadCartridge("M:\\VisualStudio\\Workspaces\\NESEmulator\\official_only.nes");
	//nes.loadCartridge("M:\\VisualStudio\\Workspaces\\NESEmulator\\01-basics.nes");
	nes.ppu.windowInit();
	while (nes.is_Running()) {
		nes.cpu.Clock_Tick();
		//ppu does 3 ticks for every 1 cpu tick
		nes.ppu.Clock_Tick();
		nes.ppu.Clock_Tick();
		nes.ppu.Clock_Tick();
		//apu does 1 tick for every 1 cpu tick

		nes.ppu.sendRender();
		//SDL_UpdateTexture(tex, NULL, pixels, 64 * sizeof(Uint32));
		//SDL_RenderClear(renderer);
		//SDL_RenderCopy(renderer, tex, NULL, NULL);
		//SDL_RenderPresent(renderer);
	}

	_CrtDumpMemoryLeaks();
	return 0;
}