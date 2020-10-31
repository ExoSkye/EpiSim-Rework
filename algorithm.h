#pragma once
#include <vector>
#include <random>
#include "human.h"
#include "tracy/Tracy.hpp"
#include "tracy/TracyC.h"
struct square
{
	int x1, x2, y1, y2;
};
class algo
{
public:
	virtual void
    run(std::vector<human> *humans, int infectChance, int infectRadius, int x, int y, double immuneChance,
        int immuneLength,
        int immuneLengthVar, int timestep) = 0;
	virtual void end() = 0;
};