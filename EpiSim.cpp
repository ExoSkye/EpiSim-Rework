// EpiSim.cpp : Defines the entry point for the application.
//

#include "EpiSim.h"
#include "algononnaive.h"

#ifdef TRACY_ENABLE
#pragma message("Tracy is enabled")
#endif
int main(int argc, char* argv[])
{
    int x, y, timestep, population, infectChance, infectRadius, length,peopleInfected,immuneChance,immuneLength,immuneLengthVar,algo_choice;
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
        immuneLengthVar = reader.GetInteger("Simulation","immuneLengthVar",1);
        std::string algo_input = reader.Get("Simulation","algorithm","single");
        timestep = reader.GetInteger("Simulation","timestep",1);
        if (algo_input == "single")
            algo_choice = 0;
        else if (algo_input == "multi")
            algo_choice = 1;
        else if (algo_input == "opencl")
            algo_choice = 2;
    }
	algo* algorithm;
	renderer* rend;
	std::vector<human> humans;
	{

		ZoneScopedN("Init");
		rend = new sdlRenderer();
		rend->init(x, y);
        if (!render) {
            rend->pause = true;
        }
		auto* rand = new std::default_random_engine();
        switch (algo_choice) {
            case 0:
                algorithm = new singleAlgo();
                break;
            case 1:
                algorithm = new multiAlgo();
                break;
            case 2:
                algorithm = new multiAlgo();
                break;
            default:
                algorithm = new multiAlgo();
                break;
        }
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
            algorithm->run(&humans, infectChance, infectRadius, x, y, immuneChance, immuneLength, immuneLengthVar, timestep);
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
