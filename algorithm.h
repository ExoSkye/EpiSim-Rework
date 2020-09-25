#pragma once
#include <vector>
#include <random>
#include "human.h"

struct square
{
	int x1, x2, y1, y2;
};

class algo
{
public:
	virtual void run(std::vector<human>* humans, int infectChance, int infectRadius) = 0;
};

class singleCPUalgo : public algo
{
private:
	bool checkRadius(square checkBox, int px, int py);
	std::default_random_engine* random_ = nullptr;
public:
	void run(std::vector<human>* humans, int infectChance, int infectRadius) override;
};