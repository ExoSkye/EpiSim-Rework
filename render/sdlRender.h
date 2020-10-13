#pragma once
#include "render.h"
#include "../grapher.hpp"
#include <SDL.h>

class sdlRenderer : public renderer
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	grapher* InfectedPeople;
public:
	bool init(int x, int y) override;
	bool drawScreen(const std::vector<human>& toDraw) override;
	void end() override;
	~sdlRenderer();
};
