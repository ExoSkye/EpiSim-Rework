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
    grapher* ImmunePeople;
    grapher* SusceptiblePeople;
    grapher_template<double,100>* RRateGraph;
	SDL_Event e;
	int count = 0;
    SDL_Point *InfectedPoints;
    SDL_Point *ImmunePoints;
    SDL_Point *NotInfectedPoints;
    bool _render;
public:
	bool init(int x, int y, bool render) override;
	bool drawScreen(const std::vector<human>& toDraw) override;
	void end() override;
	~sdlRenderer();
};
