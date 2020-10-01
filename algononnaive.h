#pragma once
#include "algorithm.h"
template <int x, int y>
class singleAlgo : public algo
{
private:
	int move_[7][2] = { {0,0},{0,1},{1,0},{1,1},{0,-1},{-1,0},{-1,-1} };
	std::default_random_engine* random_ = nullptr;
    std::vector<std::vector<human*>> ret;
    void getArray(std::vector<human>* humans)
	{
        ZoneScopedNC("Building array",0xff0000);
        if (ret.size() == 0) {
            ret.resize(x);
        }
        if (ret[0].size() == 0) {
            for (std::vector<human *> &humanArray : ret) {
                humanArray.resize(y);
            }
        }
		for (int i = 0; i < humans->size(); i++)
		{
			ret[humans->at(i).x][humans->at(i).y] = &humans->at(i);
		}
	}
public:
	void run(std::vector<human>* humans, int infectChance, int infectRadius) override {
        getArray(humans);
        if (random_ == nullptr) {
            random_ = new std::default_random_engine();
        }

        for (human &person : *humans) {
            ZoneScoped;
            auto *movement = move_[random_->operator()() % 7];
            if (!(movement[0] + person.x < 0 || movement[0] + person.x >= x)) {
                person.x += movement[0];
            }
            if (!(movement[1] + person.y < 0 || movement[1] + person.y >= x)) {
                person.y += movement[1];
            }
            if (person.infect_info == infectInfo::infectious) {
                std::vector<human *> peopleCloseEnough;
                for (int x1 = person.x - infectRadius / 2; x1 < person.x + infectRadius / 2; x1++) {
                    for (int y1 = person.y - infectRadius / 2; y1 < person.y + infectRadius / 2; y1++) {
                        if (x1 < 0) {
                            x1 = 0;
                        } else if (x1 >= x) {
                            x1 = x - 1;
                        }
                        if (y1 < 0) {
                            y1 = 0;
                        } else if (y1 >= y) {
                            y1 = y - 1;
                        }
                        if (ret[x1][y1] != nullptr &&
                            ret[x1][y1]->infect_info != infectInfo::infectious) {
                            peopleCloseEnough.push_back(ret[x1][y1]);
                        }
                    }
                }
                for (human *person2 : peopleCloseEnough) {
                    ZoneScoped;
                    if ((random_->operator()() % 101) < infectChance) {
                        person2->infect_info = infectInfo::infectious;
                    }
                }
            }
        }
        for (human person : *humans) {
            ret[person.x][person.y] = nullptr;
        }
    }
};
