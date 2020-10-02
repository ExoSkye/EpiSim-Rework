// EpiSim.cpp : Defines the entry point for the application.
//

#include "EpiSim.h"
#ifdef TRACY_ENABLE
#pragma message("Tracy is enabled")
#endif
int main(int argc, char* argv[])
{
	algo* algorithm;
	renderer* rend;
	std::vector<human> humans;
	{

		ZoneScopedN("Init");
		rend = new sdlRenderer();
		std::default_random_engine* rand = new std::default_random_engine();
		algorithm = new singleAlgo();
		rend->init(1024, 1024);
		human testSubject;
		testSubject.infect_info = infectInfo::susceptible;
		humans.resize(1024, testSubject);
		for (human& person : humans)
		{
			person.x = (rand->operator()() % 1024);
			person.y = (rand->operator()() % 1024);
		}
		humans[1].infect_info = infectInfo::infectious;
	}
	while (true) {
		{
			ZoneScopedN("Algorithm")
			algorithm->run(&humans, 10, 6,1024,1024);
		}
		{
			ZoneScopedN("Drawing")
			rend->drawScreen(humans);
		}
		FrameMark
	}
	return 0;
}
