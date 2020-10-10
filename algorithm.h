#pragma once
#include <vector>
#include <random>
#include "human.h"
#undef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#include "tracy/TracyC.h"
struct square
{
	int x1, x2, y1, y2;
};

class algo
{
public:
	virtual void run(std::vector<human>* humans, int infectChance, int infectRadius,int x, int y) = 0;
	virtual void end() = 0;
};

class singleCPUalgo : public algo
{
private:
	int move_[7][2] = { {0,0},{0,1},{1,0},{1,1},{0,-1},{-1,0},{-1,-1} };
	bool checkRadius(square checkBox, int px, int py);
	std::default_random_engine* random_ = nullptr;
public:
	void run(std::vector<human>* humans, int infectChance, int infectRadius, int x, int y) override;
	void end() override {
	    ;
	}
};