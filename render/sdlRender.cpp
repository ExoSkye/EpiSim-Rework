#include "sdlRender.h"

bool sdlRenderer::init(int x, int y)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("EpiSim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x, y, 0);
#ifdef _DEBUG
    printf("SDL (After window creation) Current error = %s\n",SDL_GetError());
#endif
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
#ifdef _DEBUG
    printf("SDL (After renderer creation) Current error = %s\n",SDL_GetError());
#endif
	return (window == nullptr || renderer == nullptr);
}

bool sdlRenderer::drawScreen(std::vector<human> toDraw)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	/*SDL_Rect screenRect = { 0,0,1024,1024 };
	SDL_RenderDrawRect(renderer, &screenRect);*/
	SDL_RenderClear(renderer);
	for (human curHuman : toDraw)
	{
		color curHumanColor = possibleColors[(int)curHuman.infect_info];
		SDL_SetRenderDrawColor(renderer, curHumanColor.r, curHumanColor.g, curHumanColor.b, 255);
		SDL_RenderDrawPoint(renderer, curHuman.x, curHuman.y);
	}
	SDL_RenderPresent(renderer);
	return true;
}

sdlRenderer::~sdlRenderer()
{
	SDL_Quit();
}

void sdlRenderer::end() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
