#pragma once

#include <SDL.h>
#undef main
const int height = 512;
const int width = 1024;

class gui {
public:
	void createWindow();
	void update(unsigned int*);
	void render();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture *tex;
};
