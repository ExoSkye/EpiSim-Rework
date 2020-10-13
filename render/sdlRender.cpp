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
    InfectedPeople = new grapher(1024,"Infected People",16,256,1,true);

    return (window == nullptr || renderer == nullptr);
}

bool sdlRenderer::drawScreen(const std::vector<human>& toDraw)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	/*SDL_Rect screenRect = { 0,0,1024,1024 };
	SDL_RenderDrawRect(renderer, &screenRect);*/
	SDL_RenderClear(renderer);
	int infected = 0;
	int notinfected = 0;
	int immune = 0;
	SDL_Point* InfectedPoints = (SDL_Point*)alloca(toDraw.size()*sizeof(SDL_Point));
    SDL_Point* ImmunePoints = (SDL_Point*)alloca(toDraw.size()*sizeof(SDL_Point));
    SDL_Point* NotInfectedPoints = (SDL_Point*)alloca(toDraw.size()*sizeof(SDL_Point));
	for (human curHuman : toDraw)
	{
		if (curHuman.infect_info == infectInfo::infectious) {
		    InfectedPoints[infected] = {curHuman.x,curHuman.y};
		    infected++;
		}
        else if (curHuman.infect_info == infectInfo::susceptible) {
            NotInfectedPoints[notinfected] = {curHuman.x,curHuman.y};
            notinfected++;
        }
        else if (curHuman.infect_info == infectInfo::immune) {
            ImmunePoints[immune] = {curHuman.x,curHuman.y};
            immune++;
        }
	}
	InfectedPeople->append(infected);
    InfectedPeople->update();
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	SDL_RenderDrawPoints(renderer,NotInfectedPoints,notinfected);
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    SDL_RenderDrawPoints(renderer,InfectedPoints,infected);
    SDL_SetRenderDrawColor(renderer,127,127,127,255);
    SDL_RenderDrawPoints(renderer,ImmunePoints,immune);
	SDL_RenderPresent(renderer);
	return true;
}

sdlRenderer::~sdlRenderer()
{
	SDL_Quit();
}

void sdlRenderer::end() {
    InfectedPeople->closeGraph();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
