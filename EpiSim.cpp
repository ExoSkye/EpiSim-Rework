// EpiSim.cpp : Defines the entry point for the application.
//

#include "EpiSim.h"
#ifdef TRACY_ENABLE
#pragma message("Tracy is enabled")
#endif
int main(int argc, char* argv[])
{
    int x, y, population, infectChance, infectRadius, length;
    {
        ZoneScopedN("Read Config");
        INIReader reader("./conf.ini");
        if (reader.ParseError() != 0) {
            printf("Can't load 'conf.ini'\n");
            return 1;
        }
        x = reader.GetInteger("Window","x",1024);
        y = reader.GetInteger("Window","y",1024);
        population = reader.GetInteger("Simulation","population",10240);
        infectChance = reader.GetInteger("Simulation","infectChance",10);
        infectRadius = reader.GetInteger("Simulation","infectRadius",6);
        length = reader.GetInteger("Simulation","length",1000);
    }
	algo* algorithm;
	renderer* rend;
	std::vector<human> humans;
	{

		ZoneScopedN("Init");
		rend = new sdlRenderer();
		auto* rand = new std::default_random_engine();
		algorithm = new singleAlgo();
		rend->init(x, y);
		human testSubject{};
		testSubject.infect_info = infectInfo::susceptible;
		humans.resize(population, testSubject);
		for (human& person : humans)
		{
			person.x = (rand->operator()() % 1024);
			person.y = (rand->operator()() % 1024);
		}
		humans[1].infect_info = infectInfo::infectious;
	}
	for (int i = 0; i < length; i++) {
		{
			ZoneScopedN("Algorithm")
			algorithm->run(&humans, infectChance, infectRadius,x,y);
		}
		{
			ZoneScopedN("Drawing")
			rend->drawScreen(humans);
		}
		FrameMark;
	}
	rend->end();
	algorithm->end();
	return 0;
}
