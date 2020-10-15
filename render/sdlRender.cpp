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
    ImmunePeople = new grapher(1024,"Immune People",16,256,1,true);
    SusceptiblePeople = new grapher(1024,"Susceptible People",16,256,1,true);
    return (window == nullptr || renderer == nullptr);
}

bool sdlRenderer::drawScreen(const std::vector<human>& toDraw)
{
    while( SDL_PollEvent( &e ) != 0 )
    {
        //User requests quit
        switch (e.type) {
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_SPACE:
                        pause = true;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (e.key.keysym.sym) {
                    case SDLK_SPACE:
                        pause = false;
                        break;
                }
                break;
        }
    }
    int infected = 0;
    int notinfected = 0;
    int immune = 0;
    count++;
    if (count == 1) {
        InfectedPoints = (SDL_Point *) alloca(toDraw.size() * sizeof(SDL_Point));
        ImmunePoints = (SDL_Point *) alloca(toDraw.size() * sizeof(SDL_Point));
        NotInfectedPoints = (SDL_Point *) alloca(toDraw.size() * sizeof(SDL_Point));
    }
    for (human curHuman : toDraw) {
        if (curHuman.infect_info == infectInfo::infectious) {
            InfectedPoints[infected] = {curHuman.x, curHuman.y};
            infected++;
        } else if (curHuman.infect_info == infectInfo::susceptible) {
            NotInfectedPoints[notinfected] = {curHuman.x, curHuman.y};
            notinfected++;
        } else if (curHuman.infect_info == infectInfo::immune) {
            ImmunePoints[immune] = {curHuman.x, curHuman.y};
            immune++;
        }
    }
    if (!pause) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        /*SDL_Rect screenRect = { 0,0,1024,1024 };
        SDL_RenderDrawRect(renderer, &screenRect);*/
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawPoints(renderer, NotInfectedPoints, notinfected);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawPoints(renderer, InfectedPoints, infected);
        SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
        SDL_RenderDrawPoints(renderer, ImmunePoints, immune);
        SDL_RenderPresent(renderer);
    }
    InfectedPeople->append(infected);
    ImmunePeople->append(immune);
    SusceptiblePeople->append(notinfected);
    InfectedPeople->update();
    ImmunePeople->update();
    SusceptiblePeople->update();
	return true;
}

sdlRenderer::~sdlRenderer()
{
	SDL_Quit();
}

void sdlRenderer::end() {
    InfectedPeople->closeGraph();
    SusceptiblePeople->closeGraph();
    ImmunePeople->closeGraph();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
