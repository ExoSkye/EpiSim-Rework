// EpiSim.cpp : Defines the entry point for the application.
//

#include "EpiSim.h"
#ifdef TRACY_ENABLE
#pragma message("Tracy is enabled")
#endif
int main(int argc, char* argv[])
{
    int x, y, population, infectChance, infectRadius, length,peopleInfected,immuneChance,immuneLength;
    bool render;
    {
        ZoneScopedN("Read Config");
        INIReader reader("./conf.ini");
        if (reader.ParseError() != 0) {
            printf("Can't load 'conf.ini'\n");
            return 1;
        }
        x = reader.GetInteger("Window", "x", 1024);
        y = reader.GetInteger("Window", "y", 1024);
        population = reader.GetInteger("Simulation", "population", 10240);
        infectChance = reader.GetInteger("Simulation", "infectChance", 10);
        infectRadius = reader.GetInteger("Simulation", "infectRadius", 6);
        length = reader.GetInteger("Simulation", "length", 1000);
        peopleInfected = reader.GetInteger("Simulation", "peopleInfected", 1);
        render = reader.GetBoolean("Window","render",true);
        immuneChance = reader.GetReal("Simulation","immuneChance",0.1);
        immuneLength = reader.GetInteger("Simulation","immuneLength",0);

    }
	algo* algorithm;
	renderer* rend;
	std::vector<human> humans;
	{

		ZoneScopedN("Init");
		if (render) {
            rend = new sdlRenderer();
            rend->init(x, y);
        }
		auto* rand = new std::default_random_engine();
		algorithm = new multiAlgo();
		human testSubject{};
		testSubject.infect_info = infectInfo::susceptible;
		humans.resize(population, testSubject);
		for (human& person : humans)
		{
			person.x = (rand->operator()() % x);
			person.y = (rand->operator()() % y);
		}
		for (int i = 0; i < peopleInfected; i ++) {
		    int index = rand->operator()() % humans.size();
		    if (humans[index].infect_info != infectInfo::infectious) {
                humans[index].infect_info = infectInfo::infectious;
            }
		    else {
		        i++;
		    }
		}
	}

	for (int i = 0; i < length; i++) {
		{
            printf("Working out frame %d\n",i);
			ZoneScopedN("Algorithm")
            algorithm->run(&humans, infectChance, infectRadius, x, y, immuneChance, 1000);
		}
		if (render) {
			ZoneScopedN("Drawing")
			rend->drawScreen(humans);
		}
		FrameMark;
	}
	if (render) {
        rend->end();
    }
	algorithm->end();
	return 0;
}
