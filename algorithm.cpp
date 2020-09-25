#include "algorithm.h"

void singleCPUalgo::run(std::vector<human>* humans, int infectChance, int infectRadius)
{
	if (random_ == nullptr)
	{
		random_ = new std::default_random_engine();
	}
	for (int i = 0; i < humans->size();i++)
	{
		human person = humans->at(i);
		for (int i2 = 0; i2 < humans->size(); i2++)
		{
			human person2 = humans->at(i2);
			if (person.infect_info == infectInfo::infectious) {
				if (checkRadius(square{ person.x - (infectRadius / 2),person.x + (infectRadius / 2),person.y - (infectRadius / 2),person.y + (infectRadius / 2) }, person2.x, person2.y))
				{
					if ((random_->operator()() % 101) < infectChance)
					{
						person2.infect_info = infectInfo::infectious;
					}
				}
			}
		}
	}
}
