#include "algononnaive.h"

void singleAlgo::end() {
    infectRate->closeGraph();
    infectedPeople->closeGraph();
}

singleAlgo::~singleAlgo() {
    end();
}

std::vector<std::vector<human*>> singleAlgo::getArray(std::vector<human>* humans)
{
    ZoneScopedNC("Building array",0xff0000);
    std::vector<std::vector<human*>> ret = backup;

    for (int i = 0; i < humans->size(); i++)
    {
        ret[humans->at(i).x][humans->at(i).y] = &humans->at(i);
    }
    return ret;
}

double SgetRandom() {
    double lower_bound = 0;
    double upper_bound = 10000;
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;
    return unif(re);
}

#include <cmath>

void singleAlgo::run(std::vector<human> *humans, int infectChance, int infectRadius, int x, int y, double immuneChance,
                     int immuneLength,
                     int immuneLengthVar) {
    _x = x;
    _y = y;
    //if (infectRate == nullptr) {
        //infectRate = new grapher(128,std::string("Infection Rate"),2,128,2,true);
        //infectedPeople = new grapher(256,std::string("Total People Infected"),humans->size(),128,1,true);
    //}
    int peopleInfected = 0;
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
    {
        ZoneScopedNC("Movement",0x00ff00);
        for (human &person : *humans) {
            auto *movement = move_[random_->operator()() % 7];
            if (!(movement[0] + person.x < 0 || movement[0] + person.x >= _y)) {
                person.x += movement[0];
            }
            if (!(movement[1] + person.y < 0 || movement[1] + person.y >= _y)) {
                person.y += movement[1];
            }
        }
    }

    std::vector<std::vector<human*>> ret = getArray(humans);

    {
        ZoneScopedNC("Infection",0x0000ff);
        int infectCount = 0;
        for (human &person : *humans) {
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
                if (random_->operator()() % 101 < infectChance) {
                    person.infect_info = infectInfo::immune;
                }
                std::vector<human *> peopleCloseEnough;
                for (int x1 = person.x - infectRadius; x1 < person.x + infectRadius; x1++) {
                    for (int y1 = person.y - infectRadius; y1 < person.y + infectRadius; y1++) {
                        int temp_x = x1;
                        int temp_y = y1;
                        if (x1 < 0) {
                            temp_x = 0;
                        } else if (x1 >= _x) {
                            temp_x = x - 1;
                        }
                        if (y1 < 0) {
                            temp_y = 0;
                        } else if (y1 >= _y) {
                            temp_y = y - 1;
                        }
                        if (ret[temp_x][temp_y] != nullptr &&
                            ret[temp_x][temp_y]->infect_info != infectInfo::infectious &&
                            ret[temp_x][temp_y]->infect_info != infectInfo::immune) {
                            peopleCloseEnough.push_back(ret[temp_x][temp_y]);
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
        for (human person : *humans) {
            ret[person.x][person.y] = nullptr;
        }
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