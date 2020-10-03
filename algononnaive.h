#pragma once
#include "algorithm.h"
#include "grapher.hpp"
#include <numeric>

class singleAlgo : public algo
{
private:
    int totalInfected;
    int count = 0;
	int move_[7][2] = { {0,0},{0,1},{1,0},{1,1},{0,-1},{-1,0},{-1,-1} };
	std::default_random_engine* random_ = nullptr;
	std::vector<std::vector<human*>> backup;
    std::vector<std::vector<human*>> getArray(std::vector<human>* humans);
    std::vector<int> infectPeople;
    int _x, _y;
    grapher* infectRate = nullptr;
    grapher* infectedPeople = nullptr;
public:
	void run(std::vector<human>* humans, int infectChance, int infectRadius,int x,int y) override;
	~singleAlgo();
	void end();
};
