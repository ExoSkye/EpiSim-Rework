#include "algorithm.h"

bool singleCPUalgo::checkRadius(square checkBox, int px, int py)
{
	return (px > checkBox.x1 && px < checkBox.x2&& py > checkBox.y1 && py < checkBox.y2);
}


void singleCPUalgo::run(std::vector<human>* humans, int infectChance, int infectRadius, int x, int y)
{
	if (random_ == nullptr)
	{
		random_ = new std::default_random_engine();
	}
	for (human& person : * humans)
	{
		ZoneScoped;
		auto *movement = move_[random_->operator()() % 7];
		person.x += movement[0];
		person.y += movement[1];
		for (human& person2 : *humans)
		{
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
