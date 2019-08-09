#include "gui.h"

void gui::createWindow()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("CNES", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_RenderSetLogicalSize(renderer, width, height);

	tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
}

void gui::update(unsigned int *pixels)
{
	SDL_UpdateTexture(tex, NULL, pixels, width * sizeof(unsigned int));
}

void gui::render()
{
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, tex, NULL, NULL);

	SDL_RenderPresent(renderer);
}
