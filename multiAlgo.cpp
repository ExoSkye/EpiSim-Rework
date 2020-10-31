//
// Created by protobit on 05/10/2020.
//

#include "multiAlgo.h"

std::vector<std::vector<human *>>* multiAlgo::getArray(std::vector<human> *humans) {
    ZoneScopedNC("Building array",0xff0000);
#pragma omp parallel for
    for (int i = 0; i < humans->size(); i++)
    {
        backup[humans->at(i).x][humans->at(i).y] = &humans->at(i);
    }
    return &backup;
}

void
multiAlgo::run(std::vector<human> *humans, int infectChance, int infectRadius, int x, int y, double immuneChance,
               int immuneLength,
               int immuneLengthVar,
               int timestep) {
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

    std::vector<std::vector<human*>>* grid = getArray(humans);
    std::vector<human> humanBackup = *humans;
    unsigned int cores = std::thread::hardware_concurrency();
    unsigned long long per_thread = humans->size() / cores;
    std::vector<std::thread> threads;
    unsigned long long humans_left = humans->size();
    for (int i = 0; i < cores-1; i++)
    {
        threads.emplace_back(std::thread(std::mem_fn(&multiAlgo::threadedFunc), this, humans,grid,infectChance,infectRadius,i * per_thread,
	(i + 1) * per_thread,immuneChance,immuneLength,immuneLengthVar,timestep));
        humans_left -= per_thread;
    }
    humans_left -= per_thread;
    threads.emplace_back(std::thread(std::mem_fn(&multiAlgo::threadedFunc), this, humans,grid,infectChance,infectRadius,(cores - 1) * per_thread,cores*per_thread + humans_left-1,immuneChance,immuneLength,immuneLengthVar,timestep));
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
    for (int i = 0; i < humanBackup.size(); i++) {
        backup[humanBackup[i].x][humanBackup[i].y] = nullptr;
    }
    count++;

}

multiAlgo::~multiAlgo() {
    end();
}

void multiAlgo::end() {
    //infectedPeople->closeGraph();
    //infectRate->closeGraph();
}

#include <cmath>

void multiAlgo::threadedFunc(std::vector<human> *humans, std::vector<std::vector<human *>> *grid, int infectChance,
                             int infectRadius, int start, int end, double immuneChance, int immuneLength, int immuneLengthVar,
                             int timestep) {
        {
            ZoneScopedNC("Movement", 0x00ff00);
#ifdef OMPEnable
#pragma omp simd
            for (int i = start; i < end; i++) {
#else
        for (int i = start; i < end; i++) {
#endif
                human &person = humans->at(i);
                int addx = random_->operator()() % (timestep * 2 + 1) - timestep;
                int addy = random_->operator()() % (timestep * 2 + 1) - timestep;
                if (!(addx + person.x < 0 || addx + person.x >= _x)) {
                    person.x += addx;
                }
                if (!(addy + person.y < 0 || addy + person.y >= _y)) {
                    person.y += addy;
                }
            }
        }

        {
            ZoneScopedNC("Infection", 0x0000ff);
            int infectCount = 0;
#ifdef OMPEnable
#pragma omp simd
            for (int i = start; i < end; i++) {
#else
        for (int i = start; i < end; i++) {
#endif
                human &person = humans->at(i);
                if (person.infect_info == infectInfo::immune) {
                    if (person.time >=
                        immuneLength + ((random_->operator()() % (immuneLengthVar * 2)) - immuneLengthVar)) {
                        person.infect_info = infectInfo::susceptible;
                        person.time = 0;
                    }
                }
                person.time++;
                if (person.infect_info == infectInfo::infectious) {
                    infectCount++;
                    if (static_cast<double>(random_->operator()() % 100001)/1000 < immuneChance) {
                        person.infect_info = infectInfo::immune;
                        person.peopleInfected = 0;
                    }
                    person.time++;
                        std::vector<human *> peopleCloseEnough;
#ifdef OMPEnable
#pragma omp simd
                        for (int x1 = person.x - infectRadius; x1 < person.x + infectRadius; x1++) {
#pragma omp simd
                            for (int y1 = person.y - infectRadius; y1 < person.y + infectRadius; y1++) {
#else
                        for (int x1 = person.x - infectRadius; x1 < person.x + infectRadius; x1++) {
                            for (int y1 = person.y - infectRadius; y1 < person.y + infectRadius; y1++) {
#endif
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
