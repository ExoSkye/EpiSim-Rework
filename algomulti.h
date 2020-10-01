#pragma once
#include <thread>
#include "algorithm.h"


struct threadCreation
{
	std::vector<human>*& humans;
	int infectChance;
	int infectRadius;
	int start;
	int stop;
};

class multiThreadNaive : public algo
{
private:
	int move_[7][2] = { {0,0},{0,1},{1,0},{1,1},{0,-1},{-1,0},{-1,-1} };
	bool checkRadius(square checkBox, int px, int py);
	std::default_random_engine* random_ = nullptr;
	void threaded(threadCreation threadCreateInfo);
public:
	void run(std::vector<human>* humans, int infectChance, int infectRadius) override;
};