#pragma once
#include "algorithm.h"
template <int x, int y>
class singleAlgo : public algo
{
private:
	int move_[7][2] = { {0,0},{0,1},{1,0},{1,1},{0,-1},{-1,0},{-1,-1} };
	std::default_random_engine* random_ = nullptr;
	std::vector<std::vector<human*>> getArray(std::vector<human>* humans)
	{
		std::vector<std::vector<human*>> ret;
		ret.resize(x);
		for (std::vector<human*>& humanArray : ret)
		{
			humanArray.resize(y);
		}
		for (human person : *humans)
		{
			ret[person.x][person.y] = &person;
		}
		return ret;
	}
public:
	void run(std::vector<human>* humans, int infectChance, int infectRadius) override
	{
		std::vector<std::vector<human*>> peopleArray = getArray(humans);
	if (random_ == nullptr)
	{
		random_ = new std::default_random_engine();
	}
	
	for (human& person : *humans)
	{
		ZoneScoped;
		auto* movement = move_[random_->operator()() % 7];
		if (!(movement[0] + person.x < 0 || movement[0] + person.x >= x)) {
			person.x += movement[0];
		}
		if (!(movement[1] + person.y < 0 || movement[1] + person.y >= x)) {
			person.y += movement[1];
		}
		if (person.infect_info == infectInfo::infectious) {
			std::vector<human*> peopleCloseEnough;
			for (int x = person.x - infectRadius / 2; x < person.x + infectRadius / 2; x++)
			{
				for (int y = person.y - infectRadius / 2; y < person.y + infectRadius / 2; y++)
				{
					if (peopleArray[x][y] != nullptr && peopleArray[x][y]->infect_info != infectInfo::infectious)
					{
						peopleCloseEnough.emplace_back(peopleArray[x][y]);
					}
				}
			}
			for (human* person2 : peopleCloseEnough)
			{
				ZoneScoped;
				if ((random_->operator()() % 101) < infectChance)
				{
					person2->infect_info = infectInfo::infectious;
				}
			}
		}
	}
	}
};
