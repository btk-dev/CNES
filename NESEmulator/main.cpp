#define _CRTDBB_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//Above are for memory leak checks
#include <iostream>
#include <SDL.h>

#undef main

#include "Console.h"

const int height = 1024;
const int width = 512;

int main() {

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow("CNES", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_RenderSetLogicalSize(renderer, width, height);

	SDL_Texture* tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

	Console nes;
	nes.createCPU();
	//nes.loadCartridge("M:\\VisualStudio\\Workspaces\\NESEmulator\\nestest.nes");
	//donkey kong first opcode crashes
	nes.loadCartridge("M:\\VisualStudio\\Workspaces\\NESEmulator\\Donkey_kong.nes");
	//crashing loading the below, too big.
	//nes.loadCartridge("M:\\VisualStudio\\Workspaces\\NESEmulator\\official.nes");
	//nes.loadCartridge("M:\\VisualStudio\\Workspaces\\NESEmulator\\official_only.nes");
	//nes.loadCartridge("M:\\VisualStudio\\Workspaces\\NESEmulator\\01-basics.nes");
	while (nes.is_Running()) {
		nes.cpu.Clock_Tick();
		//ppu does 3 ticks for every 1 cpu tick
		//apu does 1 tick for every 1 cpu tick


		//SDL_UpdateTexture(tex, NULL, pixels, 64 * sizeof(Uint32));
		//SDL_RenderClear(renderer);
		//SDL_RenderCopy(renderer, tex, NULL, NULL);
		//SDL_RenderPresent(renderer);
	}

	_CrtDumpMemoryLeaks();
	return 0;
}