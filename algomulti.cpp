#include "algomulti.h"

bool multiThreadNaive::checkRadius(square checkBox, int px, int py)
{
	return (px > checkBox.x1 && px < checkBox.x2&& py > checkBox.y1 && py < checkBox.y2);
}

void multiThreadNaive::threaded(threadCreation threadCreateInfo)
{
	for (int i = threadCreateInfo.start; i < threadCreateInfo.stop; i++)
	{
		human& person = threadCreateInfo.humans->at(i);
		ZoneScoped;
		auto* movement = move_[random_->operator()() % 7];
		person.x += movement[0];
		person.y += movement[1];
		for (human& person2 : *threadCreateInfo.humans)
		{
			if (person.infect_info == infectInfo::infectious) {
				if (checkRadius(square{ person.x - (threadCreateInfo.infectRadius / 2),person.x + (threadCreateInfo.infectRadius / 2),person.y - (threadCreateInfo.infectRadius / 2),person.y + (threadCreateInfo.infectRadius / 2) }, person2.x, person2.y))
				{
					if ((random_->operator()() % 101) < threadCreateInfo.infectChance)
					{
						person2.infect_info = infectInfo::infectious;
					}
				}
			}
		}
	}
}



void
multiThreadNaive::run(std::vector<human> *humans, int infectChance, int infectRadius, int x, int y, double immuneChance,
                      int immuneLength,
                      int immuneLengthVar)
{
	if (random_ == nullptr)
	{
		random_ = new std::default_random_engine();
	}
	unsigned int cores = std::thread::hardware_concurrency();
	unsigned long long per_thread = humans->size() / cores;
	std::vector<std::thread> threads;
	unsigned long long humans_left = humans->size();
	threadCreation threadCreateInfo{ humans,infectChance,infectRadius,0,0 };
	for (int i = 0; i < cores-1; i++)
	{
		threadCreateInfo.start = i * per_thread;
		threadCreateInfo.stop = (i + 1) * per_thread;
		threads.emplace_back(std::thread(&multiThreadNaive::threaded, this, threadCreateInfo));
		humans_left -= per_thread;
	}
	humans_left -= per_thread;
	threadCreateInfo.start = (cores - 1) * per_thread;
	threadCreateInfo.stop = (cores)*per_thread + humans_left-1;
	threads.emplace_back(std::thread(&multiThreadNaive::threaded, this, threadCreateInfo));
	for (std::thread& curThread : threads)
	{
		curThread.join();
	}

} 
