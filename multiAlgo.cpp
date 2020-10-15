//
// Created by protobit on 05/10/2020.
//

#include "multiAlgo.h"

std::vector<std::vector<human *>> multiAlgo::getArray(std::vector<human> *humans) {
    ZoneScopedNC("Building array",0xff0000);
    std::vector<std::vector<human*>> ret = backup;

    for (int i = 0; i < humans->size(); i++)
    {
        ret[humans->at(i).x][humans->at(i).y] = &humans->at(i);
    }
    return ret;
}

void
multiAlgo::run(std::vector<human> *humans, int infectChance, int infectRadius, int x, int y, double immuneChance,
               int immuneLength,
               int immuneLengthVar) {
    _x = x;
    _y = y;
    peopleInfected = 0;
    /*if (infectRate == nullptr) {
        infectRate = new grapher(128,std::string("Infection Rate"),2,128,2,true);
        infectedPeople = new grapher(256,std::string("Total People Infected"),humans->size(),128,1,true);
    }*/
    if (infectPeople.size() == 20) {
        infectPeople.erase(infectPeople.cbegin());
    }
    if (backup.size() == 0) {
        backup.resize(_x);
        for (std::vector<human *> &humanArray : backup) {
            humanArray.resize(_y);
        }
    }
    if (random_ == nullptr) {
        random_ = new std::default_random_engine();
    }

    std::vector<std::vector<human*>> grid = getArray(humans);

    unsigned int cores = std::thread::hardware_concurrency();
    unsigned long long per_thread = humans->size() / cores;
    std::vector<std::thread> threads;
    unsigned long long humans_left = humans->size();
    for (int i = 0; i < cores-1; i++)
    {
        threads.emplace_back(std::thread(&multiAlgo::threadedFunc, this, humans,&grid,infectChance,infectRadius,i * per_thread,(i + 1) * per_thread,immuneChance));
        humans_left -= per_thread;
    }
    humans_left -= per_thread;
    threads.emplace_back(std::thread(&multiAlgo::threadedFunc, this, humans,&grid,infectChance,infectRadius,(cores - 1) * per_thread,cores*per_thread + humans_left-1,immuneChance));
    for (std::thread& curThread : threads)
    {
        curThread.join();
    }
    /*infectedPeople->append(totalInfected);
    infectedPeople->update();
    infectPeople.push_back(peopleInfected);
    if (count % 20 == 0) {
        infectRate->append(std::accumulate(infectPeople.begin(), infectPeople.end(),
                                           decltype(infectPeople)::value_type(0)));
    }
    infectRate->update();*/
    count++;
}

multiAlgo::~multiAlgo() {
    end();
}

void multiAlgo::end() {
    //infectedPeople->closeGraph();
    //infectRate->closeGraph();
}

double MgetRandom() {
    double lower_bound = 0;
    double upper_bound = 10000;
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;
    return unif(re);
}

#include <cmath>

void multiAlgo::threadedFunc(std::vector<human> *humans, std::vector<std::vector<human *>> *grid, int infectChance,
                             int infectRadius, int start, int end, double immuneChance) {
        {
            ZoneScopedNC("Movement", 0x00ff00);
            for (int i = start; i < end; i++) {
                human &person = humans->at(i);
                auto *movement = move_[random_->operator()() % 7];
                if (!(movement[0] + person.x < 0 || movement[0] + person.x >= _y)) {
                    person.x += movement[0];
                }
                if (!(movement[1] + person.y < 0 || movement[1] + person.y >= _y)) {
                    person.y += movement[1];
                }
            }
        }

        {
            ZoneScopedNC("Infection", 0x0000ff);
            int infectCount = 0;
            for (int i = start; i < end; i++) {
                human &person = humans->at(i);
                if (person.infect_info == infectInfo::infectious) {
                    infectCount++;
                    person.timeSinceInfected++;
                    if (static_cast<double>(random_->operator()() % 100001)/1000 < immuneChance) {
                        person.infect_info = infectInfo::immune;
                    }
                    else {
                        std::vector<human *> peopleCloseEnough;
                        for (int x1 = person.x - infectRadius; x1 < person.x + infectRadius; x1++) {
                            for (int y1 = person.y - infectRadius; y1 < person.y + infectRadius; y1++) {
                                int temp_x = x1;
                                int temp_y = y1;
                                if (x1 < 0) {
                                    temp_x = 0;
                                } else if (x1 >= _x) {
                                    temp_x = _x - 1;
                                }
                                if (y1 < 0) {
                                    temp_y = 0;
                                } else if (y1 >= _y) {
                                    temp_y = _y - 1;
                                }
                                if (grid->at(temp_x)[temp_y] != nullptr &&
                                    grid->at(temp_x)[temp_y]->infect_info != infectInfo::infectious &&
                                    grid->at(temp_x)[temp_y]->infect_info != infectInfo::immune) {
                                    peopleCloseEnough.push_back(grid->at(temp_x)[temp_y]);
                                }
                            }
                        }
                        if (infectCount > totalInfected) {
                            totalInfected = infectCount;
                        }
                        for (human *person2 : peopleCloseEnough) {
                            ZoneScoped;
                            if ((random_->operator()() % 101) < infectChance) {
                                person2->infect_info = infectInfo::infectious;
                                peopleInfected++;
                                person.peopleInfected++;
                            }
                        }
                    }
                }
            }
        }
}
