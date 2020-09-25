#pragma once
#include "render.h"

#include <SDL.h>

class sdlRenderer : public renderer
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
public:
	bool init(int x, int y) override;
	bool drawScreen(std::vector<human> toDraw) override;
	~sdlRenderer();
};
